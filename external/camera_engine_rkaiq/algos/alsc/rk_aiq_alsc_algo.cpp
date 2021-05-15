/*
* rk_aiq_alsc_algo.cpp

* for rockchip v2.0.0
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
/* for rockchip v2.0.0*/

#include "alsc/rk_aiq_alsc_algo.h"
#include "xcam_log.h"
#include "interpolation.h"
RKAIQ_BEGIN_DECLARE

static XCamReturn illuminant_index_estimation(int light_num, const CalibDb_AlscCof_ill_t illAll[], float awbGain[2], int* illuminant_index)
{

    LOGI_ALSC( "%s: (enter)\n", __FUNCTION__);
    float minDist = 9999999;
    float dist[8];
    float nRG, nBG;
    *illuminant_index = 0;
    XCamReturn ret = XCAM_RETURN_ERROR_FAILED;
    for(int i = 0; i < light_num; i++)
    {
        nRG = awbGain[0];
        nBG = awbGain[1];
        dist[i] = sqrt((nRG - illAll[i].awbGain[0]) * (nRG -  illAll[i].awbGain[0])
                       + (nBG -  illAll[i].awbGain[1]) * (nBG -  illAll[i].awbGain[1]));
        if(dist[i] < minDist)
        {
            minDist = dist[i];
            *illuminant_index = i;
            ret = XCAM_RETURN_NO_ERROR;
        }
    }
    if(ret != XCAM_RETURN_NO_ERROR) {
        LOGE_ALSC("fail to estimate illuminant!!!\n");
    }

    LOGD_ALSC( "wbGain:%f,%f, estimation illuminant  is %s(%d) \n", awbGain[0], awbGain[1],
               illAll[*illuminant_index].illuName, *illuminant_index);

    LOGI_ALSC( "%s: (exit)\n", __FUNCTION__);
    return ret;
}


static XCamReturn AwbOrderLscProfilesByVignetting
(
    const CalibDb_LscTableProfile_t* pLscProfiles[],
    const int32_t   cnt
) {
    int32_t i, j;

    for (i = 0; i < (cnt - 1); ++i) {
        for (j = 0; j < (cnt - i - 1); ++j) {
            if (pLscProfiles[j]->vignetting < pLscProfiles[j + 1]->vignetting) {
                const CalibDb_LscTableProfile_t* temp   = pLscProfiles[j];
                pLscProfiles[j]         = pLscProfiles[j + 1];
                pLscProfiles[j + 1]       = temp;
            }
        }
    }

    return (XCAM_RETURN_NO_ERROR);
}

static XCamReturn VignSelectLscProfiles
(
    const float     fVignetting,
    int32_t         no_lsc,
    const CalibDb_LscTableProfile_t* pLscProfiles[],
    const CalibDb_LscTableProfile_t** pLscProfile1,
    const CalibDb_LscTableProfile_t** pLscProfile2
) {
    XCamReturn XCamReturn = XCAM_RETURN_NO_ERROR;

    if ((no_lsc == 0) || (pLscProfiles == NULL)
            || (pLscProfile1 == NULL) || (pLscProfile2 == NULL)) {
        return (XCAM_RETURN_ERROR_PARAM);
    }

    if (fVignetting >= pLscProfiles[0]->vignetting) {
        *pLscProfile1 = pLscProfiles[0];
        *pLscProfile2 = NULL;
        LOGD_ALSC( "select:%s \n", (*pLscProfile1)->name);
        XCamReturn = XCAM_RETURN_ERROR_OUTOFRANGE;
    } else {
        int32_t nLast = no_lsc - 1;
        if (fVignetting <= pLscProfiles[nLast]->vignetting) {
            *pLscProfile1 = pLscProfiles[nLast];
            *pLscProfile2 = NULL;
            LOGD_ALSC( "select:%s \n", (*pLscProfile1)->name);
            XCamReturn = XCAM_RETURN_ERROR_OUTOFRANGE;
        } else {
            uint16_t n = 0;

            /* find the segment */
            while ((fVignetting <= pLscProfiles[n]->vignetting) && (n <= nLast)) {
                n++;
            }

            if (n > 0)
                n--;

            *pLscProfile1 = pLscProfiles[n];
            *pLscProfile2 = pLscProfiles[n + 1];

        }
    }

    return (XCamReturn);
}



