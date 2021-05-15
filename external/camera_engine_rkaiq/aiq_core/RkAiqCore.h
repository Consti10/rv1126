/*
 * rkisp_aiq_core.h
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

#ifndef _RK_AIQ_CORE_H_
#define _RK_AIQ_CORE_H_

#include "rk_aiq_types.h"
#include "rk_aiq_algo_types_int.h"
#include "RkAiqCalibDb.h"
#include "RkAiqCalibDbTypes.h"
#include "RkAiqHandle.h"
#include "xcam_thread.h"
#include "smartptr.h"
#include "safe_list.h"
#include "xcam_log.h"
#include "video_buffer.h"
#include "rk_aiq_pool.h"

using namespace XCam;
namespace RkCam {

#define RKAIQCORE_CHECK_RET(ret, format, ...) \
    do { \
        if (ret < 0) { \
            LOGE_ANALYZER(format, ##__VA_ARGS__); \
            return ret; \
        } else if (ret == XCAM_RETURN_BYPASS) { \
            LOGW_ANALYZER("bypass !", __FUNCTION__, __LINE__); \
            return ret; \
        } \
    } while (0)

#define RKAIQCORE_CHECK_RET_NULL(ret, format, ...) \
    do { \
        if (ret < 0) { \
            LOGE_ANALYZER(format, ##__VA_ARGS__); \
            return NULL; \
        } else if (ret == XCAM_RETURN_BYPASS) { \
            LOGW_ANALYZER("bypass !", __FUNCTION__, __LINE__); \
            return NULL; \
        } \
    } while (0)

#define RKAIQCORE_CHECK_BYPASS(ret, format, ...) \
    do { \
        if (ret < 0) { \
            LOGE_ANALYZER(format, ##__VA_ARGS__); \
            return ret; \
        } else if (ret == XCAM_RETURN_BYPASS) { \
            LOGW_ANALYZER("bypass !", __FUNCTION__, __LINE__); \
            ret = XCAM_RETURN_NO_ERROR; \
        } \
    } while (0)

class RkAiqCore;
class RkAiqIspStats {
public:
    explicit RkAiqIspStats() {
        xcam_mem_clear(aec_stats);
        xcam_mem_clear(awb_stats);
        xcam_mem_clear(awb_stats_v201);
        xcam_mem_clear(awb_cfg_effect_v200);
        xcam_mem_clear(awb_cfg_effect_v201);
        xcam_mem_clear(af_stats);
        aec_stats_valid = false;
        awb_stats_valid = false;
        awb_cfg_effect_valid = false;
        af_stats_valid = false;
        frame_id = -1;
    };
    ~RkAiqIspStats() {};
    rk_aiq_isp_aec_stats_t aec_stats;
    bool aec_stats_valid;
    rk_aiq_awb_stat_res_v200_t awb_stats;
    rk_aiq_awb_stat_cfg_v200_t  awb_cfg_effect_v200;
    rk_aiq_awb_stat_res_v201_t awb_stats_v201;
    rk_aiq_awb_stat_cfg_v201_t  awb_cfg_effect_v201;

    bool awb_stats_valid;
    bool awb_cfg_effect_valid;
    rk_aiq_isp_af_stats_t af_stats;
    bool af_stats_valid;
    rk_aiq_isp_orb_stats_t orb_stats;
    bool orb_stats_valid;
    rkisp_ahdr_stats_t ahdr_stats;
    bool ahdr_stats_valid;
    uint32_t frame_id;
private:
    XCAM_DEAD_COPY (RkAiqIspStats);
};

class RkAiqAnalyzerCb {
public:
    explicit RkAiqAnalyzerCb() {};
    virtual ~RkAiqAnalyzerCb() {};
    virtual void rkAiqCalcDone(SmartPtr<RkAiqFullParamsProxy> &results) = 0;
    virtual void rkAiqCalcFailed(const char* msg) = 0;
private:
    XCAM_DEAD_COPY (RkAiqAnalyzerCb);
};

class RkAiqCoreThread
    : public Thread {
public:
    RkAiqCoreThread(RkAiqCore* rkAiqCore)
        : Thread("RkAiqCoreThread")
        , mRkAiqCore(rkAiqCore) {};
    ~RkAiqCoreThread() {
        mStatsQueue.clear ();
    };

    void triger_stop() {
        mStatsQueue.pause_pop ();
    };

    void triger_start() {
        mStatsQueue.clear ();
        mStatsQueue.resume_pop ();
    };

    bool push_stats (const SmartPtr<VideoBuffer> &buffer) {
        mStatsQueue.push (buffer);
        return true;
    };

protected:
    //virtual bool started ();
    virtual void stopped () {
        mStatsQueue.clear ();
    };
    virtual bool loop ();
private:
    RkAiqCore* mRkAiqCore;
    SafeList<VideoBuffer> mStatsQueue;
};

struct RkAiqHwInfo {
    bool fl_supported;   // led flash
    bool fl_strth_adj;   // led streng_adjust
    bool irc_supported;  // ir flash & ir cutter
    bool fl_ir_strth_adj;   // ir streng_adjust
    bool lens_supported;
};

class RkAiqCore {
    friend class RkAiqCoreThread;
public:
    RkAiqCore();
    virtual ~RkAiqCore();

    bool setAnalyzeResultCb(RkAiqAnalyzerCb* callback) {
        mCb = callback;
        return true;
    }
    // called only once
    XCamReturn init(const char* sns_ent_name, const CamCalibDbContext_t* aiqCalib);
    // called only once
    XCamReturn deInit();
    // start analyze thread
    XCamReturn start();
    // stop analyze thread
    XCamReturn stop();
    // called before start(), get initial settings
    XCamReturn prepare(const rk_aiq_exposure_sensor_descriptor* sensor_des,
                       int mode);
    // could be called after prepare() or start()
    // if called between prepare() start(), can get
    // initial settings
    SmartPtr<RkAiqFullParamsProxy>& getAiqFullParams() {
        return mAiqCurParams;
    };
    XCamReturn pushStats(SmartPtr<VideoBuffer> &buffer);
    XCamReturn addAlgo(RkAiqAlgoDesComm& algo);
    XCamReturn enableAlgo(int algoType, int id, bool enable);
    XCamReturn rmAlgo(int algoType, int id);
    bool getAxlibStatus(int algoType, int id);
    const RkAiqAlgoContext* getEnabledAxlibCtx(const int algo_type);
    const RkAiqHandle* getAiqAlgoHandle(const int algo_type);
    XCamReturn get3AStatsFromCachedList(rk_aiq_isp_stats_t &stats);
    XCamReturn get3AStatsFromCachedList(rk_aiq_isp_stats_t **stats, int timeout_ms);
    void release3AStatsRef(rk_aiq_isp_stats_t *stats);
    XCamReturn setCpsLtCfg(rk_aiq_cpsl_cfg_t &cfg);
    XCamReturn getCpsLtInfo(rk_aiq_cpsl_info_t &info);
    XCamReturn queryCpsLtCap(rk_aiq_cpsl_cap_t &cap);
    XCamReturn setHwInfos(struct RkAiqHwInfo &hw_info);
    XCamReturn setGrayMode(rk_aiq_gray_mode_t mode);
    rk_aiq_gray_mode_t getGrayMode();
    void setSensorFlip(bool mirror, bool flip);
    void setResrcPath(const char* rp) {
        if (mAlogsSharedParams.resourcePath) {
            xcam_free((void*)(mAlogsSharedParams.resourcePath));
            mAlogsSharedParams.resourcePath = NULL;
        }
        if (rp)
            mAlogsSharedParams.resourcePath = strdup(rp);
    };
    bool isRunningState() {
        return mState == RK_AIQ_CORE_STATE_RUNNING;
    }
    void setShareMemOps(isp_drv_share_mem_ops_t *mem_ops) {
        mShareMemOps = mem_ops;
    }

public:
    // following vars shared by all algo handlers
    typedef struct RkAiqAlgosShared_s {
        RkAiqIspStats ispStats;
        RkAiqPreResComb preResComb;
        RkAiqProcResComb procResComb;
        RkAiqPostResComb postResComb;
        const CamCalibDbContext_t* calib;
        rk_aiq_exposure_sensor_descriptor snsDes;
        uint32_t frameId;
        int working_mode;
        bool fill_light_on;
        bool gray_mode;
        bool init;
        bool reConfig;
        bool is_bw_sensor;
        uint32_t hardware_version;
        int iso;
        AlgoCtxInstanceCfgInt ctxCfigs[RK_AIQ_ALGO_TYPE_MAX];
        rk_aiq_cpsl_cfg_t cpslCfg;
        const char* resourcePath;
        bool sns_mirror;
        bool sns_flip;
        void reset() {
            xcam_mem_clear(preResComb);
            xcam_mem_clear(procResComb);
            xcam_mem_clear(postResComb);
            xcam_mem_clear(ctxCfigs);
            xcam_mem_clear(cpslCfg);
            calib = NULL;
            frameId = -1;
            working_mode = 0;
            init = false;
            reConfig = false;
            hardware_version = 0;
            iso = 0;
            fill_light_on = false;
            gray_mode = false;
            is_bw_sensor = false;
            resourcePath = NULL;
            sns_mirror = false;
            sns_flip = false;
        }
    } RkAiqAlgosShared_t;
    RkAiqAlgosShared_t mAlogsSharedParams;
    isp_drv_share_mem_ops_t *mShareMemOps;
private:
    // in analyzer thread
    XCamReturn analyze(const SmartPtr<VideoBuffer> &buffer);
    SmartPtr<RkAiqFullParamsProxy> analyzeInternal();
    SmartPtr<RkAiqFullParamsProxy> analyzeInternalPp();
    XCamReturn preProcess();
    XCamReturn preProcessPp();
    XCamReturn processing();
    XCamReturn processingPp();
    XCamReturn postProcess();
    XCamReturn postProcessPp();
    XCamReturn convertIspstatsToAlgo(const SmartPtr<VideoBuffer> &buffer);
    SmartPtr<RkAiqHandle>* getCurAlgoTypeHandle(int algo_type);
    std::map<int, SmartPtr<RkAiqHandle>>* getAlgoTypeHandleMap(int algo_type);
    SmartPtr<RkAiqHandle> newAlgoHandle(RkAiqAlgoDesComm* algo);
    void addDefaultAlgos();
    XCamReturn genIspAeResult(RkAiqFullParams* params);
    XCamReturn genIspAwbResult(RkAiqFullParams* params);
    XCamReturn genIspAfResult(RkAiqFullParams* params);
    XCamReturn genIspAhdrResult(RkAiqFullParams* params);
    XCamReturn genIspAnrResult(RkAiqFullParams* params);
    XCamReturn genIspAdhazResult(RkAiqFullParams* params);
    XCamReturn genIspAsdResult(RkAiqFullParams* params);
    XCamReturn genIspAcpResult(RkAiqFullParams* params);
    XCamReturn genIspAieResult(RkAiqFullParams* params);
    XCamReturn genIspAsharpResult(RkAiqFullParams* params);
    XCamReturn genIspA3dlutResult(RkAiqFullParams* params);
    XCamReturn genIspAblcResult(RkAiqFullParams* params);
    XCamReturn genIspAccmResult(RkAiqFullParams* params);
    XCamReturn genIspAcgcResult(RkAiqFullParams* params);
    XCamReturn genIspAdebayerResult(RkAiqFullParams* params);
    XCamReturn genIspAdpccResult(RkAiqFullParams* params);
    XCamReturn genIspAfecResult(RkAiqFullParams* params);
    XCamReturn genIspAgammaResult(RkAiqFullParams* params);
    XCamReturn genIspAgicResult(RkAiqFullParams* params);
    XCamReturn genIspAldchResult(RkAiqFullParams* params);
    XCamReturn genIspAlscResult(RkAiqFullParams* params);
    XCamReturn genIspAorbResult(RkAiqFullParams* params);
    XCamReturn genIspAr2yResult(RkAiqFullParams* params);
    XCamReturn genIspAwdrResult(RkAiqFullParams* params);
    XCamReturn genCpslResult(RkAiqFullParams* params);
    void cacheIspStatsToList();
    void initCpsl();

private:
    enum rk_aiq_core_state_e {
        RK_AIQ_CORE_STATE_INVALID,
        RK_AIQ_CORE_STATE_INITED,
        RK_AIQ_CORE_STATE_PREPARED,
        RK_AIQ_CORE_STATE_STARTED,
        RK_AIQ_CORE_STATE_RUNNING,
        RK_AIQ_CORE_STATE_STOPED,
    };
    SmartPtr<RkAiqCoreThread> mRkAiqCoreTh;
    SmartPtr<RkAiqCoreThread> mRkAiqCorePpTh;
    int mState;
    RkAiqAnalyzerCb* mCb;
    std::map<int, SmartPtr<RkAiqHandle>> mAeAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAwbAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAfAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAhdrAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAnrAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAdhazAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAsdAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAcpAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAsharpAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mA3dlutAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAblcAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAccmAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAcgcAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAdebayerAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAdpccAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAfecAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAgammaAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAgicAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAieAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAldchAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAlscAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAorbAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAr2yAlgoHandleMap;
    std::map<int, SmartPtr<RkAiqHandle>> mAwdrAlgoHandleMap;
    SmartPtr<RkAiqHandle> mCurAhdrAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAnrAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAdhazAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAsdAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAcpAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAsharpAlgoHdl;
    SmartPtr<RkAiqHandle> mCurA3dlutAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAblcAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAccmAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAcgcAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAdebayerAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAdpccAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAfecAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAgammaAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAgicAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAieAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAldchAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAlscAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAorbAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAr2yAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAwdrAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAeAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAwbAlgoHdl;
    SmartPtr<RkAiqHandle> mCurAfAlgoHdl;
    SmartPtr<RkAiqFullParamsPool> mAiqParamsPool;
    SmartPtr<RkAiqFullParamsProxy> mAiqCurParams;
    SmartPtr<RkAiqExpParamsPool> mAiqExpParamsPool;
    SmartPtr<RkAiqIrisParamsPool> mAiqIrisParamsPool;
    SmartPtr<RkAiqIspParamsPool> mAiqIspParamsPool;
    SmartPtr<RkAiqIsppParamsPool> mAiqIsppParamsPool;
    SmartPtr<RkAiqFocusParamsPool> mAiqFocusParamsPool;
    SmartPtr<RkAiqCpslParamsPool> mAiqCpslParamsPool;
    static uint16_t DEFAULT_POOL_SIZE;
    XCam::Cond mIspStatsCond;
    Mutex ispStatsListMutex;
    struct RkAiqHwInfo mHwInfo;
    rk_aiq_cpsl_cap_t mCpslCap;
    bool mCurCpslOn;
    float mStrthLed;
    float mStrthIr;
    rk_aiq_gray_mode_t mGrayMode;
    bool firstStatsReceived;
    typedef SharedItemPool<rk_aiq_isp_stats_t> RkAiqStatsPool;
    typedef SharedItemProxy<rk_aiq_isp_stats_t> RkAiqStatsProxy;
    SmartPtr<RkAiqStatsPool> mAiqStatsPool;
    std::list<SmartPtr<RkAiqStatsProxy>> mAiqStatsCachedList;
    std::map<rk_aiq_isp_stats_t*, SmartPtr<RkAiqStatsProxy>> mAiqStatsOutMap;
};

};

#endif //_RK_AIQ_CORE_H_
