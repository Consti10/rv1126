// Copyright 2020 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "common/sample_double_cam_isp.h"
#include "rkmedia_api.h"
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
static bool quit = false;
static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  quit = true;
}
static void *GetBuffer(void *arg) {
  MEDIA_BUFFER mb = NULL;
  int *id = (int *)arg;
  while (!quit) {
    mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_RGA, *id, -1);
    if (!mb) {
      printf("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
      break;
    }
    RK_MPI_MB_ReleaseBuffer(mb);
  }
  return NULL;
}
void usage(char *s) {
  printf("Usage: %s\n", s);
  printf("Usage: %s -rgb\n", s);
  printf("Usage: %s -ir\n", s);
  printf("Usage: %s -rgb [/etc/iqfiles]\n", s);
  exit(0);
}
int main(int argc, char *argv[]) {
  int ret = 0;
  int video_width = 1920;
  int video_hegith = 1080;
  int disp_width = 720;
  int disp_height = 1280;
  int id = 0;
  char *iq_dir = NULL;
  if (argc >= 2) {
    if (strstr(argv[1], "rgb"))
      id = 0;
    else if (strstr(argv[1], "ir"))
      id = 1;
    else
      usage(argv[0]);
  }
  if (argc >= 3) {
    iq_dir = argv[2];
    if (access(iq_dir, R_OK))
      usage(argv[0]);
  }

  rk_aiq_sys_ctx_t *ctx0 =
      aiq_double_cam_init(0, RK_AIQ_WORKING_MODE_NORMAL, iq_dir);
  if (!ctx0)
    return -1;
  rk_aiq_sys_ctx_t *ctx1 =
      aiq_double_cam_init(1, RK_AIQ_WORKING_MODE_NORMAL, iq_dir);
  if (!ctx1)
    return -1;

  RK_MPI_SYS_Init();
  VI_CHN_ATTR_S vi_chn_attr;
  memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
  vi_chn_attr.pcVideoNode = "rkispp_scale1";
  vi_chn_attr.u32BufCnt = 3;
  vi_chn_attr.u32Width = video_width;
  vi_chn_attr.u32Height = video_hegith;
  vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;
  vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
  ret = RK_MPI_VI_SetChnAttr(0, 0, &vi_chn_attr);
  ret |= RK_MPI_VI_EnableChn(0, 0);
  if (ret) {
    printf("Create vi[0] failed! ret=%d\n", ret);
    return -1;
  }
  memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
  vi_chn_attr.pcVideoNode = "rkispp_scale1";
  vi_chn_attr.u32BufCnt = 3;
  vi_chn_attr.u32Width = video_width;
  vi_chn_attr.u32Height = video_hegith;
  vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;
  vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
  ret = RK_MPI_VI_SetChnAttr(1, 1, &vi_chn_attr);
  ret |= RK_MPI_VI_EnableChn(1, 1);
  if (ret) {
    printf("Create vi[1] failed! ret=%d\n", ret);
    return -1;
  }
  RGA_ATTR_S stRgaAttr;
  memset(&stRgaAttr, 0, sizeof(stRgaAttr));
  stRgaAttr.bEnBufPool = RK_TRUE;
  stRgaAttr.u16BufPoolCnt = 12;
  stRgaAttr.u16Rotaion = 90;
  stRgaAttr.stImgIn.u32X = 0;
  stRgaAttr.stImgIn.u32Y = 0;
  stRgaAttr.stImgIn.imgType = IMAGE_TYPE_NV12;
  stRgaAttr.stImgIn.u32Width = video_width;
  stRgaAttr.stImgIn.u32Height = video_hegith;
  stRgaAttr.stImgIn.u32HorStride = video_width;
  stRgaAttr.stImgIn.u32VirStride = video_hegith;
  stRgaAttr.stImgOut.u32X = 0;
  stRgaAttr.stImgOut.u32Y = 0;
  stRgaAttr.stImgOut.imgType = IMAGE_TYPE_NV12;
  stRgaAttr.stImgOut.u32Width = disp_width;
  stRgaAttr.stImgOut.u32Height = disp_height;
  stRgaAttr.stImgOut.u32HorStride = disp_width;
  stRgaAttr.stImgOut.u32VirStride = disp_height;
  ret = RK_MPI_RGA_CreateChn(0, &stRgaAttr);
  if (ret) {
    printf("Create rga[0] falied! ret=%d\n", ret);
    return -1;
  }
  memset(&stRgaAttr, 0, sizeof(stRgaAttr));
  stRgaAttr.bEnBufPool = RK_TRUE;
  stRgaAttr.u16BufPoolCnt = 12;
  stRgaAttr.u16Rotaion = 270;
  stRgaAttr.stImgIn.u32X = 0;
  stRgaAttr.stImgIn.u32Y = 0;
  stRgaAttr.stImgIn.imgType = IMAGE_TYPE_NV12;
  stRgaAttr.stImgIn.u32Width = video_width;
  stRgaAttr.stImgIn.u32Height = video_hegith;
  stRgaAttr.stImgIn.u32HorStride = video_width;
  stRgaAttr.stImgIn.u32VirStride = video_hegith;
  stRgaAttr.stImgOut.u32X = 0;
  stRgaAttr.stImgOut.u32Y = 0;
  stRgaAttr.stImgOut.imgType = IMAGE_TYPE_NV12;
  stRgaAttr.stImgOut.u32Width = disp_width;
  stRgaAttr.stImgOut.u32Height = disp_height;
  stRgaAttr.stImgOut.u32HorStride = disp_width;
  stRgaAttr.stImgOut.u32VirStride = disp_height;
  ret = RK_MPI_RGA_CreateChn(1, &stRgaAttr);
  if (ret) {
    printf("Create rga[1] falied! ret=%d\n", ret);
    return -1;
  }
  VO_CHN_ATTR_S stVoAttr = {0};
  stVoAttr.emPlaneType = VO_PLANE_OVERLAY;
  stVoAttr.enImgType = IMAGE_TYPE_NV12;
  stVoAttr.u16Zpos = 0;
  stVoAttr.stImgRect.s32X = 0;
  stVoAttr.stImgRect.s32Y = 0;
  stVoAttr.stImgRect.u32Width = disp_width;
  stVoAttr.stImgRect.u32Height = disp_height;
  stVoAttr.stDispRect.s32X = 0;
  stVoAttr.stDispRect.s32Y = 0;
  stVoAttr.stDispRect.u32Width = disp_width;
  stVoAttr.stDispRect.u32Height = disp_height;
  ret = RK_MPI_VO_CreateChn(0, &stVoAttr);
  if (ret) {
    printf("Create vo[0] failed! ret=%d\n", ret);
    return -1;
  }
  MPP_CHN_S stSrcChn = {0};
  MPP_CHN_S stDestChn = {0};
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32ChnId = 0;
  stDestChn.enModId = RK_ID_RGA;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("Bind vi[0] to rga[0] failed! ret=%d\n", ret);
    return -1;
  }
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32ChnId = 1;
  stDestChn.enModId = RK_ID_RGA;
  stDestChn.s32ChnId = 1;
  ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("Bind vi[1] to rga[1] failed! ret=%d\n", ret);
    return -1;
  }
  stSrcChn.enModId = RK_ID_RGA;
  stSrcChn.s32ChnId = id;
  stDestChn.enModId = RK_ID_VO;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("Bind vi[%d] to rga[0] failed! ret=%d\n", id, ret);
    return -1;
  }
  pthread_t th;
  int idx = (id ? 0 : 1);
  if (pthread_create(&th, NULL, GetBuffer, &idx)) {
    printf("create GetBuffer thread failed!\n");
    return -1;
  }
  printf("%s initial finish\n", __func__);
  signal(SIGINT, sigterm_handler);
  while (!quit) {
    usleep(100);
  }
  printf("%s exit!\n", __func__);
  pthread_join(th, NULL);
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32ChnId = 0;
  stDestChn.enModId = RK_ID_RGA;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("UnBind vi[0] to rga[0] failed! ret=%d\n", ret);
    return -1;
  }
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32ChnId = 1;
  stDestChn.enModId = RK_ID_RGA;
  stDestChn.s32ChnId = 1;
  ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("UnBind vi[1] to rga[1] failed! ret=%d\n", ret);
    return -1;
  }
  stSrcChn.enModId = RK_ID_RGA;
  stSrcChn.s32ChnId = id;
  stDestChn.enModId = RK_ID_VO;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("UnBind rga[%d] to vo[0] failed! ret=%d\n", id, ret);
    return -1;
  }
  RK_MPI_VO_DestroyChn(0);
  RK_MPI_VI_DisableChn(0, 0);
  RK_MPI_VI_DisableChn(1, 1);
  RK_MPI_RGA_DestroyChn(0);
  RK_MPI_RGA_DestroyChn(1);
  aiq_double_cam_exit(ctx0);
  aiq_double_cam_exit(ctx1);
  return 0;
}