/******************************************************************************
 * InterpolateMatrices
 *****************************************************************************/
static XCamReturn VignInterpolateMatrices
(
    const float             fVignetting,
    const CalibDb_LscTableProfile_t*   pLscProfile1,
    const CalibDb_LscTableProfile_t*   pLscProfile2,
    CamLscMatrix_t*          pResMatrix
) {
    XCamReturn iXCamReturn = XCAM_RETURN_ERROR_PARAM;

    if ((pLscProfile1 != NULL) && (pLscProfile2 != NULL) && (pResMatrix != NULL)) {
        float fVigA = pLscProfile1->vignetting;
        float fVigB = pLscProfile2->vignetting;

        float f1 = (fVigB - fVignetting) / (fVigB - fVigA);
        /* FLOAT f2 = ( fVigB - fVignetting ) / ( fVigB - fVigA ); */
        float f2 = 1.0f - f1;
        LOGD_ALSC( "select:%s :%f  and %s :%f", pLscProfile1->name,
                   f1, pLscProfile2->name, f2);

        /* left shift 16 */
        uint32_t f1_ = (uint32_t)(f1 * 65536.0f);
        uint32_t f2_ = (uint32_t)(f2 * 65536.0f);

        int16_t i;

        uint32_t red;
        uint32_t greenr;
        uint32_t greenb;
        uint32_t blue;

        for (i = 0; i < (17 * 17); i++) {
            red     = (f1_ * (uint32_t)pLscProfile1->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[i])
                      + (f2_ * (uint32_t)pLscProfile2->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[i]);

            greenr  = (f1_ * (uint32_t)pLscProfile1->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[i])
                      + (f2_ * (uint32_t)pLscProfile2->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[i]);

            greenb  = (f1_ * (uint32_t)pLscProfile1->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[i])
                      + (f2_ * (uint32_t)pLscProfile2->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[i]);

            blue    = (f1_ * (uint32_t)pLscProfile1->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[i])
                      + (f2_ * (uint32_t)pLscProfile2->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[i]);

            /* with round up (add 65536/2 <=> 0.5) before right shift */
            pResMatrix->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[i]    = (uint16_t)((red + (65536 >> 1)) >> 16);
            pResMatrix->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[i] = (uint16_t)((greenr + (65536 >> 1)) >> 16);
            pResMatrix->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[i] = (uint16_t)((greenb + (65536 >> 1)) >> 16);
            pResMatrix->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[i]   = (uint16_t)((blue + (65536 >> 1)) >> 16);
        }

        iXCamReturn = XCAM_RETURN_NO_ERROR;
    }

    return (iXCamReturn);
}


/******************************************************************************
 * Damping
 *****************************************************************************/
