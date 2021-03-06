/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "camera_pu_control.h"
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>

#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "json-c/json.h"
#include <pthread.h>
#include "uvc_log.h"

#if DBSERVER_SUPPORT
#include "dbserver.h"


extern "C" int video_record_set_brightness(int brightness) {
   char *table = TABLE_IMAGE_ADJUSTMENT;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   json_object_object_add(js, "iBrightness", json_object_new_int(brightness));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_contrast(int contrast) {
   char *table = TABLE_IMAGE_ADJUSTMENT;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   json_object_object_add(js, "iContrast", json_object_new_int(contrast));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_hue(int hue) {
   char *table = TABLE_IMAGE_ADJUSTMENT;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   json_object_object_add(js, "iHue", json_object_new_int(hue));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_staturation(int staturation) {
   char *table = TABLE_IMAGE_ADJUSTMENT;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   json_object_object_add(js, "iSaturation", json_object_new_int(staturation));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_sharpness(int sharpness) {
   char *table = TABLE_IMAGE_ADJUSTMENT;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   json_object_object_add(js, "iSharpness", json_object_new_int(sharpness));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_gamma(int gamma) {
   char *table = TABLE_IMAGE_ADJUSTMENT;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   json_object_object_add(js, "iGamma", json_object_new_int(gamma));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_white_balance_temperature(int balance) {
   char *table = TABLE_IMAGE_WHITE_BLANCE;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   //[2800,6500]->[0,100]
   int bg_level = (balance - 2800)/((6500 - 2800)/100);
   LOG_INFO("iWhiteBalanceRed is %d",bg_level);
   json_object_object_add(js, "iWhiteBalanceRed", json_object_new_int(bg_level));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_white_balance_temperature_auto(int balance) {
   char *table = TABLE_IMAGE_WHITE_BLANCE;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   if (balance == 1)
     json_object_object_add(js, "sWhiteBlanceStyle", json_object_new_string("autoWhiteBalance"));
   else
     json_object_object_add(js, "sWhiteBlanceStyle", json_object_new_string("manualWhiteBalance"));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_gain(int gain) {
   char *table = TABLE_IMAGE_ADJUSTMENT;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   json_object_object_add(js, "iGain", json_object_new_int(gain));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_hue_auto(int hue_auto) {
   char *table = TABLE_IMAGE_ADJUSTMENT;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   json_object_object_add(js, "iHueAuto", json_object_new_int(hue_auto));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}

extern "C" int video_record_set_frequency_mode(int mode) {
   char *table = TABLE_IMAGE_VIDEO_ADJUSTMEN;
   struct json_object *js = NULL;
   js = json_object_new_object();
   if (NULL == js)
   {
        LOG_DEBUG("+++new json object failed.\n");
        return -1;
   }
   if (mode == 1)
     json_object_object_add(js, "sPowerLineFrequencyMode", json_object_new_string("PAL(50HZ)"));
   else
     json_object_object_add(js, "sPowerLineFrequencyMode", json_object_new_string("PAL(60HZ)"));
   dbserver_media_set(table, (char*)json_object_to_json_string(js), 0);
   json_object_put(js);
   return 0;
}
#endif

extern "C" void camera_pu_control_init(int type,int def,int min,int max)
{
    LOG_DEBUG("%s!\n", __func__);
}

extern "C" int camera_pu_control_get(int type, int def)
{
    LOG_DEBUG("%s!\n", __func__);
    return def;
}

extern "C" int camera_pu_control_set(int type, int value)
{
    LOG_DEBUG("%s! type is %d,value is %d\n", __func__,type,value);
    switch (type) {
#if DBSERVER_SUPPORT
        case UVC_PU_BRIGHTNESS_CONTROL:
            video_record_set_brightness(value);
            break;
        case UVC_PU_CONTRAST_CONTROL:
            video_record_set_contrast(value);
            break;
        case UVC_PU_HUE_CONTROL:
            video_record_set_hue(value);
            break;
        case UVC_PU_SATURATION_CONTROL:
            video_record_set_staturation(value);
            break;
        case UVC_PU_SHARPNESS_CONTROL:
            video_record_set_sharpness(value);
            break;
        case UVC_PU_GAMMA_CONTROL:
            //video_record_set_gamma(value);
            break;
        case UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL:
            video_record_set_white_balance_temperature(value);
            break;
        case UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL:
            video_record_set_white_balance_temperature_auto(value);
            break;
        case UVC_PU_GAIN_CONTROL:
            //video_record_set_gain(value);
            break;
        case UVC_PU_HUE_AUTO_CONTROL:
            //video_record_set_hue_auto(value);
            break;
        case UVC_PU_POWER_LINE_FREQUENCY_CONTROL:
            video_record_set_frequency_mode(value);
            break;
#endif
      default:
         LOG_DEBUG("====unknow pu cmd.\n");
         break;

    }
    return 0;
}

extern "C" int camera_pu_control_check(int deviceid)
{
    LOG_DEBUG("%s!\n", __func__);
    return 0;
}
