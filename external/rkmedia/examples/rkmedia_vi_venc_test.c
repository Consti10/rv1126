// Copyright 2020 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

#include "common/sample_common.h"
#include "rkmedia_api.h"
#include "rkmedia_venc.h"

#include <sys/time.h>
//socket
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static bool quit = false;
static void sigterm_handler(int sig) {
    fprintf(stderr, "signal %d\n", sig);
    quit = true;
}

static uint64_t lastTimeStamp=0;
static RK_U32 m_framerate;
static int sockfd;
static struct sockaddr_in address={};
static const int UDP_PACKET_MAX_SIZE=65507;
static const int M_PORT=5600;
static char* M_IP="192.168.0.11";

static uint64_t getTimeMs(){
    struct timeval time;
    gettimeofday(&time, NULL);
    uint64_t millis = (time.tv_sec * ((uint64_t)1000)) + ((uint64_t)time.tv_usec / ((uint64_t)1000));
    return millis;
}

static void createSocket(){
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0) {
        printf("Cannot create socket\n");
    }
    //Create the address
    address.sin_family = AF_INET;
    address.sin_port = htons(M_PORT);
    inet_pton(AF_INET,M_IP, &address.sin_addr);
}

static void __attribute__((unused)) mySendTo(void* data,int data_length){
    if(data_length>UDP_PACKET_MAX_SIZE){
        printf("Data size exceeds UDP packet size\n");
        return;
    }
    int result= sendto(sockfd, data, data_length, 0, (struct sockaddr *) &(address),
                       sizeof(struct sockaddr_in));
    if(result<0){
        printf("Cannot send data\n");
    }else{
        printf("Succesfully sent data\n");
    }
}


void video_packet_cb(MEDIA_BUFFER mb) {
    const char *nalu_type = "Unknow";
    switch (RK_MPI_MB_GetFlag(mb)) {
        case VENC_NALU_IDRSLICE:
            nalu_type = "IDR Slice";
            break;
        case VENC_NALU_PSLICE:
            nalu_type = "P Slice";
            break;
        default:
            break;
    }
    printf("Get Video Encoded packet(%s):ptr:%p, fd:%d, size:%zu, mode:%d\n",
           nalu_type, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb),
           RK_MPI_MB_GetSize(mb), RK_MPI_MB_GetModeID(mb));
    // send out data via udp (raw)
    mySendTo( RK_MPI_MB_GetPtr(mb),RK_MPI_MB_GetSize(mb));

    //Consti10: print time to check for fps
    uint64_t ts=getTimeMs();
    uint64_t delta=ts-lastTimeStamp;
    lastTimeStamp=ts;
    printf("Current time %" PRIu64 "(ms), delta %" PRIu64 "(ms)\n",ts,delta);
    //COnsti10: add end
    RK_MPI_MB_ReleaseBuffer(mb);
}

static RK_CHAR optstr[] = "?:a::h:w:e:d:f:";
static const struct option long_options[] = {
        {"aiq", optional_argument, NULL, 'a'},
        {"height", required_argument, NULL, 'h'},
        {"width", required_argument, NULL, 'w'},
        {"encode", required_argument, NULL, 'e'},
        {"framerate", required_argument, NULL, 'f'},
        {"device_name", required_argument, NULL, 'd'},
        {"ip_address",required_argument,NULL, 'i'},
        {NULL, 0, NULL, 0},
};

static void print_usage(const RK_CHAR *name) {
    printf("usage example:\n");
#ifdef RKAIQ
    printf("\t%s "
         "[-a | --aiq /oem/etc/iqfiles/] "
         "[-h | --height 1080] "
         "[-w | --width 1920] "
         "[-e | --encode 0] "
         "[-f | --framerate 30]"
         "[-d | --device_name rkispp_scale0]\n",
         name);
  printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a "
         "/oem/etc/iqfiles/, "
         "set dirpath empty to using path by default, without this option aiq "
         "should run in other application\n");
#else
    printf("\t%s "
           "[-h | --height 1080] "
           "[-w | --width 1920] "
           "[-e | --encode 0] "
           "[-d | --device_name rkispp_scale0]\n",
           name);
#endif
    printf("\t-h | --height: VI height, Default:1080\n");
    printf("\t-w | --width: VI width, Default:1920\n");
    printf("\t-e | --encode: encode type, Default:0, set 0 to use H264, set 1 to "
           "use H265\n");
    printf("\t-d | --device_name set pcDeviceName, Default:rkispp_scale0, "
           "Option:[rkispp_scale0, rkispp_scale1, rkispp_scale2]\n");
}

