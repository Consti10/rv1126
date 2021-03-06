/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "uvc_encode.h"
#include "uvc_video.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "uvc_log.h"
#include "mpi_enc.h"
#include "yuv.h"

#if RK_MPP_RANGE_DEBUG_ON
static char *strtrimr(char *pstr)
{
    int i;
    i = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i >= 0))
        pstr[i--] = '\0';
    return pstr;
}

static char *strtriml(char *pstr)
{
    int i = 0,j;
    j = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i <= j))
        i++;
    if (0<i)
        strcpy(pstr, &pstr[i]);
    return pstr;
}

static char *strtrim(char *pstr)
{
    char *p;
    p = strtrimr(pstr);
    return strtriml(p);
}

static char *strrmlb(char *pstr)
{
    int i;
    i = strlen(pstr) - 1;
    while ((pstr[i] == '\n') && (i >= 0))
        pstr[i--] = '\0';
    return pstr;
}
#endif

int uvc_encode_init(struct uvc_encode *e, int width, int height, int fcc, int h265)
{
    LOG_INFO("%s: width = %d, height = %d, fcc = %d, h265= %d\n", __func__, width, height,fcc,h265);
    memset(e, 0, sizeof(*e));
    e->video_id = -1;
    e->width = -1;
    e->height = -1;
    e->width = width;
    e->height = height;
    e->fcc = fcc;
    e->loss_frm = 0;
    mpi_enc_cmd_config(&e->mpi_cmd, width, height, fcc, h265);
    //mpi_enc_cmd_config_mjpg(&e->mpi_cmd, width, height);
    if(fcc == V4L2_PIX_FMT_YUYV)
        return 0;
    if (mpi_enc_test_init(&e->mpi_cmd, &e->mpi_data) != MPP_OK)
        return -1;

    return 0;
}

void uvc_encode_exit(struct uvc_encode *e)
{
    if(e->fcc != V4L2_PIX_FMT_YUYV)
        mpi_enc_test_deinit(&e->mpi_data);
    e->video_id = -1;
    e->width = -1;
    e->height = -1;
    e->fcc = -1;
}

