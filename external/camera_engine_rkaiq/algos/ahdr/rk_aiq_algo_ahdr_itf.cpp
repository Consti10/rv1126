/*
 * rk_aiq_algo_ae_itf.c
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

#include "rk_aiq_algo_types_int.h"
#include "ahdr/rk_aiq_algo_ahdr_itf.h"
#include "xcam_log.h"
#include "ahdr/rk_aiq_ahdr_algo.h"
#include "ahdr/rk_aiq_types_ahdr_algo_prvt.h"



RKAIQ_BEGIN_DECLARE



static XCamReturn AhdrCreateCtx(RkAiqAlgoContext **context, const AlgoCtxInstanceCfg* cfg)
{

    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    RESULT ret = AHDR_RET_SUCCESS;
    RkAiqAlgoContext *ctx = new RkAiqAlgoContext();
    if (ctx == NULL) {
        LOGE_AHDR( "%s: create ahdr context fail!\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_MEM;
    }
    memset(&ctx->AhdrInstConfig, 0x00, sizeof(AhdrInstanceConfig_t));
    ret = AhdrInit(&ctx->AhdrInstConfig);

    if (ret != AHDR_RET_SUCCESS) {
        LOGE_AHDR("%s AHDRInit failed: %d", __FUNCTION__, ret);
        return(XCAM_RETURN_ERROR_FAILED);
    }

    *context = ctx;

    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return(XCAM_RETURN_NO_ERROR);
}

static XCamReturn AhdrDestroyCtx(RkAiqAlgoContext *context)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    RESULT ret = AHDR_RET_SUCCESS;


    if(context != NULL) {

        AhdrHandle_t pAhdrCtx = (AhdrHandle_t)context->AhdrInstConfig.hAhdr;
        ret = AhdrRelease(pAhdrCtx);
        if (ret != AHDR_RET_SUCCESS) {
            LOGE_AHDR("%s AecRelease failed: %d", __FUNCTION__, ret);
            return(XCAM_RETURN_ERROR_FAILED);
        }
        delete context;
        context = NULL;
    }

    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return(XCAM_RETURN_NO_ERROR);
}

static XCamReturn AhdrPrepare(RkAiqAlgoCom* params)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    RESULT ret = AHDR_RET_SUCCESS;

    AhdrHandle_t pAhdrCtx = params->ctx->AhdrInstConfig.hAhdr;
    RkAiqAlgoConfigAhdrInt* AhdrCfgParam = (RkAiqAlgoConfigAhdrInt*)params; //come from params in html
    const CamCalibDbContext_t* pCalibDb = AhdrCfgParam->rk_com.u.prepare.calib;
    pAhdrCtx->width = AhdrCfgParam->rawWidth;
    pAhdrCtx->height = AhdrCfgParam->rawHeight;

    if (AhdrCfgParam->working_mode < RK_AIQ_WORKING_MODE_ISP_HDR2)
        pAhdrCtx->FrameNumber = 1;
    else if (AhdrCfgParam->working_mode < RK_AIQ_WORKING_MODE_ISP_HDR3 &&
             AhdrCfgParam->working_mode >= RK_AIQ_WORKING_MODE_ISP_HDR2)
        pAhdrCtx->FrameNumber = 2;
    else
        pAhdrCtx->FrameNumber = 3;

    AhdrConfig(pAhdrCtx); //set default para
    memcpy(&pAhdrCtx->pCalibDB, &pCalibDb->ahdr, sizeof(CalibDb_Ahdr_Para_t));//load iq paras
    memcpy(&pAhdrCtx->hdrAttr.stTool, &pCalibDb->ahdr, sizeof(CalibDb_Ahdr_Para_t));//load iq paras to stTool

    if (ret != AHDR_RET_SUCCESS) {
        LOGE_AHDR("%s AHDRUpdateConfig failed: %d", __FUNCTION__, ret);
        return(XCAM_RETURN_ERROR_FAILED);
    }

    if(/* !params->u.prepare.reconfig*/true) {
        AhdrStop(pAhdrCtx); // stop firstly for re-preapre
        ret = AhdrStart(pAhdrCtx);
        if (ret != AHDR_RET_SUCCESS) {
            LOGE_AHDR("%s AHDRStart failed: %d", __FUNCTION__, ret);
            return(XCAM_RETURN_ERROR_FAILED);
        }
    }

    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return(XCAM_RETURN_NO_ERROR);
}

static XCamReturn AhdrPreProcess(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    RESULT ret = AHDR_RET_SUCCESS;

    AhdrHandle_t pAhdrCtx = inparams->ctx->AhdrInstConfig.hAhdr;
    RkAiqAlgoConfigAhdrInt* AhdrCfgParam = (RkAiqAlgoConfigAhdrInt*)inparams;
    const CamCalibDbContext_t* pCalibDb = AhdrCfgParam->rk_com.u.prepare.calib;

    // sence mode
    if (AhdrCfgParam->rk_com.u.proc.gray_mode)
        pAhdrCtx->sence_mode = AHDR_NIGHT;
    else if (pAhdrCtx->FrameNumber == 1)
        pAhdrCtx->sence_mode = AHDR_NORMAL;
    else
        pAhdrCtx->sence_mode = AHDR_HDR;

    LOGD_AHDR("%s:Current mode:%d\n", __FUNCTION__, pAhdrCtx->sence_mode);
    if(pAhdrCtx->hdrAttr.opMode == HDR_OpMode_Tool)
        AhdrSelectMode(pAhdrCtx, &pAhdrCtx->hdrAttr.stTool, pAhdrCtx->sence_mode);
    else
        AhdrSelectMode(pAhdrCtx, &pAhdrCtx->pCalibDB, pAhdrCtx->sence_mode);

    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return(XCAM_RETURN_NO_ERROR);
}

