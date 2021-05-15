/*
 * Isp20StatsBuffer.h - isp20 stats buffer
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

#ifndef ISP20_STATS_BUFFER_H
#define ISP20_STATS_BUFFER_H

#include "rk_aiq_pool.h"
#include "v4l2_buffer_proxy.h"

using namespace XCam;

namespace RkCam {

class Isp20StatsBuffer
    : public V4l2BufferProxy {
public:
    explicit Isp20StatsBuffer(SmartPtr<V4l2Buffer> buf,
                              SmartPtr<V4l2Device> &device,
                              SmartPtr<RkAiqIspParamsProxy> IspParams,
                              SmartPtr<RkAiqExpParamsProxy> ExpParams,
                              SmartPtr<RkAiqAfInfoProxy> AfParams,
                              SmartPtr<RkAiqIrisParamsProxy> IrisParams);
    virtual ~Isp20StatsBuffer() {};

    SmartPtr<RkAiqExpParamsProxy>& get_exp_params () {
        // XCAM_ASSERT (_expParams.ptr ());
        return _expParams;
    }

    SmartPtr<RkAiqIspParamsProxy>& get_isp_params () {
        // XCAM_ASSERT (_ispParams.ptr ());
        return _ispParams;
    }

    SmartPtr<RkAiqAfInfoProxy>& get_af_params () {
        return _afParams;
    }

    SmartPtr<RkAiqIrisParamsProxy>& get_iris_params () {
        return _irisParams;
    }

private:
    XCAM_DEAD_COPY(Isp20StatsBuffer);
    SmartPtr<RkAiqExpParamsProxy> _expParams;
    SmartPtr<RkAiqIspParamsProxy> _ispParams;
    SmartPtr<RkAiqAfInfoProxy> _afParams;
    SmartPtr<RkAiqIrisParamsProxy> _irisParams;
};
}

#endif
