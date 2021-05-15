/*
 * Isp20OfflineFrmRead.h - isp20 offline frame reading thread for event and buffer
 *
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
#ifndef _ISP20_OFFLINE_FRM_READ_H_
#define _ISP20_OFFLINE_FRM_READ_H_

#include "xcam_thread.h"
#include "poll_thread.h"
#include "SensorHw.h"
#include "LensHw.h"
#include "CamHwIsp20.h"

using namespace XCam;

namespace RkCam {

#define ISP2X_FASTBOOT_DELAY_MS         100

class OfflineFrmRdThread
    : public Thread
{
public:
    explicit OfflineFrmRdThread (Isp20PollThread *poll)
        : Thread("offlineFrmRead"),
          _poll (poll) {
        _rx_dev_max = 0;
        _vbuf_count = 0;
        _queued_vbuf_count = 0;
        _initialized = false;
        _last_timestamp = 0;
    }
    void set_mipi_devs(SmartPtr<V4l2Device> mipi_rx_devs[3], SmartPtr<V4l2SubDevice> isp_dev);
    XCamReturn enqueueBuffer(struct rk_aiq_vbuf *buf);
    void set_working_mode(int hdr_mode);
    void offlineRdJobPrepare();
    void offlineRdJobDone();
protected:
    virtual bool loop ();
    XCamReturn queue_all_buffer();
    XCamReturn dequeue_all_buffer ();
    void triger_readback(rk_aiq_vbuf *vb);
private:
    Isp20PollThread *_poll;
    SmartPtr<V4l2SubDevice> _isp_dev;
    SmartPtr<V4l2Device>  _rx_dev[3];
    int _rx_dev_max;
    int _vbuf_count;
    int _queued_vbuf_count;
    bool _initialized;
    std::list<struct rk_aiq_vbuf> _vbuf_cached_list;
    Mutex _mutex;
    int _work_mode;
    uint32_t _last_timestamp;
};

}
#endif
