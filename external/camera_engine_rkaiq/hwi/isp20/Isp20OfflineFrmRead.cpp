/*
 *  Copyright (c) 2019 Rockchip Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "Isp20PollThread.h"
#include "Isp20OfflineFrmRead.h"

namespace RkCam {

static void offlineFrmRdSleep(uint32_t msec)
{
    if (msec)
    {
        struct timeval timeout;
        timeout.tv_sec  = msec / 1000;
        timeout.tv_usec = (msec % 1000) * 1000;
        select(0, NULL, NULL, NULL, &timeout);
    }
}

XCamReturn
OfflineFrmRdThread::dequeue_all_buffer () {
    struct v4l2_buffer v4l2_buf;
    struct v4l2_plane planes[FMT_NUM_PLANES];
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    LOGD_CAMHW("%s enter,_queued_vbuf_count=%d",__FUNCTION__,_queued_vbuf_count);
    memset(planes, 0, sizeof(struct v4l2_plane) * FMT_NUM_PLANES);
    memset(&v4l2_buf, 0, sizeof(v4l2_buf));
    for (int i = 0; i < _queued_vbuf_count; i++) {
        v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        v4l2_buf.memory = V4L2_MEMORY_USERPTR;
        v4l2_buf.m.planes = planes;
        v4l2_buf.length = FMT_NUM_PLANES;
        for (int j = 0; j < _rx_dev_max; j++) {
            if (-1 == _rx_dev[j]->io_control(VIDIOC_DQBUF, &v4l2_buf)) {
                LOGE_CAMHW("%s: VIDIOC_DQBUF failed, device name: %s\n", __FUNCTION__, _rx_dev[j]->get_device_name());
                ret = XCAM_RETURN_ERROR_UNKNOWN;
                goto exit;
            }
        }
    }
exit:
    LOGD_CAMHW("%s exit",__FUNCTION__);
    return ret;
}

XCamReturn
OfflineFrmRdThread::queue_all_buffer() {
    struct v4l2_buffer v4l2_buf;
    struct v4l2_plane planes[FMT_NUM_PLANES];
    struct rk_aiq_vbuf vbuf;
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    uint32_t interval = 0;

    LOGD_CAMHW("%s enter",__FUNCTION__);
    memset(&v4l2_buf, 0, sizeof(v4l2_buf));
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    v4l2_buf.memory = V4L2_MEMORY_USERPTR;
    memset(planes, 0, sizeof(struct v4l2_plane) * FMT_NUM_PLANES);
    v4l2_buf.m.planes = planes;
    v4l2_buf.length = FMT_NUM_PLANES;

    for(int i=0; i<_vbuf_count; i++) {
        if(!_vbuf_cached_list.empty()) {
            vbuf = _vbuf_cached_list.front();
            _vbuf_cached_list.pop_front();
            v4l2_buf.index = i;
            for(int j = 0; j < _rx_dev_max; j++) {
                v4l2_buf.m.planes[0].m.userptr = (unsigned long)vbuf.buf_info[j].data_addr;
                v4l2_buf.m.planes[0].length = vbuf.buf_info[j].data_length;
                v4l2_buf.m.planes[0].bytesused = v4l2_buf.m.planes[j].length;
                if (-1 == _rx_dev[j]->io_control(VIDIOC_QBUF, &v4l2_buf)) {
                    LOGE_CAMHW("%s: VIDIOC_QBUF failed, device name: %s\n", __FUNCTION__, _rx_dev[j]->get_device_name());
                    ret = XCAM_RETURN_ERROR_UNKNOWN;
                    goto exit;
                }
            }
            _queued_vbuf_count++;
            triger_readback(&vbuf);
            if (i == 0) {
                interval = 0;
            }else {
                interval = vbuf.buf_info[0].timestamp - _last_timestamp;
            }
            offlineFrmRdSleep(interval);
            _last_timestamp = vbuf.buf_info[0].timestamp;
        }
    }
 exit:
    LOGD_CAMHW("%s exit",__FUNCTION__);
    return ret;
}

void
OfflineFrmRdThread::triger_readback(struct rk_aiq_vbuf *vb)
{
    struct isp2x_csi_trigger tg;

    tg.frame_timestamp = vb->buf_info[0].timestamp;
    tg.frame_id = vb->buf_info[0].frame_id;
    tg.times = 1;
    LOGD_CAMHW("%s: id: %d, timestamp: %lld, times: %d\n", __FUNCTION__, tg.frame_id, tg.frame_timestamp, tg.times);
    _isp_dev->io_control(RKISP_CMD_TRIGGER_READ_BACK, &tg);
}

bool
OfflineFrmRdThread::loop () {
    if (_initialized) {
        struct v4l2_buffer v4l2_buf;
        struct v4l2_plane planes[FMT_NUM_PLANES];
        struct rk_aiq_vbuf vbuf;
        uint32_t interval = 0;

        if (XCAM_RETURN_NO_ERROR != queue_all_buffer()) {
            goto exit;
        }
        memset(&v4l2_buf, 0, sizeof(v4l2_buf));
        v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        v4l2_buf.memory = V4L2_MEMORY_USERPTR;
        memset(planes, 0, sizeof(struct v4l2_plane) * FMT_NUM_PLANES);
        v4l2_buf.m.planes = planes;
        v4l2_buf.length = FMT_NUM_PLANES;
        LOGD_CAMHW("list size: %d",_vbuf_cached_list.size());
        while(!_vbuf_cached_list.empty()) {
            vbuf = _vbuf_cached_list.front();
            _vbuf_cached_list.pop_front();
            for(int i = 0; i < _rx_dev_max; i++) {
                if (-1 == _rx_dev[i]->io_control(VIDIOC_DQBUF, &v4l2_buf)) {
                    LOGE_CAMHW("%s: VIDIOC_DQBUF failed, device name: %s\n", __FUNCTION__, _rx_dev[i]->get_device_name());
                    goto exit;
                }
                _queued_vbuf_count--;
                v4l2_buf.m.planes[0].m.userptr = (unsigned long)vbuf.buf_info[i].data_addr;
                v4l2_buf.m.planes[0].length = vbuf.buf_info[i].data_length;
                v4l2_buf.m.planes[0].bytesused = v4l2_buf.m.planes[i].length;
                if (-1 == _rx_dev[i]->io_control(VIDIOC_QBUF, &v4l2_buf)) {
                    LOGE_CAMHW("%s: VIDIOC_QBUF failed, device name: %s\n", __FUNCTION__, _rx_dev[i]->get_device_name());
                    goto exit;
                }
                _queued_vbuf_count++;
            }
            triger_readback(&vbuf);
            interval = vbuf.buf_info[0].timestamp - _last_timestamp;
            offlineFrmRdSleep(interval);
            _last_timestamp = vbuf.buf_info[0].timestamp;
        }
exit:
        dequeue_all_buffer();
        offlineRdJobDone();
    }
    return false;
}

void
OfflineFrmRdThread::set_mipi_devs(SmartPtr<V4l2Device> mipi_rx_devs[3], SmartPtr<V4l2SubDevice> isp_dev)
{
    _isp_dev = isp_dev;

    for (int i = 0; i < 3; i++) {
        _rx_dev[i] = mipi_rx_devs[i];
    }
}

XCamReturn
OfflineFrmRdThread::enqueueBuffer(struct rk_aiq_vbuf *buf)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    LOGD_CAMHW("%s enter",__FUNCTION__);
    _vbuf_cached_list.push_back(*buf);
    LOGD_CAMHW("%s exit",__FUNCTION__);
    return ret;
}

void
OfflineFrmRdThread::offlineRdJobPrepare()
{
    LOGD_CAMHW("%s enter",__FUNCTION__);
    _vbuf_count = _rx_dev[0]->get_buffer_count();
    _initialized = true;
    _poll->set_loop_status(true);
    LOGD_CAMHW("%s exit",__FUNCTION__);
}

void
OfflineFrmRdThread::offlineRdJobDone()
{
    LOGD_CAMHW("%s enter",__FUNCTION__);
    _initialized = false;
    _poll->set_loop_status(false);
    LOGD_CAMHW("%s exit",__FUNCTION__);
}

void
OfflineFrmRdThread::set_working_mode(int hdr_mode)
{
    _work_mode = hdr_mode;
    switch (_work_mode) {
    case RK_AIQ_ISP_HDR_MODE_3_FRAME_HDR:
    case RK_AIQ_ISP_HDR_MODE_3_LINE_HDR:
        _rx_dev_max = 3;
        break;
    case RK_AIQ_ISP_HDR_MODE_2_FRAME_HDR:
    case RK_AIQ_ISP_HDR_MODE_2_LINE_HDR:
        _rx_dev_max = 2;
        break;
    default:
        _rx_dev_max = 1;
    }
}

}; //namspace RkCam
