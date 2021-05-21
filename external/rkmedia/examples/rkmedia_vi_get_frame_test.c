// Copyright 2020 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "common/sample_common.h"
#include "rkmedia_api.h"
#include "rkmedia_venc.h"

typedef struct {
  char *file_path;
  int frame_cnt;
} OutputArgs;

static bool quit = false;
static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  quit = true;
}

static void *GetMediaBuffer(void *arg) {
  OutputArgs *outArgs = (OutputArgs *)arg;
  char *save_path = outArgs->file_path;
  int save_cnt = outArgs->frame_cnt;
  FILE *save_file = fopen(save_path, "w");
  if (!save_file)
    printf("ERROR: Open %s failed!\n", save_path);

  MEDIA_BUFFER mb = NULL;
  while (!quit) {
    mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VI, 0, -1);
    if (!mb) {
      printf("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
      break;
    }

    MB_IMAGE_INFO_S stImageInfo = {0};
    int ret = RK_MPI_MB_GetImageInfo(mb, &stImageInfo);
    if (ret)
      printf("Warn: Get image info failed! ret = %d\n", ret);

    printf("Get Frame:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
           "timestamp:%lld, ImgInfo:<wxh %dx%d, fmt 0x%x>\n",
           RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb), RK_MPI_MB_GetSize(mb),
           RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb),
           RK_MPI_MB_GetTimestamp(mb), stImageInfo.u32Width,
           stImageInfo.u32Height, stImageInfo.enImgType);

    if (save_file && (save_cnt-- > 0)) {
      fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), save_file);
      printf("#Save frame-%d to %s\n", save_cnt, save_path);
    }

    RK_MPI_MB_ReleaseBuffer(mb);

    // exit when complete and error
    if (!save_file) {
      quit = true;
      printf("target file is null, exit!\n");
      break;
    } else if (save_cnt <= 0) {
      quit = true;
      printf("Output is completed!\n");
      break;
    }
  }

  if (save_file)
    fclose(save_file);

  return NULL;
}

static RK_CHAR optstr[] = "?:a::w:h:c:o:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"device_name", required_argument, NULL, 'd'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"frame_cnt", required_argument, NULL, 'c'},
    {"output_path", required_argument, NULL, 'o'},
    {NULL, 0, NULL, 0},
};

static void print_usage(const RK_CHAR *name) {
  printf("usage example:\n");
#ifdef RKAIQ
  printf("\t%s [-a | --aiq /oem/etc/iqfiles/] [-w | --width 1920] "
         "[-h | --heght 1080]"
         "[-c | --frame_cnt 10] "
         "[-d | --device_name rkispp_scale0] "
         "[-o | --output_path /tmp/1080p.nv12] \n",
         name);
  printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a "
         "/oem/etc/iqfiles/, "
         "set dirpath emtpty to using path by default, without this option aiq "
         "should run in other application\n");
#else
  printf("\t%s [-w | --width 1920] "
         "[-h | --heght 1080]"
         "[-c | --frame_cnt 10] "
         "[-d | --device_name rkispp_scale0] "
         "[-o | --output_path /tmp/1080p.nv12] \n",
         name);
#endif
  printf("\t-w | --width: VI width, Default:1920\n");
  printf("\t-h | --heght: VI height, Default:1080\n");
  printf("\t-d | --device_name set pcDeviceName, Default:rkispp_scale0\n");
  printf("\t-c | --frame_cnt: record frame, Default:10\n");
  printf("\t-o | --output_path: output path, Default:/tmp/1080p.nv12\n");
  printf("Notice: fmt always NV12\n");
}

int main(int argc, char *argv[]) {
  RK_U32 u32Width = 1920;
  RK_U32 u32Height = 1080;
  RK_U32 u32FrameCnt = 10;
  RK_CHAR *pDeviceName = "rkispp_scale0";
  RK_CHAR *pOutPath = "/tmp/1080p.nv12";
  RK_CHAR *pIqfilesPath = NULL;
  int c;
  int ret = 0;
  while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
    printf("get op is %d, a is %d\n", c, 'a');
    const char *tmp_optarg = optarg;
    switch (c) {
    case 'a':
      if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
        tmp_optarg = argv[optind++];
      }
      if (tmp_optarg) {
        pIqfilesPath = (char *)tmp_optarg;
      } else {
        pIqfilesPath = "/oem/etc/iqfiles";
      }
      break;
    case 'w':
      u32Width = atoi(optarg);
      break;
    case 'h':
      u32Height = atoi(optarg);
      break;
    case 'c':
      u32FrameCnt = atoi(optarg);
      break;
    case 'o':
      pOutPath = optarg;
      break;
    case 'd':
      pDeviceName = optarg;
      break;
    case '?':
    default:
      print_usage(argv[0]);
      return 0;
    }
  }

  printf(">>>>>>>>>>>>>>> Test START <<<<<<<<<<<<<<<<<<<<<<\n");
  printf("#Device: %s\n", pDeviceName);
  printf("#Resolution: %dx%d\n", u32Width, u32Height);
  printf("#Frame Count to save: %d\n", u32FrameCnt);
  printf("#Output Path: %s\n", pOutPath);
  printf("#Aiq xml dirpath: %s\n\n", pIqfilesPath);

  if (pIqfilesPath) {
#ifdef RKAIQ
    rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
    RK_BOOL fec_enable = RK_FALSE;
    int fps = 30;
    SAMPLE_COMM_ISP_Init(hdr_mode, fec_enable, pIqfilesPath);
    SAMPLE_COMM_ISP_Run();
    SAMPLE_COMM_ISP_SetFrameRate(fps);
#endif
  }

  RK_MPI_SYS_Init();
  VI_CHN_ATTR_S vi_chn_attr;
  vi_chn_attr.pcVideoNode = pDeviceName;
  vi_chn_attr.u32BufCnt = 4;
  vi_chn_attr.u32Width = u32Width;
  vi_chn_attr.u32Height = u32Height;
  vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;
  vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
  ret = RK_MPI_VI_SetChnAttr(0, 0, &vi_chn_attr);
  ret |= RK_MPI_VI_EnableChn(0, 0);
  if (ret) {
    printf("Create VI[0] failed! ret=%d\n", ret);
    return -1;
  }

  printf("%s initial finish\n", __func__);
  signal(SIGINT, sigterm_handler);

  pthread_t read_thread;
  OutputArgs outArgs = {pOutPath, u32FrameCnt};
  pthread_create(&read_thread, NULL, GetMediaBuffer, &outArgs);
  ret = RK_MPI_VI_StartStream(0, 0);
  if (ret) {
    printf("Start VI[0] failed! ret=%d\n", ret);
    return -1;
  }

  while (!quit) {
    usleep(100);
  }

#ifdef RKAIQ
  if (pIqfilesPath)
    SAMPLE_COMM_ISP_Stop(); // isp aiq stop before vi streamoff
#endif

  printf("%s exit!\n", __func__);
  printf(">>>>>>>>>>>>>>> Test END <<<<<<<<<<<<<<<<<<<<<<\n");
  RK_MPI_VI_DisableChn(0, 0);

  return 0;
}