static XCamReturn Damping
(
    const float     damp,               /**< damping coefficient */
    CamLscMatrix_t*  pMatrixUndamped,   /**< undamped new computed matrices */
    CamLscMatrix_t*  pMatrixDamped      /**< old matrices and XCamReturn */
) {
    XCamReturn XCamReturn = XCAM_RETURN_ERROR_PARAM;

    if ((pMatrixUndamped != NULL) && (pMatrixDamped != NULL)) {
        /* left shift 16 */
        uint32_t f1_ = (uint32_t)(damp * 65536.0f);
        uint32_t f2_ = (uint32_t)(65536U - f1_);

        int16_t i;

        uint32_t red;
        uint32_t greenr;
        uint32_t greenb;
        uint32_t blue;

        for (i = 0; i < (17 * 17); i++) {
            red     = (f1_ * (uint32_t)pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[i])
                      + (f2_ * (uint32_t)pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[i]);

            greenr  = (f1_ * (uint32_t)pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[i])
                      + (f2_ * (uint32_t)pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[i]);

            greenb  = (f1_ * (uint32_t)pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[i])
                      + (f2_ * (uint32_t)pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[i]);

            blue    = (f1_ * (uint32_t)pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[i])
                      + (f2_ * (uint32_t)pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[i]);

            /* with round up (add 65536/2 <=> 0.5) before right shift */
            pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[i]    = (uint16_t)((red    + (65536 >> 1)) >> 16);
            pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[i] = (uint16_t)((greenr + (65536 >> 1)) >> 16);
            pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[i] = (uint16_t)((greenb + (65536 >> 1)) >> 16);
            pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[i]   = (uint16_t)((blue   + (65536 >> 1)) >> 16);
        }
        LOGD_ALSC( "dampfactor:%f", damp);
        LOGD_ALSC( " undampedLscMatrix r[0:3]:%d,%d,%d,%d, gr[0:3]:%d,%d,%d,%d, gb[0:3]:%d,%d,%d,%d, b[0:3]:%d,%d,%d,%d\n",
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[0],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[1],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[2],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[3],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[0],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[1],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[2],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[3],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[0],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[1],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[2],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[3],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[0],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[1],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[2],
                   pMatrixUndamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[3]
                 );
        LOGD_ALSC( " dampedLscMatrix r[0:3]:%d,%d,%d,%d, gr[0:3]:%d,%d,%d,%d, gb[0:3]:%d,%d,%d,%d, b[0:3]:%d,%d,%d,%d\n",
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[0],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[1],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[2],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff[3],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[0],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[1],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[2],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff[3],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[0],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[1],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[2],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff[3],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[0],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[1],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[2],
                   pMatrixDamped->LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff[3]
                 );

        XCamReturn = XCAM_RETURN_NO_ERROR;
    }


    return (XCamReturn);
}

static XCamReturn CamCalibDbGetLscProfileByName(const CalibDb_Lsc_t *calibLsc, char* name, const CalibDb_LscTableProfile_t **pLscTableProfile)
{
    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_ERROR_FAILED;

    for(int i = 0; i < calibLsc->tableAllNum; i++) { //todo
        if(strcmp(calibLsc->tableAll[i].name, name) == 0) {
            *pLscTableProfile = &calibLsc->tableAll[i];
            ret = XCAM_RETURN_NO_ERROR;
            break;
        }
    }
    if(ret != XCAM_RETURN_NO_ERROR) {
        LOGD_ALSC("can't find %s in tableAll\n", name);
    }
    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);

    return ret;
}

static XCamReturn CamCalibDbGetLscResIdxByName(const CalibDb_Lsc_t *calibLsc, char* name, int *resIdx)
{
    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_ERROR_FAILED;

    for(int i = 0; i < calibLsc->aLscCof.lscResNum; i++) {
        if(strcmp(calibLsc->aLscCof.lscResName[i], name) == 0) {
            *resIdx = i;
            ret = XCAM_RETURN_NO_ERROR;
            break;
        }
    }
    if(ret != XCAM_RETURN_NO_ERROR) {
        LOGD_ALSC("can't find %s in lscResName\n", name);
    }
    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);

    return ret;
}

void UpdateDominateIlluList(List *l, int illu, int listMaxSize)
{
    illu_node_t *pCurNode;
    illu_node_t *pDelNode;
    int sizeList;
    if(listMaxSize == 0) {
        return;
    }
    pCurNode = (illu_node_t*)malloc(sizeof(illu_node_t));
    pCurNode->value = illu;
    ListPrepareItem(pCurNode);
    ListAddTail(l, pCurNode);
    sizeList = ListNoItems(l);
    if (sizeList > listMaxSize)
    {
        pDelNode = (illu_node_t *)ListRemoveHead(l);
        free(pDelNode);
    }
}

