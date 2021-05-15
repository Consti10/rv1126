#include "rk_aiq_uapi_asharp_int.h"
#include "asharp/rk_aiq_types_asharp_algo_prvt.h"



XCamReturn
rk_aiq_uapi_asharp_SetAttrib(RkAiqAlgoContext *ctx,
                             rk_aiq_sharp_attrib_t *attr,
                             bool need_sync)
{

    AsharpContext_t* pAsharpCtx = (AsharpContext_t*)ctx;

    pAsharpCtx->eMode = attr->eMode;
    pAsharpCtx->stAuto = attr->stAuto;
    pAsharpCtx->stManual = attr->stManual;

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_uapi_asharp_GetAttrib(const RkAiqAlgoContext *ctx,
                             rk_aiq_sharp_attrib_t *attr)
{

    AsharpContext_t* pAsharpCtx = (AsharpContext_t*)ctx;

    attr->eMode = pAsharpCtx->eMode;
    memcpy(&attr->stAuto, &pAsharpCtx->stAuto, sizeof(Asharp_Auto_Attr_t));
    memcpy(&attr->stManual, &pAsharpCtx->stManual, sizeof(Asharp_Manual_Attr_t));

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_uapi_asharp_SetIQpara(RkAiqAlgoContext *ctx,
                             rk_aiq_sharp_IQpara_t *para,
                             bool need_sync)
{

    AsharpContext_t* pAsharpCtx = (AsharpContext_t*)ctx;

    if(para->module_bits & (1 << ASHARP_MODULE_SHARP)){
		pAsharpCtx->stSharpCalib = para->stSharpPara;
		pAsharpCtx->isIQParaUpdate = true;
    }	

	if(para->module_bits & (1 << ASHARP_MODULE_EDGEFILTER)){
		pAsharpCtx->stEdgeFltCalib = para->stEdgeFltPara;
		pAsharpCtx->isIQParaUpdate = true;
    }
	
    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_uapi_asharp_GetIQpara(const RkAiqAlgoContext *ctx,
                             rk_aiq_sharp_IQpara_t *para)
{

 	AsharpContext_t* pAsharpCtx = (AsharpContext_t*)ctx;

	para->stSharpPara = pAsharpCtx->stSharpCalib;
	para->stEdgeFltPara = pAsharpCtx->stEdgeFltCalib;
	
    return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_asharp_SetStrength(const RkAiqAlgoContext *ctx,
                             float fPercent)
{

    AsharpContext_t* pAsharpCtx = (AsharpContext_t*)ctx;
	float fMax = SHARP_MAX_STRENGTH_PERCENT;
	float fStrength = 1.0;

	
	if(fPercent <= 0.5){
		fStrength =  fPercent /0.5;
	}else{
		fStrength = (fPercent - 0.5)*(fMax - 1) * 2 + 1;
	}
	
	pAsharpCtx->fStrength = fStrength;
    
    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_uapi_asharp_GetStrength(const RkAiqAlgoContext *ctx,
                             float *pPercent)
{

    AsharpContext_t* pAsharpCtx = (AsharpContext_t*)ctx;
	float fMax = SHARP_MAX_STRENGTH_PERCENT;
	float fStrength = 1.0;
	
	fStrength = pAsharpCtx->fStrength;

	if(fStrength <= 1){
		*pPercent = fStrength * 0.5;
	}else{
		*pPercent = 2 * (fStrength - 1)/(fMax - 1) + 0.5;
	}
    
    return XCAM_RETURN_NO_ERROR;
}

