/*
 * Copyright 2015 Rockchip Electronics Co. LTD
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
 */

#define MODULE_TAG "h265e_api"

#include <string.h>

#include "mpp_err.h"
#include "mpp_log.h"
#include "mpp_env.h"
#include "mpp_common.h"

#include "h265e_api.h"
#include "h265e_syntax.h"

#define H265E_DBG_FUNCTION          (0x00000001)
#define H265E_DBG_INPUT             (0x00000010)
#define H265E_DBG_OUTPUT            (0x00000020)

RK_U32 h265e_debug = 0;

#define h265e_dbg(flag, fmt, ...)   _mpp_dbg(h265e_debug, flag, fmt, ## __VA_ARGS__)
#define h265e_dbg_f(flag, fmt, ...) _mpp_dbg_f(h265e_debug, flag, fmt, ## __VA_ARGS__)

#define h265e_dbg_func(fmt, ...)    h265e_dbg_f(H265E_DBG_FUNCTION, fmt, ## __VA_ARGS__)
#define h265e_dbg_input(fmt, ...)   h265e_dbg(H265E_DBG_INPUT, fmt, ## __VA_ARGS__)
#define h265e_dbg_output(fmt, ...)  h265e_dbg(H265E_DBG_OUTPUT, fmt, ## __VA_ARGS__)

#define H265E_BIG_ENDIAN 1
#define H265E_LITTLE_ENDIAN 0

typedef struct {
    MppEncCfgSet    *cfg;

    H265eSyntax     syntax;
} H265eCtx;

static MPP_RET h265e_init(void *ctx, EncImplCfg *ctrlCfg)
{
    H265eCtx *p = (H265eCtx *)ctx;
    MppEncCodecCfg *codec = NULL;
    if (p == NULL) {
        mpp_err_f("error: p == NULL");
        return MPP_NOK;
    }

    mpp_assert(ctrlCfg->coding = MPP_VIDEO_CodingHEVC);
    p->cfg = ctrlCfg->cfg;
    mpp_env_get_u32("h265e_debug", &h265e_debug, 0);
    h265e_dbg_func("enter ctx %p\n", ctx);

    memset(&p->syntax, 0, sizeof(p->syntax));
    ctrlCfg->task_count = 1;

    /* set defualt value of codec */
    codec = &p->cfg->codec;
    codec->h265.intra_qp = 26;
    codec->h265.max_qp = 51;
    codec->h265.min_qp = 10;
    codec->h265.max_delta_qp = 10;
    codec->h265.const_intra_pred = 0;
    codec->h265.sao_enable = 1;
    codec->h265.gop_delta_qp = 0;
    codec->h265.intra_refresh_mode = 0;
    codec->h265.intra_refresh_arg = 0;
    codec->h265.independ_slice_mode = 0;
    codec->h265.independ_slice_arg = 0;
    codec->h265.depend_slice_mode = 0;
    codec->h265.depend_slice_arg = 0;

    h265e_dbg_func("leave ctx %p\n", ctx);
    (void)ctx;
    (void)ctrlCfg;
    return MPP_OK;
}

static MPP_RET h265e_deinit(void *ctx)
{
    H265eCtx *p = (H265eCtx *)ctx;
    if (p == NULL) {
        mpp_err_f("error: p == NULL");
        return MPP_NOK;
    }

    h265e_dbg_func("enter ctx %p\n", ctx);
    h265e_dbg_func("leave ctx %p\n", ctx);
    (void)ctx;
    return MPP_OK;
}

static MPP_RET h265e_encode(void *ctx, HalEncTask *task)
{
    H265eCtx *p = (H265eCtx *)ctx;
    H265eSyntax* syntax = NULL;
    if (p == NULL) {
        mpp_err_f("error: p == NULL");
        return MPP_NOK;
    }

    h265e_dbg_func("enter ctx %p\n", ctx);
    syntax = &p->syntax;

    task->valid = 1;
//    syntax->eos = task->eos;
    task->syntax.data = syntax;
    h265e_dbg_func("leave ctx %p\n", ctx);
    (void)ctx;
    (void)task;
    return MPP_OK;
}

static MPP_RET h265e_config(void *ctx, MpiCmd cmd, void *param)
{
    H265eCtx *p = (H265eCtx *)ctx;
    MPP_RET ret = MPP_NOK;
    (void)param;
    h265e_dbg_func("enter ctx %p, cmd = %d\n", ctx, cmd);
    switch (cmd) {
    case MPP_ENC_SET_IDR_FRAME : {
        p->syntax.idr_request++;
    } break;
    case MPP_ENC_SET_RC_CFG : {
        ret = MPP_OK;
    } break;
    default:
        mpp_err("No correspond cmd found, and can not config!");
        ret = MPP_NOK;
        break;
    }
    h265e_dbg_func("leave ctx %p\n", ctx);
    return ret;
}

const EncImplApi api_h265e_controller = {
    "h265e_control",
    MPP_VIDEO_CodingHEVC,
    sizeof(H265eCtx),
    0,
    h265e_init,
    h265e_deinit,
    h265e_config,
    NULL,
    NULL,
    NULL,
    h265e_encode,
    NULL,
    NULL,
};
