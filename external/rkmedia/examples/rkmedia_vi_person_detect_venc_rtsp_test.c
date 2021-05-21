// Copyright 2020 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "librtsp/rtsp_demo.h"
#include "rkmedia_api.h"
#include "rkmedia_api.h"
#include "rkmedia_venc.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "stdbool.h"
#include "rockx.h"

#define MAX_SESSION_NUM 2
#define DRAW_INDEX 0
#define RK_NN_INDEX 1
#define MAX_RKNN_LIST_NUM 10
#define UPALIGNTO(value, align) ((value + align - 1) & (~(align - 1)))
#define UPALIGNTO16(value) UPALIGNTO(value, 16)
#define RKNN_RUNTIME_LIB_PATH "/usr/lib/librknn_runtime.so"

typedef struct node {
  long timeval;
  rockx_object_array_t person_array;
  struct node *next;
} Node;

typedef struct my_stack {
  int size;
  Node *top;
} rknn_list;

void create_rknn_list(rknn_list **s) {
  if (*s != NULL)
    return;
  *s = (rknn_list *)malloc(sizeof(rknn_list));
  (*s)->top = NULL;
  (*s)->size = 0;
  printf("create rknn_list success\n");
}

void destory_rknn_list(rknn_list **s) {
  Node *t = NULL;
  if (*s == NULL)
    return;
  while ((*s)->top) {
    t = (*s)->top;
    (*s)->top = t->next;
    free(t);
  }
  free(*s);
  *s = NULL;
}

void rknn_list_push(rknn_list *s, long timeval,
                    rockx_object_array_t person_array) {
  Node *t = NULL;
  t = (Node *)malloc(sizeof(Node));
  t->timeval = timeval;
  t->person_array = person_array;
  if (s->top == NULL) {
    s->top = t;
    t->next = NULL;
  } else {
    t->next = s->top;
    s->top = t;
  }
  s->size++;
}

void rknn_list_pop(rknn_list *s, long *timeval,
                   rockx_object_array_t *person_array) {
  Node *t = NULL;
  if (s == NULL || s->top == NULL)
    return;
  t = s->top;
  *timeval = t->timeval;
  *person_array = t->person_array;
  s->top = t->next;
  free(t);
  s->size--;
}

void rknn_list_drop(rknn_list *s) {
  Node *t = NULL;
  if (s == NULL || s->top == NULL)
    return;
  t = s->top;
  s->top = t->next;
  free(t);
  s->size--;
}

int rknn_list_size(rknn_list *s) {
  if (s == NULL)
    return -1;
  return s->size;
}

rtsp_demo_handle g_rtsplive = NULL;

struct Session {
  char path[64];
  CODEC_TYPE_E video_type;
  RK_U32 u32Width;
  RK_U32 u32Height;
  IMAGE_TYPE_E enImageType;
  char videopath[120];

  rtsp_session_handle session;
  MPP_CHN_S stViChn;
  MPP_CHN_S stVenChn;
};

struct demo_cfg {
  int session_count;
  struct Session session_cfg[MAX_SESSION_NUM];
};

struct demo_cfg cfg;
rknn_list *rknn_list_;

static int g_flag_run = 1;

static void sig_proc(int signo) {
  fprintf(stderr, "signal %d\n", signo);
  g_flag_run = 0;
}

static long getCurrentTimeMsec() {
  long msec = 0;
  char str[20] = {0};
  struct timeval stuCurrentTime;

  gettimeofday(&stuCurrentTime, NULL);
  sprintf(str, "%ld%03ld", stuCurrentTime.tv_sec,
          (stuCurrentTime.tv_usec) / 1000);
  for (size_t i = 0; i < strlen(str); i++) {
    msec = msec * 10 + (str[i] - '0');
  }

  return msec;
}