void StableIlluEstimation(List l, int listSize, int illuNum, float varianceLuma, float varianceLumaTh, bool awbConverged, int preIllu, int *newIllu)
{
    int sizeList = ListNoItems(&l);
    if(sizeList < listSize || listSize == 0) {
        return;
    }
    /*if( awbConverged) {
        *newIllu = preIllu;
        LOGD_ALSC("awb is converged , reserve the last illumination(%d) \n", preIllu );
        return;
    }*/
    /*if( varianceLuma <= varianceLumaTh) {
        *newIllu = preIllu;
        LOGD_ALSC("varianceLuma %f < varianceLumaTh %f , reserve the last illumination(%d) \n", varianceLuma,varianceLumaTh,preIllu );
        return;
    }*/
    List *pNextNode = ListHead(&l);
    illu_node_t *pL;
    int *illuSet = (int*)malloc(illuNum*sizeof(int));
    memset(illuSet, 0, illuNum*sizeof(int));
    while (NULL != pNextNode)
    {
        pL = (illu_node_t*)pNextNode;
        illuSet[pL->value]++;
        pNextNode = pNextNode->p_next;
    }
    int count2 = 0;
    int max_count = 0;
    for(int i=0; i<illuNum; i++){
        LOGV_ALSC("illu(%d), count(%d)\n", i,illuSet[i]);
        if(illuSet[i] > max_count){
            max_count = illuSet[i];
            *newIllu = i;
        }
    }
    free(illuSet);
    LOGD_ALSC("varianceLuma %f, varianceLumaTh %f final estmination illu is %d\n", varianceLuma,varianceLumaTh,*newIllu );

}

