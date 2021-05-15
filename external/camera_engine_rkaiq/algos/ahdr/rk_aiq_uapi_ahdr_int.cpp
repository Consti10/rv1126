#include "rk_aiq_uapi_ahdr_int.h"
#include "rk_aiq_types_ahdr_algo_prvt.h"


XCamReturn
rk_aiq_uapi_ahdr_SetAttrib
(
    RkAiqAlgoContext* ctx,
    ahdr_attrib_t attr,
    bool need_sync
)
{
    if(ctx == NULL) {
        LOGE_AHDR("%s(%d): null pointer\n", __FUNCTION__, __LINE__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    AhdrContext_t* pAhdrCtx = (AhdrContext_t*)(ctx->AhdrInstConfig.hAhdr);

    //Todo
    pAhdrCtx->hdrAttr.opMode = attr.opMode;
    memcpy(&pAhdrCtx->hdrAttr.stSetLevel, &attr.stSetLevel, sizeof(FastMode_t));
    memcpy(&pAhdrCtx->hdrAttr.stDarkArea, &attr.stDarkArea, sizeof(DarkArea_t));
    memcpy(&pAhdrCtx->hdrAttr.stTool, &attr.stTool, sizeof(CalibDb_Ahdr_Para_t));

    if (need_sync == true)
    {

        if (0 != memcmp(&pAhdrCtx->hdrAttr.stAuto.stMgeAuto, &attr.stAuto.stMgeAuto, sizeof(amgeAttr_t))) {
            memcpy(&pAhdrCtx->hdrAttr.stAuto.stMgeAuto, &attr.stAuto.stMgeAuto, sizeof(amgeAttr_t));
            pAhdrCtx->hdrAttr.stAuto.bUpdateMge = true;
        }
        else
            pAhdrCtx->hdrAttr.stAuto.bUpdateMge = false;

        if (0 != memcmp(&pAhdrCtx->hdrAttr.stAuto.stTmoAuto, &attr.stAuto.stTmoAuto, sizeof(atmoAttr_t))) {
            memcpy(&pAhdrCtx->hdrAttr.stAuto.stTmoAuto, &attr.stAuto.stTmoAuto, sizeof(atmoAttr_t));
            pAhdrCtx->hdrAttr.stAuto.bUpdateTmo = true;
        }
        else
            pAhdrCtx->hdrAttr.stAuto.bUpdateTmo = false;


        if (0 != memcmp(&pAhdrCtx->hdrAttr.stManual.stMgeManual, &attr.stManual.stMgeManual, sizeof(mmgeAttr_t))) {
            memcpy(&pAhdrCtx->hdrAttr.stManual.stMgeManual, &attr.stManual.stMgeManual, sizeof(mmgeAttr_t));
            pAhdrCtx->hdrAttr.stManual.bUpdateMge = true;
        }
        else
            pAhdrCtx->hdrAttr.stManual.bUpdateMge = false;

        if (0 != memcmp(&pAhdrCtx->hdrAttr.stManual.stTmoManual, &attr.stManual.stTmoManual, sizeof(mtmoAttr_t))) {
            memcpy(&pAhdrCtx->hdrAttr.stManual.stTmoManual, &attr.stManual.stTmoManual, sizeof(mtmoAttr_t));
            pAhdrCtx->hdrAttr.stManual.bUpdateTmo = true;
        }
        else
            pAhdrCtx->hdrAttr.stManual.bUpdateTmo = false;

    }
    else
    {
        pAhdrCtx->hdrAttr.stAuto.bUpdateMge = need_sync;
        pAhdrCtx->hdrAttr.stAuto.bUpdateTmo = need_sync;
        pAhdrCtx->hdrAttr.stManual.bUpdateMge = need_sync;
        pAhdrCtx->hdrAttr.stManual.bUpdateTmo = need_sync;
    }

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_uapi_ahdr_GetAttrib
(
    RkAiqAlgoContext*  ctx,
    ahdr_attrib_t* attr
)
{
    if(ctx == NULL || attr == NULL) {
        LOGE_AHDR("%s(%d): null pointer\n", __FUNCTION__, __LINE__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    AhdrContext_t* pAhdrCtx = (AhdrContext_t*)ctx->AhdrInstConfig.hAhdr;

    attr->opMode = pAhdrCtx->hdrAttr.opMode;

    memcpy(&attr->stAuto.stMgeAuto, &pAhdrCtx->hdrAttr.stAuto.stMgeAuto, sizeof(amgeAttr_t));
    memcpy(&attr->stAuto.stTmoAuto, &pAhdrCtx->hdrAttr.stAuto.stTmoAuto, sizeof(atmoAttr_t));
    memcpy(&attr->stManual.stMgeManual, &pAhdrCtx->hdrAttr.stManual.stMgeManual, sizeof(mmgeAttr_t));
    memcpy(&attr->stManual.stTmoManual, &pAhdrCtx->hdrAttr.stManual.stTmoManual, sizeof(mtmoAttr_t));
    memcpy(&attr->stSetLevel, &pAhdrCtx->hdrAttr.stSetLevel, sizeof(FastMode_t));
    memcpy(&attr->stDarkArea, &pAhdrCtx->hdrAttr.stDarkArea, sizeof(DarkArea_t));
    memcpy(&attr->stTool, &pAhdrCtx->hdrAttr.stTool, sizeof(CalibDb_Ahdr_Para_t));
    memcpy(&attr->CtlInfo, &pAhdrCtx->hdrAttr.CtlInfo, sizeof(CurrCtlData_t));
    memcpy(&attr->RegInfo, &pAhdrCtx->hdrAttr.RegInfo, sizeof(CurrRegData_t));

    return XCAM_RETURN_NO_ERROR;
}