static XCamReturn AhdrProcess(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    RESULT ret = AHDR_RET_SUCCESS;

    AhdrHandle_t pAhdrCtx = (AhdrHandle_t)inparams->ctx->AhdrInstConfig.hAhdr;
    RkAiqAlgoProcAhdrInt* AhdrParams = (RkAiqAlgoProcAhdrInt*)inparams;
    RkAiqAlgoProcResAhdrInt* AhdrProcResParams = (RkAiqAlgoProcResAhdrInt*)outparams;
    // pAhdrCtx->frameCnt = inparams->frame_id;
    AhdrGetStats(pAhdrCtx, &AhdrParams->ispAhdrStats);

    RkAiqAlgoProcResAeInt* ae_proc_res_int =
        (RkAiqAlgoProcResAeInt*)(AhdrParams->rk_com.u.proc.proc_res_comb->ae_proc_res);

    if (ae_proc_res_int)
        AhdrGetSensorInfo(pAhdrCtx, ae_proc_res_int->ae_proc_res_rk);
    else {
        AecProcResult_t AeProcResult;
        LOGW_AHDR("%s: Ae Proc result is null!!!\n", __FUNCTION__);
        AhdrGetSensorInfo(pAhdrCtx, AeProcResult);
    }

    RkAiqAlgoPreResAeInt* ae_pre_res_int =
        (RkAiqAlgoPreResAeInt*)(AhdrParams->rk_com.u.proc.pre_res_comb->ae_pre_res);
    RkAiqAlgoPreResAfInt* af_pre_res_int =
        (RkAiqAlgoPreResAfInt*)(AhdrParams->rk_com.u.proc.pre_res_comb->af_pre_res);
    if (ae_pre_res_int && af_pre_res_int)
        AhdrUpdateConfig(pAhdrCtx,
                         ae_pre_res_int->ae_pre_res_rk,
                         af_pre_res_int->af_pre_result);
    else if (ae_pre_res_int) {
        af_preprocess_result_t AfPreResult;
        LOGW_AHDR("%s: af Pre result is null!!!\n", __FUNCTION__);
        AhdrUpdateConfig(pAhdrCtx,
                         ae_pre_res_int->ae_pre_res_rk,
                         AfPreResult);
    }
    else {
        AecPreResult_t AecHdrPreResult;
        af_preprocess_result_t AfPreResult;
        LOGW_AHDR("%s: ae/af Pre result is null!!!\n", __FUNCTION__);
        AhdrUpdateConfig(pAhdrCtx,
                         AecHdrPreResult,
                         AfPreResult);
    }
    AhdrProcessing(pAhdrCtx);

    pAhdrCtx->AhdrProcRes.LongFrameMode = pAhdrCtx->SensorInfo.LongFrmMode;
    AhdrProcResParams->AhdrProcRes.isHdrGlobalTmo = pAhdrCtx->AhdrProcRes.isHdrGlobalTmo;
    AhdrProcResParams->AhdrProcRes.bTmoEn = true;
    AhdrProcResParams->AhdrProcRes.isLinearTmo = pAhdrCtx->FrameNumber == 1;
    memcpy(&AhdrProcResParams->AhdrProcRes.MgeProcRes, &pAhdrCtx->AhdrProcRes.MgeProcRes, sizeof(MgeProcRes_t));
    memcpy(&AhdrProcResParams->AhdrProcRes.TmoProcRes, &pAhdrCtx->AhdrProcRes.TmoProcRes, sizeof(TmoProcRes_t));

    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return(XCAM_RETURN_NO_ERROR);
}

static XCamReturn AhdrPostProcess(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    LOG1_AHDR("%s:Enter!\n", __FUNCTION__);
    RESULT ret = AHDR_RET_SUCCESS;

    //TODO

    LOG1_AHDR("%s:Exit!\n", __FUNCTION__);
    return(XCAM_RETURN_NO_ERROR);
}

RkAiqAlgoDescription g_RkIspAlgoDescAhdr = {
    .common = {
        .version = RKISP_ALGO_AHDR_VERSION,
        .vendor  = RKISP_ALGO_AHDR_VENDOR,
        .description = RKISP_ALGO_AHDR_DESCRIPTION,
        .type    = RK_AIQ_ALGO_TYPE_AHDR,
        .id      = 0,
        .create_context  = AhdrCreateCtx,
        .destroy_context = AhdrDestroyCtx,
    },
    .prepare = AhdrPrepare,
    .pre_process = AhdrPreProcess,
    .processing = AhdrProcess,
    .post_process = AhdrPostProcess,
};

RKAIQ_END_DECLARE