XCamReturn AlscAutoConfig
(
    alsc_handle_t hAlsc
) {

    LOGI_ALSC("%s: (enter) count:%d\n", __FUNCTION__, hAlsc->count);
    int caseIndex = hAlsc->alscRest.caseIndex;
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    const CalibDb_AlscCof_ill_t* pDomIlluProfile = NULL;
    const CalibDb_LscTableProfile_t* pLscProfile1 = NULL;
    const CalibDb_LscTableProfile_t* pLscProfile2 = NULL;

    if (hAlsc == NULL) {
        return XCAM_RETURN_ERROR_PARAM;
    }

    int dominateIlluProfileIdx;
    int resIdx;
    //1)
    int dominateIlluListSize = 15;//to do from xml;
    float varianceLumaTh = 0.006;//to do from xml;
    ret = illuminant_index_estimation(hAlsc->calibLsc->aLscCof.illuNum[caseIndex], hAlsc->calibLsc->aLscCof.illAll[caseIndex],
                                      hAlsc->alscSwInfo.awbGain, &dominateIlluProfileIdx);
    RETURN_RESULT_IF_DIFFERENT(ret, XCAM_RETURN_NO_ERROR);
    UpdateDominateIlluList(&hAlsc->alscRest.dominateIlluList, dominateIlluProfileIdx, dominateIlluListSize);
    StableIlluEstimation(hAlsc->alscRest.dominateIlluList, dominateIlluListSize, hAlsc->calibLsc->aLscCof.illuNum[caseIndex],
        hAlsc->alscSwInfo.varianceLuma ,varianceLumaTh, hAlsc->alscSwInfo.awbConverged,
        hAlsc->alscRest.dominateIlluProfileIdx, &dominateIlluProfileIdx);
    hAlsc->alscRest.dominateIlluProfileIdx = dominateIlluProfileIdx;

    // 2) cal resIdx
    ret = CamCalibDbGetLscResIdxByName( hAlsc->calibLsc, hAlsc->curResName, &resIdx );
    RETURN_RESULT_IF_DIFFERENT(ret, XCAM_RETURN_NO_ERROR);
    hAlsc->alscRest.resIdx = resIdx;
    // 3)
    float sensorGain =  hAlsc->alscSwInfo.sensorGain;
    float fVignetting;
    pDomIlluProfile = &hAlsc->calibLsc->aLscCof.illAll[caseIndex][dominateIlluProfileIdx];

    interpolation(pDomIlluProfile->vignettingCurve.pSensorGain,
                  pDomIlluProfile->vignettingCurve.pVignetting,
                  pDomIlluProfile->vignettingCurve.arraySize,
                  sensorGain, &fVignetting);
    hAlsc->alscRest.fVignetting =  fVignetting;
    //4)
    // CalibDb_LscTableProfile_t* pLscProfiles = hAlsc->pLscTableAll[resIdx][dominateIlluProfileIdx];
    ret = VignSelectLscProfiles(fVignetting, pDomIlluProfile->tableUsedNO, hAlsc->pLscTableAll[caseIndex][resIdx][dominateIlluProfileIdx],
                                &pLscProfile1, &pLscProfile2);
    if (ret == XCAM_RETURN_NO_ERROR) {
        if (pLscProfile1 && pLscProfile2)
            LOGD_ALSC("fVignetting: %f (%f .. %f)\n",  fVignetting, pLscProfile1->vignetting, pLscProfile2->vignetting);
        ret = VignInterpolateMatrices(fVignetting, pLscProfile1, pLscProfile2,
                                      &hAlsc->alscRest.undampedLscMatrixTable);
        if (ret != XCAM_RETURN_NO_ERROR) {
            return (ret);
        }
    } else if (ret == XCAM_RETURN_ERROR_OUTOFRANGE) {
        /* we don't need to interpolate */
        LOGD_ALSC("fVignetting: %f (%f)\n",  fVignetting, pLscProfile1->vignetting);
        memcpy(&hAlsc->alscRest.undampedLscMatrixTable, &pLscProfile1->LscMatrix[0], sizeof(CamLscMatrix_t));
    } else {
        return (ret);
    }
    // 5) . Damping
    ret = Damping((hAlsc->calibLsc->damp_enable && hAlsc->count > 1) ? hAlsc->alscSwInfo.awbIIRDampCoef : 0,
                  &hAlsc->alscRest.undampedLscMatrixTable, &hAlsc->alscRest.dampedLscMatrixTable);

    hAlsc->alscRest.pLscProfile1 = pLscProfile1;
    hAlsc->alscRest.pLscProfile2 = pLscProfile2;

    // 6 set to ic

    memcpy(hAlsc->lscHwConf.r_data_tbl, &hAlsc->alscRest.dampedLscMatrixTable.LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff,
           sizeof(hAlsc->alscRest.dampedLscMatrixTable.LscMatrix[CAM_4CH_COLOR_COMPONENT_RED].uCoeff));
    memcpy(hAlsc->lscHwConf.gr_data_tbl, &hAlsc->alscRest.dampedLscMatrixTable.LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff,
           sizeof(hAlsc->alscRest.dampedLscMatrixTable.LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENR].uCoeff));
    memcpy(hAlsc->lscHwConf.gb_data_tbl, &hAlsc->alscRest.dampedLscMatrixTable.LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff,
           sizeof(hAlsc->alscRest.dampedLscMatrixTable.LscMatrix[CAM_4CH_COLOR_COMPONENT_GREENB].uCoeff));
    memcpy(hAlsc->lscHwConf.b_data_tbl, &hAlsc->alscRest.dampedLscMatrixTable.LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff,
           sizeof(hAlsc->alscRest.dampedLscMatrixTable.LscMatrix[CAM_4CH_COLOR_COMPONENT_BLUE].uCoeff));

    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);

    return (ret);
}

