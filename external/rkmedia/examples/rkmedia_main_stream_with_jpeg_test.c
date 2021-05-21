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
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common/sample_common.h"
#include "rkmedia_api.h"

static bool quit = false;
static char *g_save_path = "/tmp/";
static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  quit = true;
}

static void *GetMediaBuffer(void *arg) {
  printf("#Start %s thread, arg:%p\n", __func__, arg);
  FILE *save_file = fopen("/userdata/output.h265", "w");
  if (!save_file)
    printf("ERROR: Open /userdata/output.h265 failed!\n");

  MEDIA_BUFFER mb = NULL;
  while (!quit) {
    mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VENC, 0, -1);
    if (!mb) {
      printf("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
      break;
    }

    //    printf("Get packet:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
    //           "timestamp:%lld\n",
    //           RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb),
    //           RK_MPI_MB_GetSize(mb),
    //           RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb),
    //           RK_MPI_MB_GetTimestamp(mb));

    if (save_file)
      fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), save_file);
    RK_MPI_MB_ReleaseBuffer(mb);
  }

  if (save_file)
    fclose(save_file);

  return NULL;
}

void take_pictures_cb(MEDIA_BUFFER mb) {
  static RK_U32 jpeg_id = 0;
  printf("Get JPEG packet[%d]:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
         "timestamp:%lld\n",
         jpeg_id, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb),
         RK_MPI_MB_GetSize(mb), RK_MPI_MB_GetModeID(mb),
         RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));

  char jpeg_path[64];
  sprintf(jpeg_path, "%stest_jpeg%d.jpeg", g_save_path, jpeg_id);
  FILE *file = fopen(jpeg_path, "w");
  if (file) {
    fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), file);
    fclose(file);
  }

  RK_MPI_MB_ReleaseBuffer(mb);
  jpeg_id++;
}

static RK_CHAR optstr[] = "?:a::h:o:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"help", no_argument, NULL, 'h'},
    {"output_path", required_argument, NULL, 'o'},
    {NULL, 0, NULL, 0},
};

static void print_usage(const RK_CHAR *name) {
  printf("usage example:\n");
#ifdef RKAIQ
  printf("\t%s [-a | --aiq=/oem/etc/iqfiles/] [-w | --width 1920] "
         "[-h | --help] "
         "[-o | --output_path /tmp/] "
         "\n",
         name);
  printf("\t-a | --aiq: enable aiq with dirpath provided, "
         "eg:-a=/oem/etc/iqfiles/, "
         "set dirpath emtpty to using path by default\n");
#else
  printf("\t%s [-h | --help] "
         "[-o | --output_path /tmp/] \n",
         name);
#endif
  printf("\t-o | --output_dirpath: output path, Default:/tmp/\n");
  printf("\t-h | --help: show help info.\n");
}

int main(int argc, char *argv[]) {
  RK_S32 ret;
  RK_U32 u32SrcWidth = 2688;
  RK_U32 u32SrcHeight = 1520;
  RK_U32 u32DstWidth = 1920;
  RK_U32 u32DstHeight = 1080;
  IMAGE_TYPE_E enPixFmt = IMAGE_TYPE_FBC0;
  const RK_CHAR *pcVideoNode = "rkispp_m_bypass";

#ifdef RKAIQ
  rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
  RK_BOOL fec_enable = RK_FALSE;
  int fps = 30;
#endif

  char *iq_file_dir = NULL;
  int c;
  while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
    const char *tmp_optarg = optarg;
    switch (c) {
    case 'a':
      if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
        tmp_optarg = argv[optind++];
      }
      if (tmp_optarg) {
        iq_file_dir = (char *)tmp_optarg;
      } else {
        iq_file_dir = "/oem/etc/iqfiles";
      }
      break;
    case 'h':
      print_usage(argv[0]);
      return 0;
    case 'o':
      g_save_path = optarg ? optarg : NULL;
      break;
    case '?':
    default:
      print_usage(argv[0]);
      return 0;
    }
  }

