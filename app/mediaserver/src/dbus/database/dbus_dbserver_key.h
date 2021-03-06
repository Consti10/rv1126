// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _RK_DBUS_MEDIASERVER_KEY_H_
#define _RK_DBUS_MEDIASERVER_KEY_H_

namespace rockchip {
namespace mediaserver {

#define DB_MEDIA_TABLE "table"
#define DB_MEDIA_VIDEO_TABLE "video"
#define DB_MEDIA_VIDEO_ADVANCED_ENC_TABLE "video_advanced_enc"
#define DB_MEDIA_AUDIO_TABLE "audio"
#define DB_MEDIA_OSD_TABLE "osd"
#define DB_MEDIA_ROI_TABLE "roi"
#define DB_MEDIA_REGION_INVADE_TABLE "RegionalInvasion"
#define DB_MEDIA_MOVE_DETECTION_TABLE "MoveDetection"
#define DB_MEDIA_SMART_COVER_TABLE "SmartCover"

#define DB_MEDIA_TABLE_ID "id"
#define DB_MEDIA_TABLE_KEY "key"
#define DB_MEDIA_TABLE_DATA "jData"
#define DB_MEDIA_TABLE_DATACHANGE "data"

#define DB_AUDIO_SAMPLE_RATE "iSampleRate"
#define DB_AUDIO_VOLUME "iVolume"
#define DB_AUDIO_BIT_RATE "iBitRate"
#define DB_AUDIO_SOURCE "sInput"
#define DB_AUDIO_ENCODE_TYPE "sEncodeType"
#define DB_AUDIO_ANS "sANS"

#define DB_VIDEO_GOP "iGOP"
#define DB_VIDEO_MAX_RATE "iMaxRate"
#define DB_VIDEO_BITRATE "iTargetRate"
#define DB_VIDEO_MIN_RATE "iMinRate"
#define DB_VIDEO_STREAM_SMOOTH "iStreamSmooth"
#define DB_VIDEO_FRAME_RATE "sFrameRate"
#define DB_VIDEO_FRAME_RATE_IN "sFrameRateIn"
#define DB_VIDEO_RESOLUTION "sResolution"
#define DB_VIDEO_RC_QUALITY "sRCQuality"
#define DB_VIDEO_OUTPUT_DATA_TYPE "sOutputDataType"
#define DB_VIDEO_RC_MODE "sRCMode"
#define DB_VIDEO_H264_PROFILE "sH264Profile"
#define DB_VIDEO_SMART "sSmart"
#define DB_VIDEO_STREAM_TYPE "sStreamType"
#define DB_VIDEO_SVC "sSVC"
#define DB_VIDEO_TYPE "sVideoType"
#define DB_VIDEO_MAIN_STREAM "mainStream"
#define DB_VIDEO_SUB_STREAM "subStream"
#define DB_VIDEO_THIRD_STREAM "thirdStream"

#define DB_OSD_ID "id"
#define DB_OSD_TYPE "sType"
#define DB_OSD_TYPE_DATE "dateTime"
#define DB_OSD_TYPE_CHANNLE "channelName"
#define DB_OSD_TYPE_TEXT "character"
#define DB_OSD_TYPE_MASK "privacyMask"
#define DB_OSD_TYPE_IMAGE "image"
#define DB_OSD_ENABLED "iEnabled"
#define DB_OSD_POSITION_X "iPositionX"
#define DB_OSD_POSITION_Y "iPositionY"
#define DB_OSD_WIDTH "iWidth"
#define DB_OSD_HTIGHT "iHeight"
#define DB_OSD_IS_PERSISTENT_TEXT "sIsPersistentText"
#define DB_OSD_DISPLAY_TEXT "sDisplayText"
#define DB_OSD_DISPLAY_WEEK_ENABLED "iDisplayWeekEnabled"
#define DB_OSD_DATE_STYLE "sDateStyle"
#define DB_OSD_TIME_STYLE "sTimeStyle"
#define DB_OSD_TIME_STYLE_12 "12hour"
#define DB_OSD_TIME_STYLE_24 "24hour"
#define DB_OSD_TRANSPARENT_COLOR_ENABLED "iTransparentColorEnabled"
#define DB_OSD_ATTRIBUTE "sOSDAttribute"
#define DB_OSD_FONT_SIZE "sOSDFontSize"
#define DB_OSD_FRONT_COLOR_MODE "sOSDFrontColorMode"
#define DB_OSD_FRONT_COLOR "sOSDFrontColor"
#define DB_OSD_ALIGNMENT "sAlignment"
#define DB_OSD_BOUNDARY "iBoundary"

#define DB_ROI_STREAM_TYPE "sStreamType"
#define DB_ROI_STREAM_ENABLED "iStreamEnabled"
#define DB_ROI_ID "iROIId"
#define DB_ROI_ENABLED "iROIEnabled"
#define DB_ROI_NAME "sName"
#define DB_ROI_QUALITY_LEVEL "iQualityLevelOfROI"
#define DB_ROI_POSITION_X "iPositionX"
#define DB_ROI_POSITION_Y "iPositionY"
#define DB_ROI_WIDTH "iWidth"
#define DB_ROI_HTIGHT "iHeight"

#define DB_REGION_INVADE_ENABLED "iEnabled"
#define DB_REGION_INVADE_POSITION_X "iPositionX"
#define DB_REGION_INVADE_POSITION_Y "iPositionY"
#define DB_REGION_INVADE_WIDTH "iWidth"
#define DB_REGION_INVADE_HEIGHT "iHeight"
#define DB_REGION_INVADE_PROPORTION "iProportion"
#define DB_REGION_INVADE_SENSITIVITY_LEVEL "iSensitivityLevel"
#define DB_REGION_INVADE_TIME_THRESHOLD "iTimeThreshold"

#define DB_MOVE_DETECT_ENABLED "iMotionDetectionEnabled"
#define DB_MOVE_DETECT_DYNAMIC_ANALYSIS "iHighlightEnabled"
#define DB_MOVE_DETECT_SAMPLING_INTERVAL "iSamplingInterval"
#define DB_MOVE_DETECT_START_TRIGGER_TIME "iStartTriggerTime"
#define DB_MOVE_DETECT_END_TRIGGER_TIME "iEndTriggerTime"
#define DB_MOVE_DETECT_REGION_TYPE "sRegionType"
#define DB_MOVE_DETECT_ROW_GRANULARITY "iRowGranularity"
#define DB_MOVE_DETECT_COLUMN_GRANULARITY "iColumnGranularity"
#define DB_MOVE_DETECT_SENSITIVITY "iSensitivityLevel"
#define DB_MOVE_DETECT_GRID_MAP "sGridMap"

#define DB_MEDIA_SMART_COVER_FACE_DETECT_ENABLE "iFaceEnabled"
#define DB_MEDIA_SMART_COVER_FACE_REG_ENABLE "iFaceRecognitionEnabled"
#define DB_MEDIA_SMART_COVER_FACE_CAPTURE_ENABLE "iImageOverlayEnabled"
#define DB_MEDIA_SMART_COVER_DRAW_FACE_ENABLE "iStreamOverlayEnabled"

#define DB_RTMP "rtmp"
#define DB_PORT_NUM "iPortNo"

#define DB_VIDEO_ADVANCED_ENC_FUNCTION "sFunction"
#define DB_VIDEO_ADVANCED_ENC_PARAMETERS "sParameters"
#define DB_VIDEO_ADVANCED_ENC_STREAM_TYPE "sStreamType"
#define DB_VIDEO_ADVANCED_ENC_FUNCTION_QP "qp"
#define DB_VIDEO_ADVANCED_ENC_FUNCTION_SPLIT "split"
#define DB_VIDEO_ADVANCED_ENC_IMAGE_QUALITY_INIT "qp_init"
#define DB_VIDEO_ADVANCED_ENC_IMAGE_QUALITY_STEP "qp_step"
#define DB_VIDEO_ADVANCED_ENC_IMAGE_QUALITY_MIN "qp_min"
#define DB_VIDEO_ADVANCED_ENC_IMAGE_QUALITY_MAX "qp_max"
#define DB_VIDEO_ADVANCED_ENC_IMAGE_QUALITY_MIN_I_QP "min_i_qp"
#define DB_VIDEO_ADVANCED_ENC_IMAGE_QUALITY_MAX_I_QP "max_i_qp"
#define DB_VIDEO_ADVANCED_ENC_SPLIT_MODE "mode"
#define DB_VIDEO_ADVANCED_ENC_SPLIT_SIZE "size"

#define DB_VALUE_OPEN "open"
#define DB_VALUE_CLOSE "close"
#define DB_VALUE_SMART_CLOSE "\"sSmart\": \"open\""
#define DB_VALUE_ENABLE "1"
#define DB_VALUE_DISABLE "0"

#define DB_VALUE_AUTO "auto"
#define DB_VALUE_CUSTOMIZE "customize"

#define DB_SCHEDULES_ID_MOTION_DETECT 0
#define DB_SCHEDULES_ID_INTRUSION_DETECT 1
#define DB_SCHEDULES_ID_STORAGE_VIDEO_PLAN 2
#define DB_SCHEDULES_ID_SCREENSHOT 3
#define DB_SCHEDULES_TYPE_TIMING "timing"
#define DB_SCHEDULES_TYPE_MOTION_DETECT "motion-detect"
#define DB_SCHEDULES_TYPE_ALARM "alarm"
#define DB_SCHEDULES_TYPE_MOTION_OR_ALARM "motionOrAlarm"
#define DB_SCHEDULES_TYPE_MOTION_AND_ALARM "motionAndAlarm"
#define DB_SCHEDULES_TYPE_EVENT "event"

#define DB_IMAGE_SCENARIO_NORMAL "normal"
#define DB_IMAGE_SCENARIO_BACKLIGHT "backlight"
#define DB_IMAGE_SCENARIO_FRONTLIGHT "frontlight"
#define DB_IMAGE_SCENARIO_LOW_ILLUMINATION "low-illumination"
#define DB_IMAGE_SCENARIO_CUSTOM1 "custom1"
#define DB_IMAGE_SCENARIO_CUSTOM2 "custom2"

#define DB_IMAGE_GRAY_SCALE_MODE "sGrayScaleMode"
#define DB_IMAGE_GRAY_SCALE_0_255 "[0-255]"
#define DB_IMAGE_GRAY_SCALE_16_235 "[16-235]"

} // namespace mediaserver
} // namespace rockchip

#endif // _RK_DBUS_MEDIASERVER_KEY_H_