XCamReturn AlscManualConfig
(
    alsc_handle_t hAlsc
) {
    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    memcpy(hAlsc->lscHwConf.r_data_tbl, hAlsc->mCurAtt.stManual.r_data_tbl,
           sizeof(hAlsc->mCurAtt.stManual.r_data_tbl));
    memcpy(hAlsc->lscHwConf.gr_data_tbl, hAlsc->mCurAtt.stManual.gr_data_tbl,
           sizeof(hAlsc->mCurAtt.stManual.gr_data_tbl));
    memcpy(hAlsc->lscHwConf.gb_data_tbl, hAlsc->mCurAtt.stManual.gb_data_tbl,
           sizeof(hAlsc->mCurAtt.stManual.gb_data_tbl));
    memcpy(hAlsc->lscHwConf.b_data_tbl, hAlsc->mCurAtt.stManual.b_data_tbl,
           sizeof(hAlsc->mCurAtt.stManual.b_data_tbl));

    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);
    return ret;

}

XCamReturn AlscConfig
(
    alsc_handle_t hAlsc
) {
    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    LOGD_ALSC("%s: updateAtt: %d\n", __FUNCTION__, hAlsc->updateAtt);
    hAlsc->alscRest.caseIndex = USED_FOR_CASE_NORMAL;
    if((hAlsc->alscSwInfo.grayMode == true && hAlsc->alscRest.caseIndex != USED_FOR_CASE_GRAY)||
        (hAlsc->alscSwInfo.grayMode == false && hAlsc->alscRest.caseIndex == USED_FOR_CASE_GRAY)){
        ClearList(&hAlsc->alscRest.dominateIlluList);
    }
    if(hAlsc->alscSwInfo.grayMode){
        hAlsc->alscRest.caseIndex = USED_FOR_CASE_GRAY;
    }
    if(hAlsc->updateAtt) {
        hAlsc->mCurAtt = hAlsc->mNewAtt;
    }
    LOGD_ALSC("%s: byPass: %d  mode:%d used for case: %d\n", __FUNCTION__,
        hAlsc->mCurAtt.byPass, hAlsc->mCurAtt.mode,hAlsc->alscRest.caseIndex);
    if(hAlsc->mCurAtt.byPass != true ) {
        hAlsc->lscHwConf.lsc_en = true;
        if(hAlsc->mCurAtt.mode == RK_AIQ_LSC_MODE_AUTO) {
            AlscAutoConfig(hAlsc);
        } else if(hAlsc->mCurAtt.mode == RK_AIQ_LSC_MODE_MANUAL) {
            AlscManualConfig(hAlsc);
        } else {
            LOGE_ALSC("%s: hAlsc->mCurAtt.mode(%d) is invalid \n", __FUNCTION__, hAlsc->mCurAtt.mode);
        }
        memcpy(hAlsc->mCurAtt.stManual.r_data_tbl, hAlsc->lscHwConf.r_data_tbl,
               sizeof(hAlsc->mCurAtt.stManual.r_data_tbl));
        memcpy(hAlsc->mCurAtt.stManual.gr_data_tbl, hAlsc->lscHwConf.gr_data_tbl,
               sizeof(hAlsc->mCurAtt.stManual.gr_data_tbl));
        memcpy(hAlsc->mCurAtt.stManual.gb_data_tbl, hAlsc->lscHwConf.gb_data_tbl,
               sizeof(hAlsc->mCurAtt.stManual.gb_data_tbl));
        memcpy(hAlsc->mCurAtt.stManual.b_data_tbl, hAlsc->lscHwConf.b_data_tbl,
               sizeof(hAlsc->mCurAtt.stManual.b_data_tbl));
    } else {
        hAlsc->lscHwConf.lsc_en = false;
    }
    hAlsc->count = ((hAlsc->count + 2) > (65536)) ? 2 : (hAlsc->count + 1);
    LOGD_ALSC( "set to ic LscMatrix r[0:3]:%d,%d,%d,%d, gr[0:3]:%d,%d,%d,%d, gb[0:3]:%d,%d,%d,%d, b[0:3]:%d,%d,%d,%d\n",
               hAlsc->lscHwConf.r_data_tbl[0],
               hAlsc->lscHwConf.r_data_tbl[1],
               hAlsc->lscHwConf.r_data_tbl[2],
               hAlsc->lscHwConf.r_data_tbl[3],
               hAlsc->lscHwConf.gr_data_tbl[0],
               hAlsc->lscHwConf.gr_data_tbl[1],
               hAlsc->lscHwConf.gr_data_tbl[2],
               hAlsc->lscHwConf.gr_data_tbl[3],
               hAlsc->lscHwConf.gb_data_tbl[0],
               hAlsc->lscHwConf.gb_data_tbl[1],
               hAlsc->lscHwConf.gb_data_tbl[2],
               hAlsc->lscHwConf.gb_data_tbl[3],
               hAlsc->lscHwConf.b_data_tbl[0],
               hAlsc->lscHwConf.b_data_tbl[1],
               hAlsc->lscHwConf.b_data_tbl[2],
               hAlsc->lscHwConf.b_data_tbl[3]
             );

    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);
    return ret;

}