if (iq_file_dir) {
#ifdef RKAIQ
    SAMPLE_COMM_ISP_Init(hdr_mode, fec_enable, iq_file_dir);
    SAMPLE_COMM_ISP_Run();
    SAMPLE_COMM_ISP_SetFrameRate(fps);
#endif
  }

  ret = RK_MPI_SYS_Init();
  if (ret) {
    printf("Sys Init failed! ret=%d\n", ret);
    return -1;
  }

  VI_CHN_ATTR_S vi_chn_attr;
  vi_chn_attr.pcVideoNode = pcVideoNode;
  vi_chn_attr.u32BufCnt = 3;
  vi_chn_attr.u32Width = u32SrcWidth;
  vi_chn_attr.u32Height = u32SrcHeight;
  vi_chn_attr.enPixFmt = enPixFmt;
  vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
  ret = RK_MPI_VI_SetChnAttr(0, 0, &vi_chn_attr);
  ret |= RK_MPI_VI_EnableChn(0, 0);
  if (ret) {
    printf("Create Vi failed! ret=%d\n", ret);
    return -1;
  }

  // Create H265 for Main Stream.
  VENC_CHN_ATTR_S venc_chn_attr;
  venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H265;
  venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_FBC0;
  venc_chn_attr.stVencAttr.u32PicWidth = u32SrcWidth;
  venc_chn_attr.stVencAttr.u32PicHeight = u32SrcHeight;
  venc_chn_attr.stVencAttr.u32VirWidth = u32SrcWidth;
  venc_chn_attr.stVencAttr.u32VirHeight = u32SrcHeight;
  venc_chn_attr.stVencAttr.u32Profile = 77;
  venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
  venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = 30;
  venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate =
      u32SrcWidth * u32SrcHeight * 30 / 14;
  venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 0;
  venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = 30;
  venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 0;
  venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = 30;
  ret = RK_MPI_VENC_CreateChn(0, &venc_chn_attr);
  if (ret) {
    printf("Create Venc(H265) failed! ret=%d\n", ret);
    return -1;
  }

  // Create JPEG for take pictures.
  memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
  venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_JPEG;
  venc_chn_attr.stVencAttr.imageType = enPixFmt;
  venc_chn_attr.stVencAttr.u32PicWidth = u32SrcWidth;
  venc_chn_attr.stVencAttr.u32PicHeight = u32SrcHeight;
  venc_chn_attr.stVencAttr.u32VirWidth = u32SrcWidth;
  venc_chn_attr.stVencAttr.u32VirHeight = u32SrcHeight;
  venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomWidth = u32DstWidth;
  venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomHeight = u32DstHeight;
  venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirWidth = u32DstWidth;
  venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirHeight = u32DstHeight;
  ret = RK_MPI_VENC_CreateChn(1, &venc_chn_attr);
  if (ret) {
    printf("Create Venc(JPEG) failed! ret=%d\n", ret);
    return -1;
  }

  // Put the jpeg encoder to sleep.
  VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = 0;
  RK_MPI_VENC_StartRecvFrame(1, &stRecvParam);

  MPP_CHN_S stEncChn;
  stEncChn.enModId = RK_ID_VENC;
  stEncChn.s32ChnId = 1;
  ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, take_pictures_cb);
  if (ret) {
    printf("Register Output callback failed! ret=%d\n", ret);
    return -1;
  }

  // Get MediaBuffer frome VENC::H265, and save packets to file.
  pthread_t read_thread;
  pthread_create(&read_thread, NULL, GetMediaBuffer, NULL);

  MPP_CHN_S stSrcChn;
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32ChnId = 0;
  MPP_CHN_S stDestChn;
  stDestChn.enModId = RK_ID_VENC;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("Bind VI[0] to VENC[0]::H265 failed! ret=%d\n", ret);
    return -1;
  }

  stDestChn.s32ChnId = 1;
  ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("Bind VI[0] to VENC[1]::JPEG failed! ret=%d\n", ret);
    return -1;
  }

  printf("%s initial finish\n", __func__);
  signal(SIGINT, sigterm_handler);

  char cmd[64];
  printf("#Usage: input 'quit' to exit programe!\n"
         "peress any other key to capture one picture to file\n");
  while (!quit) {
    fgets(cmd, sizeof(cmd), stdin);
    printf("#Input cmd:%s\n", cmd);
    if (strstr(cmd, "quit")) {
      printf("#Get 'quit' cmd!\n");
      break;
    }
    // Activate the JPEG encoder and receive one frame.
    stRecvParam.s32RecvPicNum = 1;
    ret = RK_MPI_VENC_StartRecvFrame(1, &stRecvParam);
    if (ret) {
      printf("RK_MPI_VENC_StartRecvFrame failed!\n");
      break;
    }
    usleep(30000); // sleep 30 ms.
  }

  printf("%s exit!\n", __func__);
#ifdef RKAIQ
  SAMPLE_COMM_ISP_Stop(); // isp aiq stop before vi streamoff
#endif
  stDestChn.s32ChnId = 0;
  RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
  stDestChn.s32ChnId = 1;
  RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
  RK_MPI_VI_DisableChn(0, 0);
  RK_MPI_VENC_DestroyChn(0);
  RK_MPI_VENC_DestroyChn(1);

  return 0;
}
