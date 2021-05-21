#
# Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#

# vi: set noexpandtab syntax=cmake:

project(rkmedia_examples)

add_definitions(-g -O0 -ggdb -gdwarf -funwind-tables -rdynamic)

add_definitions(-DDEBUG)

aux_source_directory(common COMMON_SRC)
#--------------------------
# rkmedia_vi_venc_test
#--------------------------
add_executable(rkmedia_vi_venc_test rkmedia_vi_venc_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_venc_test easymedia)
target_link_libraries(rkmedia_vi_venc_test easymedia)
target_include_directories(rkmedia_vi_venc_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_venc_test RUNTIME DESTINATION "bin")

#--------------------------
# rkmedia_vi_venc_file_test
#--------------------------
add_executable(rkmedia_vi_venc_file_test rkmedia_vi_venc_file_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_venc_file_test easymedia)
target_link_libraries(rkmedia_vi_venc_file_test easymedia)
target_include_directories(rkmedia_vi_venc_file_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_venc_file_test RUNTIME DESTINATION "bin")

#--------------------------
# rkmedia_venc_osd_test
#--------------------------
add_executable(rkmedia_venc_osd_test rkmedia_venc_osd_test.c ${COMMON_SRC})
add_dependencies(rkmedia_venc_osd_test easymedia)
target_link_libraries(rkmedia_venc_osd_test easymedia)
target_include_directories(rkmedia_venc_osd_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_osd_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_audio_test
#--------------------------
add_executable(rkmedia_audio_test rkmedia_audio_test.c)
add_dependencies(rkmedia_audio_test easymedia)
target_link_libraries(rkmedia_audio_test easymedia)
target_include_directories(rkmedia_audio_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_audio_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_ai_test
#--------------------------
add_executable(rkmedia_ai_test rkmedia_ai_test.c)
add_dependencies(rkmedia_ai_test easymedia)
target_link_libraries(rkmedia_ai_test easymedia)
target_include_directories(rkmedia_ai_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_ai_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_ao_test
#--------------------------
add_executable(rkmedia_ao_test rkmedia_ao_test.c)
add_dependencies(rkmedia_ao_test easymedia)
target_link_libraries(rkmedia_ao_test easymedia)
target_include_directories(rkmedia_ao_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_ao_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_adec_ao_test
#--------------------------
add_executable(rkmedia_adec_ao_test rkmedia_adec_ao_test.c)
add_dependencies(rkmedia_adec_ao_test easymedia)
target_link_libraries(rkmedia_adec_ao_test easymedia)
target_include_directories(rkmedia_adec_ao_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_adec_ao_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_ai_aenc_test
#--------------------------
add_executable(rkmedia_ai_aenc_test rkmedia_ai_aenc_test.c)
add_dependencies(rkmedia_ai_aenc_test easymedia)
target_link_libraries(rkmedia_ai_aenc_test easymedia)
target_include_directories(rkmedia_ai_aenc_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_ai_aenc_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_md_test
#--------------------------
add_executable(rkmedia_vi_md_test rkmedia_vi_md_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_md_test easymedia)
target_link_libraries(rkmedia_vi_md_test easymedia)
target_include_directories(rkmedia_vi_md_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_md_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_od_test
#--------------------------
add_executable(rkmedia_vi_od_test rkmedia_vi_od_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_od_test easymedia)
target_link_libraries(rkmedia_vi_od_test easymedia)
target_include_directories(rkmedia_vi_od_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_od_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_venc_jpeg_test
#--------------------------
add_executable(rkmedia_venc_jpeg_test rkmedia_venc_jpeg_test.c ${COMMON_SRC})
add_dependencies(rkmedia_venc_jpeg_test easymedia)
target_link_libraries(rkmedia_venc_jpeg_test easymedia)
target_include_directories(rkmedia_venc_jpeg_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_jpeg_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_venc_jpeg_light_test
#--------------------------
add_executable(rkmedia_venc_jpeg_light_test rkmedia_venc_jpeg_light_test.c ${COMMON_SRC})
add_dependencies(rkmedia_venc_jpeg_light_test easymedia)
target_link_libraries(rkmedia_venc_jpeg_light_test easymedia)
target_include_directories(rkmedia_venc_jpeg_light_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_jpeg_light_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_venc_mjpeg_test
#--------------------------
add_executable(rkmedia_venc_mjpeg_test rkmedia_venc_mjpeg_test.c ${COMMON_SRC})
add_dependencies(rkmedia_venc_mjpeg_test easymedia)
target_link_libraries(rkmedia_venc_mjpeg_test easymedia)
target_include_directories(rkmedia_venc_mjpeg_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_mjpeg_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_multi_bind_test
#--------------------------
add_executable(rkmedia_vi_multi_bind_test rkmedia_vi_multi_bind_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_multi_bind_test easymedia)
target_link_libraries(rkmedia_vi_multi_bind_test easymedia)
target_include_directories(rkmedia_vi_multi_bind_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_multi_bind_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_venc_cover_test
#--------------------------
add_executable(rkmedia_venc_cover_test rkmedia_venc_cover_test.c ${COMMON_SRC})
add_dependencies(rkmedia_venc_cover_test easymedia)
target_link_libraries(rkmedia_venc_cover_test easymedia)
target_include_directories(rkmedia_venc_cover_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_cover_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_venc_local_file_test
#--------------------------
add_executable(rkmedia_venc_local_file_test rkmedia_venc_local_file_test.c)
add_dependencies(rkmedia_venc_local_file_test easymedia)
target_link_libraries(rkmedia_venc_local_file_test easymedia)
target_include_directories(rkmedia_venc_local_file_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_local_file_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_venc_smartp_test
#--------------------------
add_executable(rkmedia_venc_smartp_test rkmedia_venc_smartp_test.c ${COMMON_SRC})
add_dependencies(rkmedia_venc_smartp_test easymedia)
target_link_libraries(rkmedia_venc_smartp_test easymedia)
target_include_directories(rkmedia_venc_smartp_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_smartp_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_get_frame_test
#--------------------------
add_executable(rkmedia_vi_get_frame_test rkmedia_vi_get_frame_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_get_frame_test easymedia)
target_link_libraries(rkmedia_vi_get_frame_test easymedia)
target_include_directories(rkmedia_vi_get_frame_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_get_frame_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_rga_test
#--------------------------
add_executable(rkmedia_vi_rga_test rkmedia_vi_rga_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_rga_test easymedia)
target_link_libraries(rkmedia_vi_rga_test easymedia)
target_include_directories(rkmedia_vi_rga_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_rga_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_luma_only_mode_test
#--------------------------
add_executable(rkmedia_vi_luma_only_mode_test rkmedia_vi_luma_only_mode_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_luma_only_mode_test easymedia)
target_link_libraries(rkmedia_vi_luma_only_mode_test easymedia)
target_include_directories(rkmedia_vi_luma_only_mode_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_luma_only_mode_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_god_mode_test
#--------------------------
add_executable(rkmedia_vi_god_mode_test rkmedia_vi_god_mode_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_god_mode_test easymedia)
target_link_libraries(rkmedia_vi_god_mode_test easymedia)
target_include_directories(rkmedia_vi_god_mode_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_god_mode_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_venc_change_resolution_test
#--------------------------
if(USE_RKAIQ)
link_directories(${PROJECT_SOURCE_DIR}/librtsp/)
add_executable(rkmedia_vi_venc_change_resolution_test rkmedia_vi_venc_change_resolution_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_venc_change_resolution_test easymedia)
target_link_libraries(rkmedia_vi_venc_change_resolution_test rtsp easymedia)
target_include_directories(rkmedia_vi_venc_change_resolution_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_venc_change_resolution_test RUNTIME DESTINATION "bin")
endif()

#--------------------------
#  rkmedia_venc_roi_osd_test
#--------------------------
add_executable(rkmedia_venc_roi_osd_test rkmedia_venc_roi_osd_test.c ${COMMON_SRC})
add_dependencies(rkmedia_venc_roi_osd_test easymedia)
target_link_libraries(rkmedia_venc_roi_osd_test easymedia)
target_include_directories(rkmedia_venc_roi_osd_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_roi_osd_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_vo_test
#--------------------------
add_executable(rkmedia_vi_vo_test rkmedia_vi_vo_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_vo_test easymedia)
target_link_libraries(rkmedia_vi_vo_test easymedia)
target_include_directories(rkmedia_vi_vo_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_vo_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vo_local_file_test
#--------------------------
add_executable(rkmedia_vo_local_file_test rkmedia_vo_local_file_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vo_local_file_test easymedia)
target_link_libraries(rkmedia_vo_local_file_test easymedia)
target_include_directories(rkmedia_vo_local_file_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vo_local_file_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vo_scale_test
#--------------------------
add_executable(rkmedia_vo_scale_test rkmedia_vo_scale_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vo_scale_test easymedia)
target_link_libraries(rkmedia_vo_scale_test easymedia)
target_include_directories(rkmedia_vo_scale_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vo_scale_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_main_stream_with_jpeg_test
#--------------------------
add_executable(rkmedia_main_stream_with_jpeg_test rkmedia_main_stream_with_jpeg_test.c ${COMMON_SRC})
add_dependencies(rkmedia_main_stream_with_jpeg_test easymedia)
target_link_libraries(rkmedia_main_stream_with_jpeg_test easymedia)
target_include_directories(rkmedia_main_stream_with_jpeg_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_main_stream_with_jpeg_test RUNTIME DESTINATION "bin")

#--------------------------
# rkmedia_isp_test
#--------------------------
if(USE_RKAIQ)
add_executable(rkmedia_isp_test rkmedia_isp_test.c ${COMMON_SRC})
add_dependencies(rkmedia_isp_test easymedia)
target_link_libraries(rkmedia_isp_test easymedia)
target_include_directories(rkmedia_isp_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_isp_test RUNTIME DESTINATION "bin")
endif()

#--------------------------
#  rkmedia_vi_double_cameras_test
#--------------------------
add_executable(rkmedia_vi_double_cameras_test rkmedia_vi_double_cameras_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_double_cameras_test easymedia)
target_link_libraries(rkmedia_vi_double_cameras_test easymedia)
target_include_directories(rkmedia_vi_double_cameras_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_double_cameras_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_vi_double_cameras_switch_test
#--------------------------
add_executable(rkmedia_vi_double_cameras_switch_test rkmedia_vi_double_cameras_switch_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_double_cameras_switch_test easymedia)
target_link_libraries(rkmedia_vi_double_cameras_switch_test easymedia)
target_include_directories(rkmedia_vi_double_cameras_switch_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_vi_double_cameras_switch_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_venc_avbr_test
#--------------------------
add_executable(rkmedia_venc_avbr_test rkmedia_venc_avbr_test.c ${COMMON_SRC})
add_dependencies(rkmedia_venc_avbr_test easymedia)
target_link_libraries(rkmedia_venc_avbr_test easymedia)
target_include_directories(rkmedia_venc_avbr_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_venc_avbr_test RUNTIME DESTINATION "bin")

#--------------------------
#  rkmedia_stream_stress_test
#--------------------------
add_executable(rkmedia_stream_stress_test rkmedia_stream_stress_test.c ${COMMON_SRC})
add_dependencies(rkmedia_stream_stress_test easymedia)
target_link_libraries(rkmedia_stream_stress_test easymedia)
target_include_directories(rkmedia_stream_stress_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_stream_stress_test RUNTIME DESTINATION "bin")

#--------------------------
# rkmedia_isp_stresss_test
#--------------------------
if(USE_RKAIQ)
link_directories(${PROJECT_SOURCE_DIR}/librtsp/)
add_executable(rkmedia_isp_stresss_test rkmedia_isp_stresss_test.c ${COMMON_SRC})
add_dependencies(rkmedia_isp_stresss_test easymedia)
target_link_libraries(rkmedia_isp_stresss_test rtsp easymedia)
target_include_directories(rkmedia_isp_stresss_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_isp_stresss_test RUNTIME DESTINATION "bin")
endif()

#--------------------------
# rkmedia_rga_api_test
#--------------------------
add_executable(rkmedia_rga_api_test rkmedia_rga_api_test.c ${COMMON_SRC})
add_dependencies(rkmedia_rga_api_test easymedia)
target_link_libraries(rkmedia_rga_api_test easymedia rga)
target_include_directories(rkmedia_rga_api_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_rga_api_test RUNTIME DESTINATION "bin")

#  rkmedia_vi_person_detect_venc_rtsp_test
#--------------------------
if(ROCKX)
link_directories(${PROJECT_SOURCE_DIR}/librtsp/)
add_executable(rkmedia_vi_person_detect_venc_rtsp_test rkmedia_vi_person_detect_venc_rtsp_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_person_detect_venc_rtsp_test easymedia)
target_link_libraries(rkmedia_vi_person_detect_venc_rtsp_test rtsp rockx easymedia)
target_include_directories(rkmedia_vi_person_detect_venc_rtsp_test PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SYSROOT}/usr/include/rockx)
install(TARGETS rkmedia_vi_person_detect_venc_rtsp_test RUNTIME DESTINATION "bin")
install(FILES rtsp-nn.cfg DESTINATION share)
endif()

#  rkmedia_vi_rknn_venc_rtsp_test
#--------------------------
option(RKNN_API "Enable rknn api" OFF)
if(RKNN_API)
link_directories(${PROJECT_SOURCE_DIR}/librtsp/)
add_executable(rkmedia_vi_rknn_venc_rtsp_test rkmedia_vi_rknn_venc_rtsp_test.c ${COMMON_SRC})
add_dependencies(rkmedia_vi_rknn_venc_rtsp_test easymedia)
target_link_libraries(rkmedia_vi_rknn_venc_rtsp_test rtsp rknn_api m easymedia)
target_include_directories(rkmedia_vi_rknn_venc_rtsp_test PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SYSROOT}/usr/include/rknn)
install(TARGETS rkmedia_vi_rknn_venc_rtsp_test RUNTIME DESTINATION "bin")
install(FILES rtsp-nn.cfg DESTINATION share)
install(DIRECTORY rknn_model DESTINATION share)
endif()

#--------------------------
# rkmedia_rga_crop_venc_test
#--------------------------
if(USE_RKAIQ)
link_directories(${PROJECT_SOURCE_DIR}/librtsp/)
add_executable(rkmedia_rga_crop_venc_test rkmedia_rga_crop_venc_test.c ${COMMON_SRC})
add_dependencies(rkmedia_rga_crop_venc_test easymedia)
target_link_libraries(rkmedia_rga_crop_venc_test rtsp easymedia rga)
target_include_directories(rkmedia_rga_crop_venc_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkmedia_rga_crop_venc_test RUNTIME DESTINATION "bin")
endif()

option(COMPILES_UINT_TEST "Enable unit tests" OFF)
if(COMPILES_UINT_TEST)
  add_subdirectory(uintTest)
endif()