XCamReturn AlscInit(alsc_handle_t *hAlsc, const CamCalibDbContext_t* calib)
{
    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    *hAlsc = (alsc_context_t*)malloc(sizeof(alsc_context_t));
    alsc_context_t* alsc_context = *hAlsc;
    memset(alsc_context, 0, sizeof(alsc_context_t));

    if(calib == NULL) {
        return XCAM_RETURN_ERROR_FAILED;
    }
    const CalibDb_Lsc_t *calib_lsc = &calib->lsc;
    alsc_context->calibLsc = calib_lsc;

    alsc_context->alscSwInfo.sensorGain = 1.0;
    alsc_context->alscSwInfo.awbIIRDampCoef = 0;
    alsc_context->alscSwInfo.grayMode = false;
    alsc_context->alscSwInfo.varianceLuma = 255;
    alsc_context->alscSwInfo.awbConverged = false;
    int caseIndex = USED_FOR_CASE_NORMAL;
    alsc_context->alscRest.caseIndex = caseIndex;
    bool lsFound;
    for(int i = 0; i < calib_lsc->aLscCof.illuNum[caseIndex]; i++) {
        if(strcmp(calib_lsc->aLscCof.illAll[caseIndex][i].illuName, calib->awb.stategy_cfg.lsForFirstFrame) == 0) {
            memcpy(alsc_context->alscSwInfo.awbGain, calib_lsc->aLscCof.illAll[caseIndex][i].awbGain,
                   sizeof(alsc_context->alscSwInfo.awbGain));
            lsFound = true;
            LOGD_ALSC("%s: alsc lsForFirstFrame:%s", __FUNCTION__, calib_lsc->aLscCof.illAll[caseIndex][i].illuName);
            break;
        }
    }
    if(calib_lsc->aLscCof.illuNum[caseIndex] > 0 && lsFound == false) {
        memcpy(alsc_context->alscSwInfo.awbGain, calib_lsc->aLscCof.illAll[caseIndex][0].awbGain,
               sizeof(alsc_context->alscSwInfo.awbGain));
        LOGD_ALSC("%s: alsc lsForFirstFrame:%s", __FUNCTION__, calib_lsc->aLscCof.illAll[caseIndex][0].illuName);
    }
    LOGI_ALSC("%s: alsc illunum:%d", __FUNCTION__, calib_lsc->aLscCof.illuNum[caseIndex]);

    // 1) gtet and reorder para
    //const CalibDb_Lsc_t *pAlscProfile = calib->lsc;
    for(int c=0;c<USED_FOR_CASE_MAX;c++){
        for(int i = 0; i < calib_lsc->aLscCof.illuNum[c]; i++) {
            for (int k = 0; k < calib_lsc->aLscCof.lscResNum; k++) {
                for (int j = 0; j < calib_lsc->aLscCof.illAll[c][i].tableUsedNO; j++) {
                    char name[LSC_PROFILE_NAME + LSC_RESOLUTION_NAME];
                    sprintf(name, "%s_%s", calib_lsc->aLscCof.lscResName[k],
                            calib_lsc->aLscCof.illAll[c][i].tableUsed[j]);
                    const CalibDb_LscTableProfile_t* pLscTableProfile = NULL;
                    // get a lsc-profile from database
                    ret = CamCalibDbGetLscProfileByName(calib_lsc, name, &pLscTableProfile);
                    RETURN_RESULT_IF_DIFFERENT(ret, XCAM_RETURN_NO_ERROR);
                    // store lsc-profile in pointer array
                    alsc_context->pLscTableAll[c][k][i][j] = pLscTableProfile;
                    LOGD_ALSC("LSC name  %s r[1:2]:%d,%d \n", name,
                              alsc_context->pLscTableAll[c][k][i][j]->LscMatrix[0].uCoeff[1],
                              alsc_context->pLscTableAll[c][k][i][j]->LscMatrix[0].uCoeff[2]);

                }
                // order lsc-profiles by vignetting
                ret = AwbOrderLscProfilesByVignetting(alsc_context->pLscTableAll[c][k][i],
                                                      calib_lsc->aLscCof.illAll[c][i].tableUsedNO);
                //RETURN_RESULT_IF_DIFFERENT(result, XCAM_RETURN_NO_ERROR);
            }

        }
    }
    alsc_context->count = 0;
    alsc_context->mCurAtt.byPass = !(calib_lsc->enable);
    alsc_context->mCurAtt.mode = RK_AIQ_LSC_MODE_AUTO;
    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);
    return(ret);
}