int main(int argc, char *argv[]) {
    RK_U32 u32Width = 1920;
    RK_U32 u32Height = 1080;
    RK_U32 encode_type = 0;
    //RK_U32  __attribute__((unused)) m_framerate = 30;
    m_framerate=30;
    RK_CHAR *device_name = "rkispp_scale0";
    RK_CHAR *iq_file_dir = NULL;
    int ret = 0;
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
                u32Height = atoi(optarg);
                break;
            case 'w':
                u32Width = atoi(optarg);
                break;
            case 'e':
                encode_type = atoi(optarg);
                break;
            case 'd':
                device_name = optarg;
                break;
            case 'f':
                //printf("framerate %d\n",atoi(optarg));
                m_framerate=atoi(optarg);
                break;
            case 'i':
                M_IP=optarg;
                break;
            case '?':
            default:
                print_usage(argv[0]);
                return 0;
        }
    }
    createSocket();

    printf("device_name: %s\n", device_name);
    printf("#height: %d\n", u32Height);
    printf("#width: %d\n", u32Width);
    printf("#framerate: %d\n", m_framerate);
    printf("#encode_type: %d\n", encode_type);
    printf("#ip_address: %s\n",M_IP);

    if (iq_file_dir) {
#ifdef RKAIQ
        printf("#Aiq xml dirpath: %s\n", iq_file_dir);
    rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
    RK_BOOL fec_enable = RK_FALSE;
    int fps = m_framerate;
    SAMPLE_COMM_ISP_Init(hdr_mode, fec_enable, iq_file_dir);
    SAMPLE_COMM_ISP_Run();
    SAMPLE_COMM_ISP_SetFrameRate(fps);
#endif
    }

    RK_MPI_SYS_Init();
    VI_CHN_ATTR_S vi_chn_attr;
    vi_chn_attr.pcVideoNode = "rkispp_m_bypass";
    vi_chn_attr.u32BufCnt = 4; // should be >= 4
    vi_chn_attr.u32Width = u32Width;
    vi_chn_attr.u32Height = u32Height;
    vi_chn_attr.enPixFmt = IMAGE_TYPE_FBC0;
    vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
    ret = RK_MPI_VI_SetChnAttr(0, 0, &vi_chn_attr);
    ret |= RK_MPI_VI_EnableChn(0, 0);
    if (ret) {
        printf("ERROR: create VI[0] error! ret=%d\n", ret);
        return 0;
    }

    VENC_CHN_ATTR_S venc_chn_attr;
    switch (encode_type) {
        case 0:
            venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264;
            venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
            break;
        case 1:
            venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H265;
            venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
            break;
        default:
            venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264;
            venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
            break;
    }
    venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_FBC0;
    venc_chn_attr.stVencAttr.u32PicWidth = u32Width;
    venc_chn_attr.stVencAttr.u32PicHeight = u32Height;
    venc_chn_attr.stVencAttr.u32VirWidth = u32Width;
    venc_chn_attr.stVencAttr.u32VirHeight = u32Height;
    venc_chn_attr.stVencAttr.u32Profile = 77;
    venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = m_framerate;
    venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = 1920 * 1080 * 30 / 14;
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 0;
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = m_framerate;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 0;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = m_framerate;
    ret = RK_MPI_VENC_CreateChn(0, &venc_chn_attr);
    if (ret) {
        printf("ERROR: create VENC[0] error! ret=%d\n", ret);
        return 0;
    }

    MPP_CHN_S stEncChn;
    stEncChn.enModId = RK_ID_VENC;
    stEncChn.s32DevId = 0;
    stEncChn.s32ChnId = 0;
    ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, video_packet_cb);
    if (ret) {
        printf("ERROR: register output callback for VENC[0] error! ret=%d\n", ret);
        return 0;
    }

    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;
    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = 0;
    ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret) {
        printf("ERROR: Bind VI[0] and VENC[0] error! ret=%d\n", ret);
        return 0;
    }

    printf("%s initial finish\n", __func__);
    signal(SIGINT, sigterm_handler);

    while (!quit) {
        usleep(100);
    }

    if (iq_file_dir) {
#ifdef RKAIQ
        SAMPLE_COMM_ISP_Stop(); // isp aiq stop before vi streamoff
#endif
    }

    printf("%s exit!\n", __func__);
    // unbind first
    ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (ret) {
        printf("ERROR: UnBind VI[0] and VENC[0] error! ret=%d\n", ret);
        return 0;
    }
    // destroy venc before vi
    ret = RK_MPI_VENC_DestroyChn(0);
    if (ret) {
        printf("ERROR: Destroy VENC[0] error! ret=%d\n", ret);
        return 0;
    }
    // destroy vi
    ret = RK_MPI_VI_DisableChn(0, 0);
    if (ret) {
        printf("ERROR: Destroy VI[0] error! ret=%d\n", ret);
        return 0;
    }

    return 0;
}