int nv12_border(char *pic, int pic_w, int pic_h, int rect_x, int rect_y,
                int rect_w, int rect_h, int R, int G, int B) {
  /* Set up the rectangle border size */
  const int border = 5;

  /* RGB convert YUV */
  int Y, U, V;
  Y = 0.299 * R + 0.587 * G + 0.114 * B;
  U = -0.1687 * R + 0.3313 * G + 0.5 * B + 128;
  V = 0.5 * R - 0.4187 * G - 0.0813 * B + 128;
  /* Locking the scope of rectangle border range */
  int j, k;
  for (j = rect_y; j < rect_y + rect_h; j++) {
    for (k = rect_x; k < rect_x + rect_w; k++) {
      if (k < (rect_x + border) || k > (rect_x + rect_w - border) ||
          j < (rect_y + border) || j > (rect_y + rect_h - border)) {
        /* Components of YUV's storage address index */
        int y_index = j * pic_w + k;
        int u_index =
            (y_index / 2 - pic_w / 2 * ((j + 1) / 2)) * 2 + pic_w * pic_h;
        int v_index = u_index + 1;
        /* set up YUV's conponents value of rectangle border */
        pic[y_index] = Y;
        pic[u_index] = U;
        pic[v_index] = V;
      }
    }
  }

  return 0;
}

static void *GetMediaBuffer(void *arg) {
  printf("#Start %s thread, arg:%p\n", __func__, arg);
  // rockx
  rockx_ret_t rockx_ret;
  rockx_handle_t object_det_handle;
  rockx_image_t input_image;
  input_image.height = cfg.session_cfg[RK_NN_INDEX].u32Height;
  input_image.width = cfg.session_cfg[RK_NN_INDEX].u32Width;
  input_image.pixel_format = ROCKX_PIXEL_FORMAT_YUV420SP_NV12;
  input_image.is_prealloc_buf = 1;
  // create a object detection handle
  rockx_config_t *config = rockx_create_config();
  rockx_add_config(config, ROCKX_CONFIG_RKNN_RUNTIME_PATH,
                   RKNN_RUNTIME_LIB_PATH);
  rockx_ret = rockx_create(&object_det_handle, ROCKX_MODULE_PERSON_DETECTION_V2,
                           config, sizeof(rockx_config_t));
  if (rockx_ret != ROCKX_RET_SUCCESS) {
    printf("init rockx module ROCKX_MODULE_PERSON_DETECTION_V2 error %d\n",
           rockx_ret);
    return NULL;
  }
  printf("ROCKX_MODULE_PERSON_DETECTION_V2 rockx_create success\n");
  // create rockx_object_array_t for store result
  rockx_object_array_t person_array;
  memset(&person_array, 0, sizeof(person_array));

  // create a object track handle
  rockx_handle_t object_track_handle;
  rockx_ret =
      rockx_create(&object_track_handle, ROCKX_MODULE_OBJECT_TRACK, NULL, 0);
  if (rockx_ret != ROCKX_RET_SUCCESS) {
    printf("init rockx module ROCKX_MODULE_OBJECT_DETECTION error %d\n",
           rockx_ret);
  }

  MEDIA_BUFFER buffer = NULL;
  while (g_flag_run) {
    buffer = RK_MPI_SYS_GetMediaBuffer(RK_ID_VI, RK_NN_INDEX, -1);
    if (!buffer) {
      continue;
    }

    // printf("Get Frame:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
    //        "timestamp:%lld\n",
    //        RK_MPI_MB_GetPtr(buffer), RK_MPI_MB_GetFD(buffer),
    //        RK_MPI_MB_GetSize(buffer),
    //        RK_MPI_MB_GetModeID(buffer), RK_MPI_MB_GetChannelID(buffer),
    //        RK_MPI_MB_GetTimestamp(buffer));
    input_image.size = RK_MPI_MB_GetSize(buffer);
    input_image.data = RK_MPI_MB_GetPtr(buffer);
    // detect object
    long nn_before_time = getCurrentTimeMsec();
    rockx_ret = rockx_person_detect(object_det_handle, &input_image,
                                    &person_array, NULL);
    if (rockx_ret != ROCKX_RET_SUCCESS)
      printf("rockx_person_detect error %d\n", rockx_ret);
    long nn_after_time = getCurrentTimeMsec();
    printf("Algorithm time-consuming is %ld\n",
           (nn_after_time - nn_before_time));
    // printf("person_array.count is %d\n", person_array.count);

    rockx_object_array_t out_track_objects;
    rockx_ret = rockx_object_track(object_track_handle, input_image.width,
                                   input_image.height, 10, &person_array,
                                   &out_track_objects);
    if (rockx_ret != ROCKX_RET_SUCCESS)
      printf("rockx_object_track error %d\n", rockx_ret);
    // process result
    if (person_array.count > 0) {
      rknn_list_push(rknn_list_, getCurrentTimeMsec(), person_array);
      int size = rknn_list_size(rknn_list_);
      if (size >= MAX_RKNN_LIST_NUM)
        rknn_list_drop(rknn_list_);
      // printf("size is %d\n", size);
    }
    RK_MPI_MB_ReleaseBuffer(buffer);
  }
  // release
  rockx_image_release(&input_image);
  rockx_destroy(object_track_handle);
  rockx_destroy(object_det_handle);

  return NULL;
}