bool uvc_encode_process(struct uvc_encode *e, void *virt, struct MPP_ENC_INFO *info)
{
    int ret = 0;
    unsigned int fcc;
    int width, height;
    int jpeg_quant;
    void *hnd = NULL;

#if RK_MPP_ENC_TEST_NATIVE
    fcc = TEST_ENC_TPYE;
#else
#ifndef RK_MPP_USE_UVC_VIDEO_BUFFER
    if (!uvc_get_user_run_state(e->video_id) || !uvc_buffer_write_enable(e->video_id))
    {
        return false;
    }
#endif
    uvc_get_user_resolution(&width, &height, e->video_id);
    fcc = uvc_get_user_fcc(e->video_id);
#endif

#if RK_MPP_RANGE_DEBUG_ON
    if(fcc != V4L2_PIX_FMT_YUYV)
    {
        if (!access(RK_MPP_RANGE_DEBUG_IN_CHECK, 0))
        {
            if (!e->mpi_data->fp_range_file)
            {
                e->mpi_data->fp_range_path = fopen(RK_MPP_RANGE_DEBUG_IN_CHECK, "r+b");
                e->mpi_data->range_path = (char *)calloc(1, RANGE_PATH_MAX_LEN);
                fgets(e->mpi_data->range_path, RANGE_PATH_MAX_LEN, e->mpi_data->fp_range_path);
                e->mpi_data->range_path = strtrim(e->mpi_data->range_path);
                e->mpi_data->range_path = strrmlb(e->mpi_data->range_path);
                e->mpi_data->fp_range_file = fopen(e->mpi_data->range_path, "r+b");
                fclose(e->mpi_data->fp_range_path);
                e->mpi_data->fp_range_path = NULL;
                if (!e->mpi_data->fp_range_file)
                {
                    LOG_ERROR("error:no such fp_range file:%s exit\n", e->mpi_data->range_path);
                }
                else
                {
                    LOG_INFO("open fp_range file:%s ok, size=%d\n", e->mpi_data->range_path, info->size);
                    while (ret != info->size)
                    {
                        ret += fread(virt, 1, info->size - ret, e->mpi_data->fp_range_file);
                        if (feof(e->mpi_data->fp_range_file))
                            rewind(e->mpi_data->fp_range_file);
                    }
                    if (strstr(e->mpi_data->range_path, "full"))
                    {
                        ret = mpp_enc_cfg_set_s32(e->mpi_data->cfg, "prep:range", MPP_FRAME_RANGE_JPEG);
                        LOG_INFO("change to full range\n");
                    }
                    else
                    {
                        ret = mpp_enc_cfg_set_s32(e->mpi_data->cfg, "prep:range", MPP_FRAME_RANGE_UNSPECIFIED);
                        LOG_INFO("change to limit range\n");
                    }
                    if (ret)
                        LOG_ERROR("mpi control enc set prep:range failed ret %d\n", ret);
                    ret = e->mpi_data->mpi->control(e->mpi_data->ctx, MPP_ENC_SET_CFG, e->mpi_data->cfg);
                    if (ret)
                        LOG_ERROR("mpi control enc set cfg failed ret %d\n", ret);
                }
                free(e->mpi_data->range_path);
            }
            else
            {
                while (ret != info->size)
                {
                    ret += fread(virt, 1, info->size - ret, e->mpi_data->fp_range_file);
                    if (feof(e->mpi_data->fp_range_file))
                        rewind(e->mpi_data->fp_range_file);
                }
            }
        }
        else if (e->mpi_data->fp_range_file)
        {
            fclose(e->mpi_data->fp_range_file);
            e->mpi_data->fp_range_file = NULL;
            LOG_INFO("debug fp_range file close\n");
        }
    }
#endif

    if(fcc != V4L2_PIX_FMT_YUYV)
    {
        if (e->mpi_data->fp_input)
        {
            fwrite(virt, 1, info->size, e->mpi_data->fp_input);
#if RK_MPP_DYNAMIC_DEBUG_ON
            if (access(RK_MPP_DYNAMIC_DEBUG_IN_CHECK, 0))
            {
                fclose(e->mpi_data->fp_input);
                e->mpi_data->fp_input = NULL;
                LOG_INFO("debug in file close\n");
            }
        }
        else if (!access(RK_MPP_DYNAMIC_DEBUG_IN_CHECK, 0))
        {
            e->mpi_data->fp_input = fopen(RK_MPP_DEBUG_IN_FILE, "w+b");
            if (e->mpi_data->fp_input)
            {
                fwrite(virt, 1, info->size, e->mpi_data->fp_input);
                LOG_INFO("warnning:debug in file open, open it will lower the fps\n");
            }
#endif
        }
    }
    switch (fcc)
    {
    case V4L2_PIX_FMT_YUYV:
        if (virt) {
#ifdef RK_MPP_USE_UVC_VIDEO_BUFFER
           int try_count = 25;
           bool get_ok = false;
           while (try_count--)
           {
               if ((!uvc_get_user_run_state(e->video_id) || !uvc_buffer_write_enable(e->video_id)))
               {
                   usleep(MPP_FRC_WAIT_TIME_US);
               }
               else
               {
                   get_ok = true;
                   break;
               }
           }

           if (get_ok) {
               uvc_user_lock();
               struct uvc_buffer *buffer = uvc_buffer_write_get_nolock(e->video_id);
               if (!buffer || buffer->abandon)
               {
                   LOG_ERROR("uvc_buffer_write_get failed:%p\n", buffer);
                   uvc_user_unlock();
                   return true;
               }
               buffer->pts = info->pts;
#if UVC_DYNAMIC_DEBUG_USE_TIME
               if (!access(UVC_DYNAMIC_DEBUG_USE_TIME_CHECK, 0))
               {
                   int32_t use_time_us, now_time_us;
                   struct timespec now_tm = {0, 0};
                   clock_gettime(CLOCK_MONOTONIC, &now_tm);
                   now_time_us = now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us
                   use_time_us = now_time_us - buffer->pts;
#if USE_RK_AISERVER
                   LOG_INFO("isp->aiserver->ipc->mpp_get_buf latency time:%d us, %d ms\n", use_time_us, use_time_us / 1000);
#endif
#if USE_ROCKIT
                   LOG_INFO("isp->rockit->uvc->mpp_get_buf latency time:%d us, %d ms\n", use_time_us, use_time_us / 1000);
#endif
#if USE_RKMEDIA
                   LOG_INFO("isp->rkmedia->uvc->mpp_get_buf latency time:%d us, %d ms\n", use_time_us, use_time_us / 1000);
#endif
               }
#endif

               NV12_to_YUYV(width, height, virt, buffer->buffer);
               uvc_buffer_read_set_nolock(e->video_id, buffer);
               uvc_user_unlock();
           } else {
               e->loss_frm ++;
#if RK_MPP_DYNAMIC_DEBUG_ON
               if (access(RK_MPP_CLOSE_FRM_LOSS_DEBUG_CHECK, 0) && (e->loss_frm % 100 == 1)) //no exist and output log
#endif
                   LOG_ERROR("not get write buff,read too slow %lld.\"touch %s \" can close debug.\n",
                              e->loss_frm,
                              RK_MPP_CLOSE_FRM_LOSS_DEBUG_CHECK);
           }
#else
               uvc_buffer_write(0, NULL, 0, virt, width * height * 2, fcc, e->video_id);
#endif
        }
        break;
    case V4L2_PIX_FMT_MJPEG:
    case V4L2_PIX_FMT_H264:
    case V4L2_PIX_FMT_H265:
        if (info->fd >= 0 && mpi_enc_test_run(&e->mpi_data, info) == MPP_OK)
        {
#ifdef ENABLE_BUFFER_TIME_DEBUG
            struct timeval buffer_time;
            gettimeofday(&buffer_time, NULL);
            LOG_ERROR("UVC ENCODE BUFFER TIME END:%d.%d (s)", buffer_time.tv_sec, buffer_time.tv_usec);
#endif

#ifndef RK_MPP_USE_UVC_VIDEO_BUFFER
            uvc_buffer_write(0, e->extra_data, e->extra_size,
                             e->mpi_data->enc_data, e->mpi_data->enc_len, fcc, e->video_id);
#endif
        }
        break;
    default:
        LOG_ERROR("%s: not support fcc: %u\n", __func__, fcc);
        break;
    }

    return true;
}
