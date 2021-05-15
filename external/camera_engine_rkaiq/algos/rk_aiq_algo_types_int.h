/*
 * rk_aiq_algo_types_int.h
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

#ifndef _RK_AIQ_ALGO_TYPES_INT_H_
#define _RK_AIQ_ALGO_TYPES_INT_H_


#include "rk_aiq_algo_des.h"
#include "rk_aiq_types_priv.h"
#include "rk_aiq_algo_types.h"
#include "adehaze/rk_aiq_types_adehaze_algo_int.h"
#include "anr/rk_aiq_types_anr_algo_int.h"
#include "asharp/rk_aiq_types_asharp_algo_int.h"
#include "ahdr/rk_aiq_types_ahdr_algo_int.h"
#include "ae/rk_aiq_types_ae_algo_int.h"
#include "af/rk_aiq_types_af_algo_int.h"
#include "aorb/rk_aiq_types_orb_algo.h"
#include "awb/rk_aiq_types_awb_algo_int.h"
#include "agamma/rk_aiq_types_agamma_algo_int.h"
#include "adpcc/rk_aiq_types_adpcc_algo_int.h"
#include "adebayer/rk_aiq_types_algo_adebayer_int.h"
#include "RkAiqCalibDbTypes.h"
#include "ablc/rk_aiq_types_ablc_algo_int.h"
#include "alsc/rk_aiq_types_alsc_algo_int.h"
#include "accm/rk_aiq_types_accm_algo_int.h"
#include "a3dlut/rk_aiq_types_a3dlut_algo_int.h"
#include "acp/rk_aiq_types_acp_algo_int.h"
#include "aie/rk_aiq_types_aie_algo_int.h"
#include "aldch/rk_aiq_types_aldch_algo_int.h"
#include "afec/rk_aiq_types_afec_algo_int.h"

// forward declare
typedef struct _RkAiqAlgoPreResAe     RkAiqAlgoPreResAe;
typedef struct _RkAiqAlgoPreResAwb    RkAiqAlgoPreResAwb;
typedef struct _RkAiqAlgoPreResAf     RkAiqAlgoPreResAf;
typedef struct _RkAiqAlgoPreResAsd    RkAiqAlgoPreResAsd;
typedef struct _RkAiqAlgoPreResAnr    RkAiqAlgoPreResAnr;
typedef struct _RkAiqAlgoPreResAhdr   RkAiqAlgoPreResAhdr;
typedef struct _RkAiqAlgoPreResAcp    RkAiqAlgoPreResAcp;
typedef struct _RkAiqAlgoPreResAsharp RkAiqAlgoPreResAsharp;
typedef struct _RkAiqAlgoPreResAdhaz  RkAiqAlgoPreResAdhaz;
typedef struct _RkAiqAlgoPreResA3dlut RkAiqAlgoPreResA3dlut;
typedef struct _RkAiqAlgoPreResAblc   RkAiqAlgoPreResAblc;
typedef struct _RkAiqAlgoPreResAccm   RkAiqAlgoPreResAccm;
typedef struct _RkAiqAlgoPreResAcgc   RkAiqAlgoPreResAcgc;
typedef struct _RkAiqAlgoPreResAdebayer RkAiqAlgoPreResAdebayer;
typedef struct _RkAiqAlgoPreResAdpcc  RkAiqAlgoPreResAdpcc;
typedef struct _RkAiqAlgoPreResAfec   RkAiqAlgoPreResAfec;
typedef struct _RkAiqAlgoPreResAgamma RkAiqAlgoPreResAgamma;
typedef struct _RkAiqAlgoPreResAgic   RkAiqAlgoPreResAgic;
typedef struct _RkAiqAlgoPreResAie    RkAiqAlgoPreResAie;
typedef struct _RkAiqAlgoPreResAldch  RkAiqAlgoPreResAldch;
typedef struct _RkAiqAlgoPreResAlsc   RkAiqAlgoPreResAlsc;
typedef struct _RkAiqAlgoPreResAorb   RkAiqAlgoPreResAorb;
typedef struct _RkAiqAlgoPreResAr2y   RkAiqAlgoPreResAr2y;
typedef struct _RkAiqAlgoPreResAwdr   RkAiqAlgoPreResAwdr;

typedef struct _RkAiqAlgoProcResAe    RkAiqAlgoProcResAe;
typedef struct _RkAiqAlgoProcResAwb   RkAiqAlgoProcResAwb;
typedef struct _RkAiqAlgoProcResAf    RkAiqAlgoProcResAf;
typedef struct _RkAiqAlgoProcResAsd   RkAiqAlgoProcResAsd;
typedef struct _RkAiqAlgoProcResAnr   RkAiqAlgoProcResAnr;
typedef struct _RkAiqAlgoProcResAhdr  RkAiqAlgoProcResAhdr;
typedef struct _RkAiqAlgoProcResAcp   RkAiqAlgoProcResAcp;
typedef struct _RkAiqAlgoProcResAsharp RkAiqAlgoProcResAsharp;
typedef struct _RkAiqAlgoProcResAdhaz RkAiqAlgoProcResAdhaz;
typedef struct _RkAiqAlgoProcResA3dlut RkAiqAlgoProcResA3dlut;
typedef struct _RkAiqAlgoProcResAblc  RkAiqAlgoProcResAblc;
typedef struct _RkAiqAlgoProcResAccm  RkAiqAlgoProcResAccm;
typedef struct _RkAiqAlgoProcResAcgc  RkAiqAlgoProcResAcgc;
typedef struct _RkAiqAlgoProcResAdebayer RkAiqAlgoProcResAdebayer;
typedef struct _RkAiqAlgoProcResAdpcc RkAiqAlgoProcResAdpcc;
typedef struct _RkAiqAlgoProcResAfec  RkAiqAlgoProcResAfec;
typedef struct _RkAiqAlgoProcResAgamma RkAiqAlgoProcResAgamma;
typedef struct _RkAiqAlgoProcResAgic  RkAiqAlgoProcResAgic;
typedef struct _RkAiqAlgoProcResAie   RkAiqAlgoProcResAie;
typedef struct _RkAiqAlgoProcResAldch RkAiqAlgoProcResAldch;
typedef struct _RkAiqAlgoProcResAlsc  RkAiqAlgoProcResAlsc;
typedef struct _RkAiqAlgoProcResAorb  RkAiqAlgoProcResAorb;
typedef struct _RkAiqAlgoProcResAr2y  RkAiqAlgoProcResAr2y;
typedef struct _RkAiqAlgoProcResAwdr  RkAiqAlgoProcResAwdr;

typedef struct _RkAiqAlgoPostResAe    RkAiqAlgoPostResAe;
typedef struct _RkAiqAlgoPostResAwb   RkAiqAlgoPostResAwb;
typedef struct _RkAiqAlgoPostResAf    RkAiqAlgoPostResAf;
typedef struct _RkAiqAlgoPostResAsd   RkAiqAlgoPostResAsd;
typedef struct _RkAiqAlgoPostResAnr   RkAiqAlgoPostResAnr;
typedef struct _RkAiqAlgoPostResAhdr  RkAiqAlgoPostResAhdr;
typedef struct _RkAiqAlgoPostResAcp   RkAiqAlgoPostResAcp;
typedef struct _RkAiqAlgoPostResAsharp RkAiqAlgoPostResAsharp;
typedef struct _RkAiqAlgoPostResAdhaz RkAiqAlgoPostResAdhaz;
typedef struct _RkAiqAlgoPostResA3dlut RkAiqAlgoPostResA3dlut;
typedef struct _RkAiqAlgoPostResAblc  RkAiqAlgoPostResAblc;
typedef struct _RkAiqAlgoPostResAccm  RkAiqAlgoPostResAccm;
typedef struct _RkAiqAlgoPostResAcgc  RkAiqAlgoPostResAcgc;
typedef struct _RkAiqAlgoPostResAdebayer RkAiqAlgoPostResAdebayer;
typedef struct _RkAiqAlgoPostResAdpcc RkAiqAlgoPostResAdpcc;
typedef struct _RkAiqAlgoPostResAfec  RkAiqAlgoPostResAfec;
typedef struct _RkAiqAlgoPostResAgamma RkAiqAlgoPostResAgamma;
typedef struct _RkAiqAlgoPostResAgic  RkAiqAlgoPostResAgic;
typedef struct _RkAiqAlgoPostResAie   RkAiqAlgoPostResAie;
typedef struct _RkAiqAlgoPostResAldch RkAiqAlgoPostResAldch;
typedef struct _RkAiqAlgoPostResAlsc  RkAiqAlgoPostResAlsc;
typedef struct _RkAiqAlgoPostResAorb  RkAiqAlgoPostResAorb;
typedef struct _RkAiqAlgoPostResAr2y  RkAiqAlgoPostResAr2y;
typedef struct _RkAiqAlgoPostResAwdr  RkAiqAlgoPostResAwdr;

typedef struct _RkAiqPreResComb {
    RkAiqAlgoPreResAe*    ae_pre_res;
    RkAiqAlgoPreResAwb*   awb_pre_res;
    RkAiqAlgoPreResAf*    af_pre_res;
    RkAiqAlgoPreResAsd*   asd_pre_res;
    RkAiqAlgoPreResAnr*   anr_pre_res;
    RkAiqAlgoPreResAhdr*  ahdr_pre_res;
    RkAiqAlgoPreResAcp* acp_pre_res;
    RkAiqAlgoPreResAsharp* asharp_pre_res;
    RkAiqAlgoPreResAdhaz* adhaz_pre_res;
    RkAiqAlgoPreResA3dlut* a3dlut_pre_res;
    RkAiqAlgoPreResAblc* ablc_pre_res;
    RkAiqAlgoPreResAccm* accm_pre_res;
    RkAiqAlgoPreResAcgc* acgc_pre_res;
    RkAiqAlgoPreResAdebayer* adebayer_pre_res;
    RkAiqAlgoPreResAdpcc* adpcc_pre_res;
    RkAiqAlgoPreResAfec* afec_pre_res;
    RkAiqAlgoPreResAgamma* agamma_pre_res;
    RkAiqAlgoPreResAgic* agic_pre_res;
    RkAiqAlgoPreResAie* aie_pre_res;
    RkAiqAlgoPreResAldch* aldch_pre_res;
    RkAiqAlgoPreResAlsc* alsc_pre_res;
    RkAiqAlgoPreResAorb* aorb_pre_res;
    RkAiqAlgoPreResAr2y* ar2y_pre_res;
    RkAiqAlgoPreResAwdr* awdr_pre_res;
} RkAiqPreResComb;

typedef struct _RkAiqProcResComb {
    RkAiqAlgoProcResAe*    ae_proc_res;
    RkAiqAlgoProcResAwb*   awb_proc_res;
    RkAiqAlgoProcResAf*    af_proc_res;
    RkAiqAlgoProcResAsd*   asd_proc_res;
    RkAiqAlgoProcResAnr*   anr_proc_res;
    RkAiqAlgoProcResAhdr*  ahdr_proc_res;
    RkAiqAlgoProcResAcp* acp_proc_res;
    RkAiqAlgoProcResAsharp* asharp_proc_res;
    RkAiqAlgoProcResAdhaz* adhaz_proc_res;
    RkAiqAlgoProcResA3dlut* a3dlut_proc_res;
    RkAiqAlgoProcResAblc* ablc_proc_res;
    RkAiqAlgoProcResAccm* accm_proc_res;
    RkAiqAlgoProcResAcgc* acgc_proc_res;
    RkAiqAlgoProcResAdebayer* adebayer_proc_res;
    RkAiqAlgoProcResAdpcc* adpcc_proc_res;
    RkAiqAlgoProcResAfec* afec_proc_res;
    RkAiqAlgoProcResAgamma* agamma_proc_res;
    RkAiqAlgoProcResAgic* agic_proc_res;
    RkAiqAlgoProcResAie* aie_proc_res;
    RkAiqAlgoProcResAldch* aldch_proc_res;
    RkAiqAlgoProcResAlsc* alsc_proc_res;
    RkAiqAlgoProcResAorb* aorb_proc_res;
    RkAiqAlgoProcResAr2y* ar2y_proc_res;
    RkAiqAlgoProcResAwdr* awdr_proc_res;
} RkAiqProcResComb;

typedef struct _RkAiqPostResComb {
    RkAiqAlgoPostResAe*    ae_post_res;
    RkAiqAlgoPostResAwb*   awb_post_res;
    RkAiqAlgoPostResAf*    af_post_res;
    RkAiqAlgoPostResAsd*   asd_post_res;
    RkAiqAlgoPostResAnr*   anr_post_res;
    RkAiqAlgoPostResAhdr*  ahdr_post_res;
    RkAiqAlgoPostResAcp* acp_post_res;
    RkAiqAlgoPostResAsharp* asharp_post_res;
    RkAiqAlgoPostResAdhaz* adhaz_post_res;
    RkAiqAlgoPostResA3dlut* a3dlut_post_res;
    RkAiqAlgoPostResAblc* ablc_post_res;
    RkAiqAlgoPostResAccm* accm_post_res;
    RkAiqAlgoPostResAcgc* acgc_post_res;
    RkAiqAlgoPostResAdebayer* adebayer_post_res;
    RkAiqAlgoPostResAdpcc* adpcc_post_res;
    RkAiqAlgoPostResAfec* afec_post_res;
    RkAiqAlgoPostResAgamma* agamma_post_res;
    RkAiqAlgoPostResAgic* agic_post_res;
    RkAiqAlgoPostResAie* aie_post_res;
    RkAiqAlgoPostResAldch* aldch_post_res;
    RkAiqAlgoPostResAlsc* alsc_post_res;
    RkAiqAlgoPostResAorb* aorb_post_res;
    RkAiqAlgoPostResAr2y* ar2y_post_res;
    RkAiqAlgoPostResAwdr* awdr_post_res;
} RkAiqPostResComb;

struct CamCalibDbContext_s;
typedef struct CamCalibDbContext_s CamCalibDbContext_t;

typedef struct _RkAiqAlgoComInt {
    union {
        struct {
            CamCalibDbContext_t* calib;
        } prepare;

        struct {
            RkAiqPreResComb*  pre_res_comb;
            RkAiqProcResComb* proc_res_comb;
            RkAiqPostResComb* post_res_comb;
            int iso;
            bool fill_light_on;
            bool gray_mode;
            bool is_bw_sensor;
        } proc;
    } u;
} RkAiqAlgoComInt;

typedef struct _AlgoCtxInstanceCfgInt {
    AlgoCtxInstanceCfg cfg_com;
    CamCalibDbContext_t* calib;
} AlgoCtxInstanceCfgInt;

// Ae
typedef struct _RkAiqAlgoConfigAeInt {
    RkAiqAlgoConfigAe        ae_cfg_com;
    RkAiqAlgoComInt          rk_com;
    int                      RawWidth;
    int                      RawHeight;
    rk_aiq_sensor_nr_switch_t nr_switch;
} RkAiqAlgoConfigAeInt;

typedef struct _RkAiqAlgoPreAeInt {
    RkAiqAlgoPreAe ae_pre_com;
    RkAiqAlgoComInt rk_com;
    RKAiqAecStats_t* ispAeStats;
} RkAiqAlgoPreAeInt;

typedef struct _RkAiqAlgoPreResAeInt {
    RkAiqAlgoPreResAe ae_pre_res_com;
    AecPreResult_t    ae_pre_res_rk;;
} RkAiqAlgoPreResAeInt;

typedef struct _RkAiqAlgoProcAeInt {
    RkAiqAlgoProcAe ae_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAeInt;

typedef struct _RkAiqAlgoProcResAeInt {
    RkAiqAlgoProcResAe   ae_proc_res_com;
    AecProcResult_t      ae_proc_res_rk;
} RkAiqAlgoProcResAeInt;

typedef struct _RkAiqAlgoPostAeInt {
    RkAiqAlgoPostAe ae_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAeInt;

typedef struct _RkAiqAlgoPostResAeInt {
    RkAiqAlgoPostResAe ae_post_res_com;
    AecPostResult_t    ae_post_res_rk;
} RkAiqAlgoPostResAeInt;

//Awb
typedef struct _RkAiqAlgoConfigAwbInt {
    RkAiqAlgoConfigAwb awb_config_com;
    RkAiqAlgoComInt rk_com;
    int rawBit;
} RkAiqAlgoConfigAwbInt;

typedef struct _RkAiqAlgoPreAwbInt {
    RkAiqAlgoPreAwb awb_pre_com;
    RkAiqAlgoComInt rk_com;
    union {
        rk_aiq_awb_stat_res_v200_t awb_hw0_statis;
        rk_aiq_awb_stat_res_v201_t awb_hw1_statis;
    };
    union {
        rk_aiq_awb_stat_cfg_v200_t  awb_cfg_effect_v200;
        rk_aiq_awb_stat_cfg_v201_t  awb_cfg_effect_v201;
    };
} RkAiqAlgoPreAwbInt;

typedef struct _RkAiqAlgoPreResAwbInt {
    RkAiqAlgoPreResAwb awb_pre_res_com;
    color_tempture_info_t cctGloabl;
    color_tempture_info_t cctFirst[4];
    float awb_smooth_factor;
    float varianceLuma;
    rk_aiq_wb_gain_t awb_gain_algo;
    bool awbConverged;
    //blk
    bool blkWpFlagVaLid[RK_AIQ_AWB_GRID_NUM_TOTAL][3];
    int  blkWpFlag[RK_AIQ_AWB_GRID_NUM_TOTAL][3];
    bool blkSgcResVaLid;
    awb_measure_blk_res_fl_t blkSgcResult[RK_AIQ_AWB_GRID_NUM_TOTAL];
} RkAiqAlgoPreResAwbInt;

typedef struct _RkAiqAlgoProcAwbInt {
    RkAiqAlgoProcAwb awb_proc_com;
    RkAiqAlgoComInt rk_com;

} RkAiqAlgoProcAwbInt;

typedef struct _RkAiqAlgoProcResAwbInt {
    RkAiqAlgoProcResAwb awb_proc_res_com;
    color_tempture_info_t cctGloabl;
    color_tempture_info_t cctFirst[4];
    color_tempture_info_t cctBlk[RK_AIQ_AWB_GRID_NUM_TOTAL];
    float awb_smooth_factor;
    float varianceLuma;
    bool awbConverged;
    //blk
    bool blkWpFlagVaLid[RK_AIQ_AWB_GRID_NUM_TOTAL];
    int  blkWpFlag[RK_AIQ_AWB_GRID_NUM_TOTAL][3];
    bool blkSgcResVaLid;
    awb_measure_blk_res_fl_t blkSgcResult[RK_AIQ_AWB_GRID_NUM_TOTAL];

} RkAiqAlgoProcResAwbInt;

typedef struct _RkAiqAlgoPostAwbInt {
    RkAiqAlgoPostAwb awb_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAwbInt;

typedef struct _RkAiqAlgoPostResAwbInt {
    RkAiqAlgoPostResAwb awb_post_res_com;
} RkAiqAlgoPostResAwbInt;

// af
typedef struct _RkAiqAlgoConfigAfInt {
    RkAiqAlgoConfigAf af_config_com;
    RkAiqAlgoComInt rk_com;
    CalibDb_AF_t      af_calib_cfg;
} RkAiqAlgoConfigAfInt;

typedef struct _RkAiqAlgoPreAfInt {
    RkAiqAlgoPreAf af_pre_com;
    RkAiqAlgoComInt rk_com;
    rk_aiq_isp_af_stats_t *af_stats;
    rk_aiq_isp_aec_stats_t *aec_stats;
} RkAiqAlgoPreAfInt;

typedef struct _RkAiqAlgoPreResAfInt {
    RkAiqAlgoPreResAf af_pre_res_com;
    af_preprocess_result_t af_pre_result;
} RkAiqAlgoPreResAfInt;

typedef struct _RkAiqAlgoProcAfInt {
    RkAiqAlgoProcAf af_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAfInt;

typedef struct _RkAiqAlgoProcResAfInt {
    RkAiqAlgoProcResAf af_proc_res_com;
} RkAiqAlgoProcResAfInt;

typedef struct _RkAiqAlgoPostAfInt {
    RkAiqAlgoPostAf af_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAfInt;

typedef struct _RkAiqAlgoPostResAfInt {
    RkAiqAlgoPostResAf af_post_res_com;
} RkAiqAlgoPostResAfInt;

// anr
typedef struct _RkAiqAlgoConfigAnrInt {
    RkAiqAlgoConfigAnr anr_config_com;
    RkAiqAlgoComInt rk_com;
    ANRConfig_t stANRConfig;
} RkAiqAlgoConfigAnrInt;

typedef struct _RkAiqAlgoPreAnrInt {
    RkAiqAlgoPreAnr anr_pre_com;
    RkAiqAlgoComInt rk_com;
    ANRConfig_t stANRConfig;
} RkAiqAlgoPreAnrInt;

typedef struct _RkAiqAlgoPreResAnrInt {
    RkAiqAlgoPreResAnr anr_pre_res_com;
} RkAiqAlgoPreResAnrInt;

typedef struct _RkAiqAlgoProcAnrInt {
    RkAiqAlgoProcAnr anr_proc_com;
    RkAiqAlgoComInt rk_com;
    int iso;
    int hdr_mode;
} RkAiqAlgoProcAnrInt;

typedef struct _RkAiqAlgoProcResAnrInt {
    RkAiqAlgoProcResAnr anr_proc_res_com;
    ANRProcResult_t stAnrProcResult;
} RkAiqAlgoProcResAnrInt;

typedef struct _RkAiqAlgoPostAnrInt {
    RkAiqAlgoPostAnr anr_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAnrInt;

typedef struct _RkAiqAlgoPostResAnrInt {
    RkAiqAlgoPostResAnr anr_post_res_com;
} RkAiqAlgoPostResAnrInt;

// asharp
typedef struct _RkAiqAlgoConfigAsharpInt {
    RkAiqAlgoConfigAsharp asharp_config_com;
    RkAiqAlgoComInt rk_com;
    AsharpConfig_t stAsharpConfig;
} RkAiqAlgoConfigAsharpInt;

typedef struct _RkAiqAlgoPreAsharpInt {
    RkAiqAlgoPreAsharp asharp_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAsharpInt;

typedef struct _RkAiqAlgoPreResAsharpInt {
    RkAiqAlgoPreResAsharp asharp_pre_res_com;
} RkAiqAlgoPreResAsharpInt;

typedef struct _RkAiqAlgoProcAsharpInt {
    RkAiqAlgoProcAsharp asharp_proc_com;
    RkAiqAlgoComInt rk_com;
    int iso;
    int hdr_mode;
} RkAiqAlgoProcAsharpInt;

typedef struct _RkAiqAlgoProcResAsharpInt {
    RkAiqAlgoProcResAsharp asharp_proc_res_com;
    AsharpProcResult_t stAsharpProcResult;
} RkAiqAlgoProcResAsharpInt;

typedef struct _RkAiqAlgoPostAsharpInt {
    RkAiqAlgoPostAsharp asharp_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAsharpInt;

typedef struct _RkAiqAlgoPostResAsharpInt {
    RkAiqAlgoPostResAsharp asharp_post_res_com;
} RkAiqAlgoPostResAsharpInt;


// asd
typedef struct _RkAiqAlgoConfigAsdInt {
    RkAiqAlgoConfigAsd asd_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAsdInt;

typedef struct _RkAiqAlgoPreAsdInt {
    RkAiqAlgoPreAsd asd_pre_com;
    RkAiqAlgoComInt rk_com;
    asd_preprocess_in_t pre_params;
} RkAiqAlgoPreAsdInt;

typedef struct _RkAiqAlgoPreResAsdInt {
    RkAiqAlgoPreResAsd asd_pre_res_com;
    asd_preprocess_result_t asd_result;
} RkAiqAlgoPreResAsdInt;

typedef struct _RkAiqAlgoProcAsdInt {
    RkAiqAlgoProcAsd asd_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAsdInt;

typedef struct _RkAiqAlgoProcResAsdInt {
    RkAiqAlgoProcResAsd asd_proc_res_com;
} RkAiqAlgoProcResAsdInt;

typedef struct _RkAiqAlgoPostAsdInt {
    RkAiqAlgoPostAsd asd_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAsdInt;

typedef struct _RkAiqAlgoPostResAsdInt {
    RkAiqAlgoPostResAsd asd_post_res_com;
} RkAiqAlgoPostResAsdInt;

// ahdr
typedef struct _RkAiqAlgoConfigAhdrInt {
    RkAiqAlgoConfigAhdr ahdr_config_com;
    RkAiqAlgoComInt rk_com;
    //todo
    AhdrConfig_t* ahdrCfg;
    int working_mode;
    unsigned short  rawWidth;               // rawÍ¼¿í
    unsigned short  rawHeight;
} RkAiqAlgoConfigAhdrInt;

typedef struct _RkAiqAlgoPreAhdrInt {
    RkAiqAlgoPreAhdr ahdr_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAhdrInt;

typedef struct _RkAiqAlgoPreResAhdrInt {
    RkAiqAlgoPreResAhdr ahdr_pre_res_com;
} RkAiqAlgoPreResAhdrInt;

typedef struct _RkAiqAlgoProcAhdrInt {
    RkAiqAlgoProcAhdr ahdr_proc_com;
    RkAiqAlgoComInt rk_com;
    rkisp_ahdr_stats_t ispAhdrStats;
    uint32_t width;
    uint32_t height;
} RkAiqAlgoProcAhdrInt;

typedef struct _RkAiqAlgoProcResAhdrInt {
    RkAiqAlgoProcResAhdr ahdr_proc_res_com;
    //todo lrk
    RkAiqAhdrProcResult_t AhdrProcRes;
} RkAiqAlgoProcResAhdrInt;

typedef struct _RkAiqAlgoPostAhdrInt {
    RkAiqAlgoPostAhdr ahdr_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAhdrInt;

typedef struct _RkAiqAlgoPostResAhdrInt {
    RkAiqAlgoPostResAhdr ahdr_post_res_com;
} RkAiqAlgoPostResAhdrInt;

// acp
typedef struct _RkAiqAlgoConfigAcpInt {
    RkAiqAlgoConfigAcp acp_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAcpInt;

typedef struct _RkAiqAlgoPreAcpInt {
    RkAiqAlgoPreAcp acp_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAcpInt;

typedef struct _RkAiqAlgoPreResAcpInt {
    RkAiqAlgoPreResAcp acp_pre_res_com;
} RkAiqAlgoPreResAcpInt;

typedef struct _RkAiqAlgoProcAcpInt {
    RkAiqAlgoProcAcp acp_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAcpInt;

typedef struct _RkAiqAlgoProcResAcpInt {
    RkAiqAlgoProcResAcp acp_proc_res_com;
} RkAiqAlgoProcResAcpInt;

typedef struct _RkAiqAlgoPostAcpInt {
    RkAiqAlgoPostAcp acp_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAcpInt;

typedef struct _RkAiqAlgoPostResAcpInt {
    RkAiqAlgoPostResAcp acp_post_res_com;
} RkAiqAlgoPostResAcpInt;

//adehaze
typedef struct _RkAiqAlgoConfigAdhazInt {
    RkAiqAlgoConfigAdhaz adhaz_config_com;
    RkAiqAlgoComInt rk_com;
    const CamCalibDbContext_t *calib;
} RkAiqAlgoConfigAdhazInt;

typedef struct _RkAiqAlgoPreAdhazInt {
    RkAiqAlgoPreAdhaz adhaz_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAdhazInt;

typedef struct _RkAiqAlgoPreResAdhazInt {
    RkAiqAlgoPreResAdhaz adhaz_pre_res_com;
} RkAiqAlgoPreResAdhazInt;

typedef struct _RkAiqAlgoProcAdhazInt {
    RkAiqAlgoProcAdhaz adhaz_proc_com;
    RkAiqAlgoComInt rk_com;
    const CamCalibDbContext_t *pCalibDehaze;
    int iso;
    int hdr_mode;

} RkAiqAlgoProcAdhazInt;

typedef struct _RkAiqAlgoProcResAdhazInt {
    RkAiqAlgoProcResAdhaz adhaz_proc_res_com;

} RkAiqAlgoProcResAdhazInt;


typedef struct _RkAiqAlgoPostAdhazInt {
    RkAiqAlgoPostAdhaz adhaz_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAdhazInt;

typedef struct _RkAiqAlgoPostResAdhazInt {
    RkAiqAlgoPostResAdhaz adhaz_post_res_com;
} RkAiqAlgoPostResAdhazInt;

// a3dlut
typedef struct _RkAiqAlgoConfigA3dlutInt {
    RkAiqAlgoConfigA3dlut a3dlut_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigA3dlutInt;

typedef struct _RkAiqAlgoPreA3dlutInt {
    RkAiqAlgoPreA3dlut a3dlut_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreA3dlutInt;

typedef struct _RkAiqAlgoPreResA3dlutInt {
    RkAiqAlgoPreResA3dlut a3dlut_pre_res_com;
} RkAiqAlgoPreResA3dlutInt;

typedef struct _RkAiqAlgoProcA3dlutInt {
    RkAiqAlgoProcA3dlut a3dlut_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcA3dlutInt;

typedef struct _RkAiqAlgoProcResA3dlutInt {
    RkAiqAlgoProcResA3dlut a3dlut_proc_res_com;
} RkAiqAlgoProcResA3dlutInt;

typedef struct _RkAiqAlgoPostA3dlutInt {
    RkAiqAlgoPostA3dlut a3dlut_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostA3dlutInt;

typedef struct _RkAiqAlgoPostResA3dlutInt {
    RkAiqAlgoPostResA3dlut a3dlut_post_res_com;
} RkAiqAlgoPostResA3dlutInt;

// ablc
typedef struct _RkAiqAlgoConfigAblcInt {
    RkAiqAlgoConfigAblc ablc_config_com;
    RkAiqAlgoComInt rk_com;
    AblcConfig_t ablc_config;
} RkAiqAlgoConfigAblcInt;

typedef struct _RkAiqAlgoPreAblcInt {
    RkAiqAlgoPreAblc ablc_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAblcInt;

typedef struct _RkAiqAlgoPreResAblcInt {
    RkAiqAlgoPreResAblc ablc_pre_res_com;
} RkAiqAlgoPreResAblcInt;

typedef struct _RkAiqAlgoProcAblcInt {
    RkAiqAlgoProcAblc ablc_proc_com;
    RkAiqAlgoComInt rk_com;
    int iso;
    int hdr_mode;
} RkAiqAlgoProcAblcInt;

typedef struct _RkAiqAlgoProcResAblcInt {
    RkAiqAlgoProcResAblc ablc_proc_res_com;
    AblcProcResult_t ablc_proc_res;
} RkAiqAlgoProcResAblcInt;

typedef struct _RkAiqAlgoPostAblcInt {
    RkAiqAlgoPostAblc ablc_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAblcInt;

typedef struct _RkAiqAlgoPostResAblcInt {
    RkAiqAlgoPostResAblc ablc_post_res_com;
} RkAiqAlgoPostResAblcInt;

// accm
typedef struct _RkAiqAlgoConfigAccmInt {
    RkAiqAlgoConfigAccm accm_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAccmInt;

typedef struct _RkAiqAlgoPreAccmInt {
    RkAiqAlgoPreAccm accm_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAccmInt;

typedef struct _RkAiqAlgoPreResAccmInt {
    RkAiqAlgoPreResAccm accm_pre_res_com;
} RkAiqAlgoPreResAccmInt;

typedef struct _RkAiqAlgoProcAccmInt {
    RkAiqAlgoProcAccm accm_proc_com;
    RkAiqAlgoComInt rk_com;
    accm_sw_info_t   accm_sw_info;
} RkAiqAlgoProcAccmInt;

typedef struct _RkAiqAlgoProcResAccmInt {
    RkAiqAlgoProcResAccm accm_proc_res_com;
} RkAiqAlgoProcResAccmInt;

typedef struct _RkAiqAlgoPostAccmInt {
    RkAiqAlgoPostAccm accm_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAccmInt;

typedef struct _RkAiqAlgoPostResAccmInt {
    RkAiqAlgoPostResAccm accm_post_res_com;
} RkAiqAlgoPostResAccmInt;

// acgc
typedef struct _RkAiqAlgoConfigAcgcInt {
    RkAiqAlgoConfigAcgc acgc_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAcgcInt;

typedef struct _RkAiqAlgoPreAcgcInt {
    RkAiqAlgoPreAcgc acgc_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAcgcInt;

typedef struct _RkAiqAlgoPreResAcgcInt {
    RkAiqAlgoPreResAcgc acgc_pre_res_com;
} RkAiqAlgoPreResAcgcInt;

typedef struct _RkAiqAlgoProcAcgcInt {
    RkAiqAlgoProcAcgc acgc_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAcgcInt;

typedef struct _RkAiqAlgoProcResAcgcInt {
    RkAiqAlgoProcResAcgc acgc_proc_res_com;
} RkAiqAlgoProcResAcgcInt;

typedef struct _RkAiqAlgoPostAcgcInt {
    RkAiqAlgoPostAcgc acgc_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAcgcInt;

typedef struct _RkAiqAlgoPostResAcgcInt {
    RkAiqAlgoPostResAcgc acgc_post_res_com;
} RkAiqAlgoPostResAcgcInt;

// adebayer
typedef struct _RkAiqAlgoConfigAdebayerInt {
    RkAiqAlgoConfigAdebayer adebayer_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAdebayerInt;

typedef struct _RkAiqAlgoPreAdebayerInt {
    RkAiqAlgoPreAdebayer adebayer_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAdebayerInt;

typedef struct _RkAiqAlgoPreResAdebayerInt {
    RkAiqAlgoPreResAdebayer adebayer_pre_res_com;
} RkAiqAlgoPreResAdebayerInt;

typedef struct _RkAiqAlgoProcAdebayerInt {
    RkAiqAlgoProcAdebayer adebayer_proc_com;
    RkAiqAlgoComInt rk_com;
    int hdr_mode;
} RkAiqAlgoProcAdebayerInt;

typedef struct _RkAiqAlgoProcResAdebayerInt {
    RkAiqAlgoProcResAdebayer adebayer_proc_res_com;
    AdebayerProcResult_t debayerRes;
} RkAiqAlgoProcResAdebayerInt;

typedef struct _RkAiqAlgoPostAdebayerInt {
    RkAiqAlgoPostAdebayer adebayer_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAdebayerInt;

typedef struct _RkAiqAlgoPostResAdebayerInt {
    RkAiqAlgoPostResAdebayer adebayer_post_res_com;
} RkAiqAlgoPostResAdebayerInt;

// adpcc
typedef struct _RkAiqAlgoConfigAdpccInt {
    RkAiqAlgoConfigAdpcc adpcc_config_com;
    RkAiqAlgoComInt rk_com;
    AdpccConfig_t stAdpccConfig;
} RkAiqAlgoConfigAdpccInt;

typedef struct _RkAiqAlgoPreAdpccInt {
    RkAiqAlgoPreAdpcc adpcc_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAdpccInt;

typedef struct _RkAiqAlgoPreResAdpccInt {
    RkAiqAlgoPreResAdpcc adpcc_pre_res_com;
} RkAiqAlgoPreResAdpccInt;

typedef struct _RkAiqAlgoProcAdpccInt {
    RkAiqAlgoProcAdpcc adpcc_proc_com;
    RkAiqAlgoComInt rk_com;
    int iso;
    int hdr_mode;
} RkAiqAlgoProcAdpccInt;

typedef struct _RkAiqAlgoProcResAdpccInt {
    RkAiqAlgoProcResAdpcc adpcc_proc_res_com;
    AdpccProcResult_t stAdpccProcResult;
} RkAiqAlgoProcResAdpccInt;

typedef struct _RkAiqAlgoPostAdpccInt {
    RkAiqAlgoPostAdpcc adpcc_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAdpccInt;

typedef struct _RkAiqAlgoPostResAdpccInt {
    RkAiqAlgoPostResAdpcc adpcc_post_res_com;
} RkAiqAlgoPostResAdpccInt;

// afec
typedef struct _RkAiqAlgoConfigAfecInt {
    RkAiqAlgoConfigAfec afec_config_com;
    RkAiqAlgoComInt rk_com;
    CalibDb_FEC_t afec_calib_cfg;
    const char* resource_path;
    isp_drv_share_mem_ops_t *mem_ops_ptr;
} RkAiqAlgoConfigAfecInt;

typedef struct _RkAiqAlgoPreAfecInt {
    RkAiqAlgoPreAfec afec_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAfecInt;

typedef struct _RkAiqAlgoPreResAfecInt {
    RkAiqAlgoPreResAfec afec_pre_res_com;
} RkAiqAlgoPreResAfecInt;

typedef struct _RkAiqAlgoProcAfecInt {
    RkAiqAlgoProcAfec afec_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAfecInt;

typedef struct _RkAiqAlgoProcResAfecInt {
    RkAiqAlgoProcResAfec afec_proc_res_com;
    fec_preprocess_result_t afec_result;
} RkAiqAlgoProcResAfecInt;

typedef struct _RkAiqAlgoPostAfecInt {
    RkAiqAlgoPostAfec afec_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAfecInt;

typedef struct _RkAiqAlgoPostResAfecInt {
    RkAiqAlgoPostResAfec afec_post_res_com;
} RkAiqAlgoPostResAfecInt;

// agamma
typedef struct _RkAiqAlgoConfigAgammaInt {
    RkAiqAlgoConfigAgamma agamma_config_com;
    RkAiqAlgoComInt rk_com;
    const CamCalibDbContext_t *calib;
} RkAiqAlgoConfigAgammaInt;

typedef struct _RkAiqAlgoPreAgammaInt {
    RkAiqAlgoPreAgamma agamma_pre_com;
    RkAiqAlgoComInt rk_com;
    int work_mode;
} RkAiqAlgoPreAgammaInt;

typedef struct _RkAiqAlgoPreResAgammaInt {
    RkAiqAlgoPreResAgamma agamma_pre_res_com;
    rk_aiq_gamma_cfg_t agamma_config;
} RkAiqAlgoPreResAgammaInt;

typedef struct _RkAiqAlgoProcAgammaInt {
    RkAiqAlgoProcAgamma agamma_proc_com;
    RkAiqAlgoComInt rk_com;
    const CamCalibDbContext_t *calib;
} RkAiqAlgoProcAgammaInt;

typedef struct _RkAiqAlgoProcResAgammaInt {
    RkAiqAlgoProcResAgamma agamma_proc_res_com;

} RkAiqAlgoProcResAgammaInt;

typedef struct _RkAiqAlgoPostAgammaInt {
    RkAiqAlgoPostAgamma agamma_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAgammaInt;

typedef struct _RkAiqAlgoPostResAgammaInt {
    RkAiqAlgoPostResAgamma agamma_post_res_com;
} RkAiqAlgoPostResAgammaInt;

// agic
typedef struct _RkAiqAlgoConfigAgicInt {
    RkAiqAlgoConfigAgic agic_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAgicInt;

typedef struct _RkAiqAlgoPreAgicInt {
    RkAiqAlgoPreAgic agic_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAgicInt;

typedef struct _RkAiqAlgoPreResAgicInt {
    RkAiqAlgoPreResAgic agic_pre_res_com;
} RkAiqAlgoPreResAgicInt;

typedef struct _RkAiqAlgoProcAgicInt {
    RkAiqAlgoProcAgic agic_proc_com;
    RkAiqAlgoComInt rk_com;
    int hdr_mode;
} RkAiqAlgoProcAgicInt;

typedef struct _RkAiqAlgoProcResAgicInt {
    RkAiqAlgoProcResAgic agic_proc_res_com;
    AgicProcResult_t gicRes;
} RkAiqAlgoProcResAgicInt;

typedef struct _RkAiqAlgoPostAgicInt {
    RkAiqAlgoPostAgic agic_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAgicInt;

typedef struct _RkAiqAlgoPostResAgicInt {
    RkAiqAlgoPostResAgic agic_post_res_com;
} RkAiqAlgoPostResAgicInt;

// aie
typedef struct _RkAiqAlgoConfigAieInt {
    RkAiqAlgoConfigAie aie_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAieInt;

typedef struct _RkAiqAlgoPreAieInt {
    RkAiqAlgoPreAie aie_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAieInt;

typedef struct _RkAiqAlgoPreResAieInt {
    RkAiqAlgoPreResAie aie_pre_res_com;
} RkAiqAlgoPreResAieInt;

typedef struct _RkAiqAlgoProcAieInt {
    RkAiqAlgoProcAie aie_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAieInt;

typedef struct _RkAiqAlgoProcResAieInt {
    RkAiqAlgoProcResAie aie_proc_res_com;
    rk_aiq_aie_params_int_t params;
} RkAiqAlgoProcResAieInt;

typedef struct _RkAiqAlgoPostAieInt {
    RkAiqAlgoPostAie aie_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAieInt;

typedef struct _RkAiqAlgoPostResAieInt {
    RkAiqAlgoPostResAie aie_post_res_com;
} RkAiqAlgoPostResAieInt;

// aldch
typedef struct _RkAiqAlgoConfigAldchInt {
    RkAiqAlgoConfigAldch aldch_config_com;
    RkAiqAlgoComInt rk_com;
    CalibDb_LDCH_t aldch_calib_cfg;
    const char* resource_path;
    isp_drv_share_mem_ops_t *mem_ops_ptr;
} RkAiqAlgoConfigAldchInt;

typedef struct _RkAiqAlgoPreAldchInt {
    RkAiqAlgoPreAldch aldch_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAldchInt;

typedef struct _RkAiqAlgoPreResAldchInt {
    RkAiqAlgoPreResAldch aldch_pre_res_com;
} RkAiqAlgoPreResAldchInt;

typedef struct _RkAiqAlgoProcAldchInt {
    RkAiqAlgoProcAldch aldch_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAldchInt;

typedef struct _RkAiqAlgoProcResAldchInt {
    RkAiqAlgoProcResAldch aldch_proc_res_com;
    ldch_process_result_t ldch_result;
} RkAiqAlgoProcResAldchInt;

typedef struct _RkAiqAlgoPostAldchInt {
    RkAiqAlgoPostAldch aldch_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAldchInt;

typedef struct _RkAiqAlgoPostResAldchInt {
    RkAiqAlgoPostResAldch aldch_post_res_com;
} RkAiqAlgoPostResAldchInt;

// alsc
typedef struct _RkAiqAlgoConfigAlscInt {
    RkAiqAlgoConfigAlsc alsc_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAlscInt;

typedef struct _RkAiqAlgoPreAlscInt {
    RkAiqAlgoPreAlsc alsc_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAlscInt;

typedef struct _RkAiqAlgoPreResAlscInt {
    RkAiqAlgoPreResAlsc alsc_pre_res_com;
} RkAiqAlgoPreResAlscInt;

typedef struct _RkAiqAlgoProcAlscInt {
    RkAiqAlgoProcAlsc alsc_proc_com;
    RkAiqAlgoComInt rk_com;
    alsc_sw_info_t   alsc_sw_info;
} RkAiqAlgoProcAlscInt;

typedef struct _RkAiqAlgoProcResAlscInt {
    RkAiqAlgoProcResAlsc alsc_proc_res_com;
} RkAiqAlgoProcResAlscInt;

typedef struct _RkAiqAlgoPostAlscInt {
    RkAiqAlgoPostAlsc alsc_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAlscInt;

typedef struct _RkAiqAlgoPostResAlscInt {
    RkAiqAlgoPostResAlsc alsc_post_res_com;
} RkAiqAlgoPostResAlscInt;

// aorb
typedef struct _RkAiqAlgoConfigAorbInt {
    RkAiqAlgoConfigAorb aorb_config_com;
    RkAiqAlgoComInt rk_com;
    CalibDb_ORB_t orb_calib_cfg;
} RkAiqAlgoConfigAorbInt;

typedef struct _RkAiqAlgoPreAorbInt {
    RkAiqAlgoPreAorb aorb_pre_com;
    RkAiqAlgoComInt rk_com;
    rk_aiq_isp_orb_stats_t* orb_stats;
} RkAiqAlgoPreAorbInt;

typedef struct _RkAiqAlgoPreResAorbInt {
    RkAiqAlgoPreResAorb aorb_pre_res_com;
    orb_preprocess_result_t orb_pre_result;
} RkAiqAlgoPreResAorbInt;

typedef struct _RkAiqAlgoProcAorbInt {
    RkAiqAlgoProcAorb aorb_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAorbInt;

typedef struct _RkAiqAlgoProcResAorbInt {
    RkAiqAlgoProcResAorb aorb_proc_res_com;
    rk_aiq_isp_orb_meas_t aorb_meas;
} RkAiqAlgoProcResAorbInt;

typedef struct _RkAiqAlgoPostAorbInt {
    RkAiqAlgoPostAorb aorb_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAorbInt;

typedef struct _RkAiqAlgoPostResAorbInt {
    RkAiqAlgoPostResAorb aorb_post_res_com;
} RkAiqAlgoPostResAorbInt;

// ar2y
typedef struct _RkAiqAlgoConfigAr2yInt {
    RkAiqAlgoConfigAr2y ar2y_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAr2yInt;

typedef struct _RkAiqAlgoPreAr2yInt {
    RkAiqAlgoPreAr2y ar2y_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAr2yInt;

typedef struct _RkAiqAlgoPreResAr2yInt {
    RkAiqAlgoPreResAr2y ar2y_pre_res_com;
} RkAiqAlgoPreResAr2yInt;

typedef struct _RkAiqAlgoProcAr2yInt {
    RkAiqAlgoProcAr2y ar2y_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAr2yInt;

typedef struct _RkAiqAlgoProcResAr2yInt {
    RkAiqAlgoProcResAr2y ar2y_proc_res_com;
} RkAiqAlgoProcResAr2yInt;

typedef struct _RkAiqAlgoPostAr2yInt {
    RkAiqAlgoPostAr2y ar2y_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAr2yInt;

typedef struct _RkAiqAlgoPostResAr2yInt {
    RkAiqAlgoPostResAr2y ar2y_post_res_com;
} RkAiqAlgoPostResAr2yInt;

// awdr
typedef struct _RkAiqAlgoConfigAwdrInt {
    RkAiqAlgoConfigAwdr awdr_config_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoConfigAwdrInt;

typedef struct _RkAiqAlgoPreAwdrInt {
    RkAiqAlgoPreAwdr awdr_pre_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPreAwdrInt;

typedef struct _RkAiqAlgoPreResAwdrInt {
    RkAiqAlgoPreResAwdr awdr_pre_res_com;
} RkAiqAlgoPreResAwdrInt;

typedef struct _RkAiqAlgoProcAwdrInt {
    RkAiqAlgoProcAwdr awdr_proc_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoProcAwdrInt;

typedef struct _RkAiqAlgoProcResAwdrInt {
    RkAiqAlgoProcResAwdr awdr_proc_res_com;
} RkAiqAlgoProcResAwdrInt;

typedef struct _RkAiqAlgoPostAwdrInt {
    RkAiqAlgoPostAwdr awdr_post_com;
    RkAiqAlgoComInt rk_com;
} RkAiqAlgoPostAwdrInt;

typedef struct _RkAiqAlgoPostResAwdrInt {
    RkAiqAlgoPostResAwdr awdr_post_res_com;
} RkAiqAlgoPostResAwdrInt;

#endif