static void *MainStream() {
  MEDIA_BUFFER buffer;
  float x_rate = (float)cfg.session_cfg[DRAW_INDEX].u32Width /
                 (float)cfg.session_cfg[RK_NN_INDEX].u32Width;
  float y_rate = (float)cfg.session_cfg[DRAW_INDEX].u32Height /
                 (float)cfg.session_cfg[RK_NN_INDEX].u32Height;
  printf("x_rate is %f, y_rate is %f\n", x_rate, y_rate);

  while (g_flag_run) {
    buffer = RK_MPI_SYS_GetMediaBuffer(
        RK_ID_VI, cfg.session_cfg[DRAW_INDEX].stViChn.s32ChnId, -1);
    if (!buffer)
      continue;
    // draw
    if (rknn_list_size(rknn_list_)) {
      long time_before;
      rockx_object_array_t person_array;
      memset(&person_array, 0, sizeof(person_array));
      rknn_list_pop(rknn_list_, &time_before, &person_array);
      // printf("time interval is %ld\n", getCurrentTimeMsec() - time_before);

      for (int j = 0; j < person_array.count; j++) {
        // printf("person_array.count is %d\n", person_array.count);
        // const char *cls_name =
        // OBJECT_DETECTION_LABELS_91[person_array.object[j].cls_idx];
        // int left = person_array.object[j].box.left;
        // int top = person_array.object[j].box.top;
        // int right = person_array.object[j].box.right;
        // int bottom = person_array.object[j].box.bottom;
        // float score = person_array.object[j].score;
        // printf("box=(%d %d %d %d) cls_name=%s, score=%f\n", left, top, right,
        // bottom, cls_name, score);

        int x = person_array.object[j].box.left * x_rate;
        int y = person_array.object[j].box.top * y_rate;
        int w = (person_array.object[j].box.right -
                 person_array.object[j].box.left) *
                x_rate;
        int h = (person_array.object[j].box.bottom -
                 person_array.object[j].box.top) *
                y_rate;
        if (x < 0)
          x = 0;
        if (y < 0)
          y = 0;
        while ((uint32_t)(x + w) >= cfg.session_cfg[DRAW_INDEX].u32Width) {
          w -= 16;
        }
        while ((uint32_t)(y + h) >= cfg.session_cfg[DRAW_INDEX].u32Height) {
          h -= 16;
        }
        printf("border=(%d %d %d %d)\n", x, y, w, h);
        nv12_border((char *)RK_MPI_MB_GetPtr(buffer),
                    cfg.session_cfg[DRAW_INDEX].u32Width,
                    cfg.session_cfg[DRAW_INDEX].u32Height, x, y, w, h, 0, 0,
                    255);
      }
    }
    // send from VI to VENC
    RK_MPI_SYS_SendMediaBuffer(
        RK_ID_VENC, cfg.session_cfg[DRAW_INDEX].stVenChn.s32ChnId, buffer);
    RK_MPI_MB_ReleaseBuffer(buffer);
  }

  return NULL;
}

static void dump_cfg() {
  for (int i = 0; i < cfg.session_count; i++) {
    printf("rtsp path = %s.\n", cfg.session_cfg[i].path);
    printf("video_type = %d.\n", cfg.session_cfg[i].video_type);
    printf("width = %d.\n", cfg.session_cfg[i].u32Width);
    printf("height = %d.\n", cfg.session_cfg[i].u32Height);
    printf("video path =%s.\n", cfg.session_cfg[i].videopath);
    printf("image type = %u.\n", cfg.session_cfg[i].enImageType);
  }
}

