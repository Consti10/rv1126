#include "rk_aiq_uapi_anr_int.h"
#include "anr/rk_aiq_types_anr_algo_prvt.h"

#define NR_STRENGTH_MAX_PERCENT (50.0)
#define NR_LUMA_TF_STRENGTH_MAX_PERCENT NR_STRENGTH_MAX_PERCENT
#define NR_LUMA_SF_STRENGTH_MAX_PERCENT (100.0)
#define NR_CHROMA_TF_STRENGTH_MAX_PERCENT NR_STRENGTH_MAX_PERCENT
#define NR_CHROMA_SF_STRENGTH_MAX_PERCENT NR_STRENGTH_MAX_PERCENT
#define NR_RAWNR_SF_STRENGTH_MAX_PERCENT (80.0)

XCamReturn
rk_aiq_uapi_anr_SetAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_nr_attrib_t *attr,
                          bool need_sync)
{

    ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

    pAnrCtx->eMode = attr->eMode;
    pAnrCtx->stAuto = attr->stAuto;
    pAnrCtx->stManual = attr->stManual;

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_uapi_anr_GetAttrib(const RkAiqAlgoContext *ctx,
                          rk_aiq_nr_attrib_t *attr)
{

    ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

    attr->eMode = pAnrCtx->eMode;
    memcpy(&attr->stAuto, &pAnrCtx->stAuto, sizeof(ANR_Auto_Attr_t));
    memcpy(&attr->stManual, &pAnrCtx->stManual, sizeof(ANR_Manual_Attr_t));

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
rk_aiq_uapi_anr_SetIQPara(RkAiqAlgoContext *ctx,
                          rk_aiq_nr_IQPara_t *pPara,
                          bool need_sync)
{

    ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

	if(pPara->module_bits & (1 << ANR_MODULE_BAYERNR)){
		pAnrCtx->stBayernrCalib = pPara->stBayernrPara;
		pAnrCtx->isIQParaUpdate = true;
	}

	if(pPara->module_bits & (1 << ANR_MODULE_MFNR)){
		pAnrCtx->stMfnrCalib = pPara->stMfnrPara;
		pAnrCtx->isIQParaUpdate = true;
	}

	if(pPara->module_bits & (1 << ANR_MODULE_UVNR)){
		pAnrCtx->stUvnrCalib = pPara->stUvnrPara;
		pAnrCtx->isIQParaUpdate = true;
	}

	if(pPara->module_bits & (1 << ANR_MODULE_YNR)){
		pAnrCtx->stYnrCalib = pPara->stYnrPara;
		pAnrCtx->isIQParaUpdate = true;
	}

    return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_GetIQPara(RkAiqAlgoContext *ctx,
                          rk_aiq_nr_IQPara_t *pPara)
{

	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

	pPara->stBayernrPara = pAnrCtx->stBayernrCalib;
	pPara->stMfnrPara = pAnrCtx->stMfnrCalib;
	pPara->stUvnrPara = pAnrCtx->stUvnrCalib;
	pPara->stYnrPara = pAnrCtx->stYnrCalib;
	
    return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_SetLumaSFStrength(const RkAiqAlgoContext *ctx,
                          float fPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

	float fStrength = 1.0f;
	float fMax = NR_LUMA_SF_STRENGTH_MAX_PERCENT;

    
	if(fPercent <= 0.5){
		fStrength =  fPercent /0.5;
	}else{
		fStrength = (fPercent - 0.5)*(fMax - 1)*2 + 1;
	}

	if(fStrength > 1){
		pAnrCtx->fRawnr_SF_Strength = fStrength / 2.0;
		pAnrCtx->fLuma_SF_Strength = 1;
	}else{
		pAnrCtx->fRawnr_SF_Strength = fStrength;
		pAnrCtx->fLuma_SF_Strength = fStrength;
	}

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_SetLumaTFStrength(const RkAiqAlgoContext *ctx,
                          float fPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;
	
	float fStrength = 1.0;
	float fMax = NR_LUMA_TF_STRENGTH_MAX_PERCENT;

	if(fPercent <= 0.5){
		fStrength =  fPercent /0.5;
	}else{
		fStrength = (fPercent - 0.5)*(fMax - 1) * 2 + 1;
	}

	pAnrCtx->fLuma_TF_Strength = fStrength;

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_GetLumaSFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

	float fStrength = 1.0f;
	float fMax = NR_LUMA_SF_STRENGTH_MAX_PERCENT;

	
	fStrength = pAnrCtx->fLuma_SF_Strength;

	if(fStrength <= 1){
		*pPercent = fStrength * 0.5;
	}else{
		*pPercent = 2 * (fStrength - 1)/(fMax - 1) + 0.5;
	}

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_GetLumaTFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;
	
	float fStrength = 1.0;
	float fMax = NR_LUMA_TF_STRENGTH_MAX_PERCENT;

	fStrength = pAnrCtx->fLuma_TF_Strength;

	if(fStrength <= 1){
		*pPercent = fStrength * 0.5;
	}else{
		*pPercent = 2 * (fStrength - 1)/(fMax - 1) + 0.5;
	}

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_SetChromaSFStrength(const RkAiqAlgoContext *ctx,
                          float fPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

	float fStrength = 1.0f;
	float fMax = NR_CHROMA_SF_STRENGTH_MAX_PERCENT;

	if(fPercent <= 0.5){
		fStrength =  fPercent /0.5;
	}else{
		fStrength = (fPercent - 0.5)*(fMax - 1) * 2 + 1;
	}

	pAnrCtx->fChroma_SF_Strength = fStrength;

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_SetChromaTFStrength(const RkAiqAlgoContext *ctx,
                          float fPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;
	
	float fStrength = 1.0;
	float fMax = NR_CHROMA_TF_STRENGTH_MAX_PERCENT;

	if(fPercent <= 0.5){
		fStrength =  fPercent /0.5;
	}else{
		fStrength = (fPercent - 0.5)*(fMax - 1) * 2 + 1;
	}

	pAnrCtx->fChroma_TF_Strength = fStrength;

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_GetChromaSFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

	float fStrength = 1.0f;
	float fMax = NR_CHROMA_SF_STRENGTH_MAX_PERCENT;
	
	fStrength = pAnrCtx->fChroma_SF_Strength;
	
	
	if(fStrength <= 1){
		*pPercent = fStrength * 0.5;
	}else{
		*pPercent = 2 * (fStrength - 1)/(fMax - 1) + 0.5;
	}
	

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_GetChromaTFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;
	
	float fStrength = 1.0;
	float fMax = NR_CHROMA_TF_STRENGTH_MAX_PERCENT;

	fStrength = pAnrCtx->fChroma_TF_Strength;

	if(fStrength <= 1){
		*pPercent = fStrength * 0.5;
	}else{
		*pPercent = 2 * (fStrength - 1)/(fMax - 1) + 0.5;
	}

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_SetRawnrSFStrength(const RkAiqAlgoContext *ctx,
                          float fPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;
	
	float fStrength = 1.0;
	float fMax = NR_RAWNR_SF_STRENGTH_MAX_PERCENT;

	if(fPercent <= 0.5){
		fStrength =  fPercent /0.5;
	}else{
		fStrength = (fPercent - 0.5)*(fMax - 1) * 2 + 1;
	}

	pAnrCtx->fRawnr_SF_Strength = fStrength;

	return XCAM_RETURN_NO_ERROR;
}


XCamReturn
rk_aiq_uapi_anr_GetRawnrSFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent)
{
	ANRContext_t* pAnrCtx = (ANRContext_t*)ctx;

	float fStrength = 1.0f;
	float fMax = NR_RAWNR_SF_STRENGTH_MAX_PERCENT;
	
	fStrength = pAnrCtx->fRawnr_SF_Strength;
	
	
	if(fStrength <= 1){
		*pPercent = fStrength * 0.5;
	}else{
		*pPercent = 2 * (fStrength - 1)/(fMax - 1) + 0.5;
	}
	

	return XCAM_RETURN_NO_ERROR;
}


