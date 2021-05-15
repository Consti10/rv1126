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

#ifndef _SENSOR_HW_BASE_H_
#define _SENSOR_HW_BASE_H_

#include <map>
#include <list>
#include <istream>
#include "v4l2_device.h"
#include "rk_aiq_pool.h"
#include "linux/rk-camera-module.h"

/************ BELOW FROM kernel/include/uapi/linux/rk-preisp.h ************/

/* test hdr function */
/*
* struct hdrae_exp_s - hdrae exposure
*
*/

enum cg_mode_e {
    GAIN_MODE_LCG,
    GAIN_MODE_HCG,
};

struct hdrae_exp_s {
    unsigned int long_exp_reg;
    unsigned int long_gain_reg;
    unsigned int middle_exp_reg;
    unsigned int middle_gain_reg;
    unsigned int short_exp_reg;
    unsigned int short_gain_reg;
    unsigned int long_exp_val;
    unsigned int long_gain_val;
    unsigned int middle_exp_val;
    unsigned int middle_gain_val;
    unsigned int short_exp_val;
    unsigned int short_gain_val;
    unsigned char long_cg_mode;
    unsigned char middle_cg_mode;
    unsigned char short_cg_mode;
};

#define SENSOR_CMD_SET_HDRAE_EXP        \
    _IOW('V', BASE_VIDIOC_PRIVATE + 0, struct hdrae_exp_s)

/************ UPPER FROM kernel/include/uapi/linux/rk-preisp.h ************/

using namespace XCam;

namespace RkCam {

#define SENSOR_SUBM (0x4)

class SensorHw : public V4l2SubDevice {
public:
    explicit SensorHw(const char* name);
    virtual ~SensorHw();

    XCamReturn setExposureParams(SmartPtr<RkAiqExpParamsProxy>& expPar);
    XCamReturn getSensorModeData(const char* sns_ent_name,
                                 rk_aiq_exposure_sensor_descriptor& sns_des);

    XCamReturn handle_sof(int64_t time, int frameid);
    int get_pixel(rk_aiq_exposure_sensor_descriptor* sns_des);
    int get_blank(rk_aiq_exposure_sensor_descriptor* sns_des);
    int get_exposure_range(rk_aiq_exposure_sensor_descriptor* sns_des);
    int get_format(rk_aiq_exposure_sensor_descriptor* sns_des);

    XCamReturn get_sensor_descriptor (rk_aiq_exposure_sensor_descriptor* sns_des);
    XCamReturn get_sensor_mode_data (struct isp_supplemental_sensor_mode_data &sensor_mode_data,
                                     int frame_id = -1);
    XCamReturn getEffectiveExpParams(SmartPtr<RkAiqExpParamsProxy>& ExpParams, int frame_id);
    XCamReturn set_working_mode(int mode);
    XCamReturn set_exp_delay_info(int time_delay, int gain_delay, int hcg_lcg_mode_delay);
    XCamReturn set_mirror_flip(bool mirror, bool flip, int32_t& skip_frame_sequence);
    XCamReturn get_mirror_flip(bool& mirror, bool& flip);
    XCamReturn start();
    XCamReturn stop();

private:
    XCAM_DEAD_COPY (SensorHw);
    Mutex _mutex;
    int _working_mode;
    std::list<std::pair<SmartPtr<RkAiqExpParamsProxy>, bool>> _exp_list;
    std::map<int, SmartPtr<RkAiqExpParamsProxy>> _effecting_exp_map;
    bool _first;
    int _frame_sequence;
    rk_aiq_exposure_sensor_descriptor _sensor_desc;
    std::list<SmartPtr<RkAiqExpParamsProxy>> _delayed_gain_list;
    std::list<SmartPtr<RkAiqExpParamsProxy>> _delayed_dcg_gain_mode_list;
    SmartPtr<RkAiqExpParamsProxy> _last_exp_time;
    SmartPtr<RkAiqExpParamsProxy> _last_exp_gain;
    SmartPtr<RkAiqExpParamsProxy> _last_dcg_gain_mode;
    int _gain_delay;
    int _time_delay;
    bool _gain_delayed;
    int _dcg_gain_mode_delay;
    bool _dcg_gain_mode_delayed;
    bool _dcg_gain_mode_with_time;
    SmartPtr<RkAiqExpParamsPool> _expParamsPool;
    static uint16_t DEFAULT_POOL_SIZE;
    std::string _sns_entity_name;
    bool _mirror;
    bool _flip;
    bool _update_mirror_flip;
    int get_sensor_fps(float& fps);
    XCamReturn setLinearSensorExposure(RKAiqAecExpInfo_t* expPar);
    XCamReturn setHdrSensorExposure(RKAiqAecExpInfo_t* expPar);
    XCamReturn setExposure(int frameid);
    XCamReturn setSensorDpcc(Sensor_dpcc_res_t* SensorDpccInfo);
    uint32_t get_v4l2_pixelformat(uint32_t pixelcode);
    XCamReturn composeExpParam( RKAiqAecExpInfo_t* timeValid, RKAiqAecExpInfo_t* gainValid, RKAiqAecExpInfo_t* dcgGainModeValid, RKAiqAecExpInfo_t* newExp);
    int get_nr_switch(rk_aiq_sensor_nr_switch_t* nr_switch);
    XCamReturn _set_mirror_flip();

};

}; //namespace RkCam

#endif