static int load_cfg(const char *cfg_file) {
  // cfgline:
  // path=%s video_type=%d width=%u height=%u
  FILE *fp = fopen(cfg_file, "r");
  char line[1024];
  int count = 0;

  if (!fp) {
    fprintf(stderr, "open %s failed\n", cfg_file);
    return -1;
  }

  memset(&cfg, 0, sizeof(cfg));
  while (fgets(line, sizeof(line) - 1, fp)) {
    const char *p;
    // char codec_type[20];
    memset(&cfg.session_cfg[count], 0, sizeof(cfg.session_cfg[count]));

    if (line[0] == '#')
      continue;
    p = strstr(line, "path=");
    if (!p)
      continue;
    if (sscanf(p, "path=%s", cfg.session_cfg[count].path) != 1)
      continue;

    if ((p = strstr(line, "video_type="))) {
      if (sscanf(p,
                 "video_type=%d width=%u height=%u image_type=%u video_path=%s",
                 &cfg.session_cfg[count].video_type,
                 &cfg.session_cfg[count].u32Width,
                 &cfg.session_cfg[count].u32Height,
                 &cfg.session_cfg[count].enImageType,
                 cfg.session_cfg[count].videopath) == 0) {
        printf("parse video file failed %s.\n", p);
      }
    }
    if (cfg.session_cfg[count].video_type != RK_CODEC_TYPE_NONE) {
      count++;
    } else {
      printf("parse line %s failed\n", line);
    }
  }
  cfg.session_count = count;
  fclose(fp);
  dump_cfg();
  return count;
}

static void SAMPLE_COMMON_VI_Start(struct Session *session,
                                   VI_CHN_WORK_MODE mode) {
  VI_CHN_ATTR_S vi_chn_attr;

  vi_chn_attr.u32BufCnt = 3;
  vi_chn_attr.u32Width = session->u32Width;
  vi_chn_attr.u32Height = session->u32Height;
  vi_chn_attr.enWorkMode = mode;
  vi_chn_attr.pcVideoNode = session->videopath;
  vi_chn_attr.enPixFmt = session->enImageType;

  RK_MPI_VI_SetChnAttr(0, session->stViChn.s32ChnId, &vi_chn_attr);
  RK_MPI_VI_EnableChn(0, session->stViChn.s32ChnId);
}