XCamReturn AlscRelease(alsc_handle_t hAlsc)
{
    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    ClearList(&hAlsc->alscRest.dominateIlluList);
    free(hAlsc);

    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);
    return(ret);

}

XCamReturn AlscPrepare(alsc_handle_t hAlsc)
{
    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_NO_ERROR;


    int resIdx;
    ret = CamCalibDbGetLscResIdxByName( hAlsc->calibLsc, hAlsc->curResName, &resIdx );
    RETURN_RESULT_IF_DIFFERENT(ret, XCAM_RETURN_NO_ERROR);
    hAlsc->alscRest.resIdx = resIdx;
    const CalibDb_LscTableProfile_t* pLscProfile1 = hAlsc->pLscTableAll[USED_FOR_CASE_NORMAL][resIdx][0][0];
    memcpy(&hAlsc->lscHwConf.x_grad_tbl[0], &pLscProfile1-> LscXGradTbl[0], sizeof(pLscProfile1-> LscXGradTbl));
    memcpy(&hAlsc->lscHwConf.y_grad_tbl[0], &pLscProfile1-> LscYGradTbl[0], sizeof(pLscProfile1-> LscYGradTbl));
    memcpy(&hAlsc->lscHwConf.x_size_tbl[0], &pLscProfile1-> LscXSizeTbl[0], sizeof(pLscProfile1-> LscXSizeTbl));
    memcpy(&hAlsc->lscHwConf.y_size_tbl[0], &pLscProfile1-> LscYSizeTbl[0], sizeof(pLscProfile1-> LscYSizeTbl));

    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);
    return ret;
}
XCamReturn AlscPreProc(alsc_handle_t hAlsc)
{

    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);
    return(ret);

}
XCamReturn AlscProcessing(alsc_handle_t hAlsc)
{
    LOGI_ALSC("%s: (enter)\n", __FUNCTION__);

    XCamReturn ret = XCAM_RETURN_NO_ERROR;


    LOGI_ALSC("%s: (exit)\n", __FUNCTION__);
    return(ret);
}




RKAIQ_END_DECLARE