static void SAMPLE_COMMON_VENC_Start(struct Session *session) {
  VENC_CHN_ATTR_S venc_chn_attr;

  venc_chn_attr.stVencAttr.enType = session->video_type;

  venc_chn_attr.stVencAttr.imageType = session->enImageType;

  venc_chn_attr.stVencAttr.u32PicWidth = session->u32Width;
  venc_chn_attr.stVencAttr.u32PicHeight = session->u32Height;
  venc_chn_attr.stVencAttr.u32VirWidth = session->u32Width;
  venc_chn_attr.stVencAttr.u32VirHeight = session->u32Height;
  venc_chn_attr.stVencAttr.u32Profile = 77;

  int env_fps = 30;
  char *fps_c = getenv("FPS");
  if (fps_c) {
    printf("FPS: %s\n", fps_c);
    env_fps = atoi(fps_c);
  }
  printf("env_fps: %d\n", env_fps);
  switch (venc_chn_attr.stVencAttr.enType) {
  case RK_CODEC_TYPE_H264:
    venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
    venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = 30;
    venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate =
        session->u32Width * session->u32Height * 30 / 14;
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = 30;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = 30;
    break;
  case RK_CODEC_TYPE_H265:
    venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
    venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = 30;
    venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate =
        session->u32Width * session->u32Height * 30 / 14;
    venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum = 30;
    venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum = 30;
    break;
  default:
    printf("error: video codec not support.\n");
    break;
  }

  RK_MPI_VENC_CreateChn(session->stVenChn.s32ChnId, &venc_chn_attr);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("You must input the file of cfg.\n");
    printf("example file: oem/usr/share/rtsp-nn.cfg.\n");
    return -1;
  }
  load_cfg(argv[1]);

  // init rtsp
  printf("init rtsp\n");
  g_rtsplive = create_rtsp_demo(554);

  // init mpi
  printf("init mpi\n");
  RK_MPI_SYS_Init();

  // create session
  for (int i = 0; i < cfg.session_count; i++) {
    cfg.session_cfg[i].session =
        rtsp_new_session(g_rtsplive, cfg.session_cfg[i].path);

    // VI create
    printf("VI create\n");
    cfg.session_cfg[i].stViChn.enModId = RK_ID_VI;
    cfg.session_cfg[i].stViChn.s32ChnId = i;
    if (i == RK_NN_INDEX)
      SAMPLE_COMMON_VI_Start(&cfg.session_cfg[i], VI_WORK_MODE_GOD_MODE);
    else
      SAMPLE_COMMON_VI_Start(&cfg.session_cfg[i], VI_WORK_MODE_NORMAL);
    // VENC create
    printf("VENC create\n");
    cfg.session_cfg[i].stVenChn.enModId = RK_ID_VENC;
    cfg.session_cfg[i].stVenChn.s32ChnId = i;
    SAMPLE_COMMON_VENC_Start(&cfg.session_cfg[i]);
    if (i == DRAW_INDEX)
      RK_MPI_VI_StartStream(0, cfg.session_cfg[i].stViChn.s32ChnId);
    else
      RK_MPI_SYS_Bind(&cfg.session_cfg[i].stViChn,
                      &cfg.session_cfg[i].stVenChn);

    // rtsp video
    printf("rtsp video\n");
    switch (cfg.session_cfg[i].video_type) {
    case RK_CODEC_TYPE_H264:
      rtsp_set_video(cfg.session_cfg[i].session, RTSP_CODEC_ID_VIDEO_H264, NULL,
                     0);
      break;
    case RK_CODEC_TYPE_H265:
      rtsp_set_video(cfg.session_cfg[i].session, RTSP_CODEC_ID_VIDEO_H265, NULL,
                     0);
      break;
    default:
      printf("video codec not support.\n");
      break;
    }

    rtsp_sync_video_ts(cfg.session_cfg[i].session, rtsp_get_reltime(),
                       rtsp_get_ntptime());
  }

  create_rknn_list(&rknn_list_);
  // Get the sub-stream buffer for humanoid recognition
  pthread_t read_thread;
  pthread_create(&read_thread, NULL, GetMediaBuffer, NULL);
  // The mainstream draws a box asynchronously based on the recognition result
  pthread_t main_stream_thread;
  pthread_create(&main_stream_thread, NULL, MainStream, NULL);

  signal(SIGINT, sig_proc);
  while (g_flag_run) {
    for (int i = 0; i < cfg.session_count; i++) {
      MEDIA_BUFFER buffer;
      // send video buffer
      buffer = RK_MPI_SYS_GetMediaBuffer(
          RK_ID_VENC, cfg.session_cfg[i].stVenChn.s32ChnId, 0);
      if (buffer) {
        rtsp_tx_video(cfg.session_cfg[i].session, RK_MPI_MB_GetPtr(buffer),
                      RK_MPI_MB_GetSize(buffer),
                      RK_MPI_MB_GetTimestamp(buffer));
        RK_MPI_MB_ReleaseBuffer(buffer);
      }
    }
    rtsp_do_event(g_rtsplive);
  }

  for (int i = 0; i < cfg.session_count; i++) {
    if (i != DRAW_INDEX)
      RK_MPI_SYS_UnBind(&cfg.session_cfg[i].stViChn,
                        &cfg.session_cfg[i].stVenChn);
    RK_MPI_VI_DisableChn(0, cfg.session_cfg[i].stViChn.s32ChnId);
    RK_MPI_VENC_DestroyChn(cfg.session_cfg[i].stVenChn.s32ChnId);
  }
  rtsp_del_demo(g_rtsplive);
  destory_rknn_list(&rknn_list_);
  return 0;
}
