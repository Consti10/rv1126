// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <getopt.h>
#include <memory>

#include "mediaserver.h"
#include "flow_export.h"


#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "mediaserver.cpp"

static bool quit = false;
static bool need_dbus = true;
static bool is_session_bus = false;
static std::string media_config;
static bool need_dbserver = true;

#ifdef ENABLE_SHM_SERVER
#include "shm_control.h"
shmc::ShmQueue<shmc::SVIPC> queue_w_;
#endif

static void parse_args(int argc, char **argv);

namespace rockchip {
namespace mediaserver {

#ifdef LINK_API_ENABLE
int MediaServer::InitMediaLink() {
  LinkManagerPtr &link_manager = LinkManager::GetInstance();
  if (link_manager) {
    LicenseKey license;
    auto link_flow_unit = link_manager->GetLinkFlowUnit(0, "video:");
    if (link_flow_unit) {
      license.product_key = link_flow_unit->GetProductKey();
      license.device_name = link_flow_unit->GetDeviceName();
      license.device_secret = link_flow_unit->GetDeviceSecret();
      license.product_secret = link_flow_unit->GetProductSecret();
    } else {
      LOG_ERROR("can't find link flow\n");
      return -1;
    }
    LOG_DEBUG("license.product_key %s\n", license.product_key.c_str());
    LOG_DEBUG("license.device_name %s\n", license.device_name.c_str());
    LOG_DEBUG("license.device_secret %s\n", license.device_secret.c_str());
    LOG_DEBUG("license.product_secret %s\n", license.product_secret.c_str());
    link_manager->FillLicenseKey(&license);
    link_manager->InitDevice();
    link_manager->StartLink();
  }
  return 0;
}

int MediaServer::DeInitMediaLink() {
  LinkManagerPtr &link_manager = LinkManager::GetInstance();
  if (link_manager) {
    link_manager->StopLink();
    link_manager->DeInitDevice();
  }
  return 0;
}
#endif

#ifdef ENABLE_DBUS
int MediaServer::InitDbusServer() {
  if (need_dbus) {
    dbus_server_.reset(new DBusServer(is_session_bus, need_dbserver));
    assert(dbus_server_);
    dbus_server_->start();
  }
  return 0;
}

int MediaServer::DeInitDbusServer() {
  if (need_dbus) {
    if (dbus_server_ != nullptr)
      dbus_server_->stop();
  }
  return 0;
}

int MediaServer::RegisterDbusProxy(FlowManagerPtr &flow_manager) {
  if (need_dbus) {
    flow_manager->RegisterDBserverProxy(dbus_server_->GetDBserverProxy());
    flow_manager->RegisterDBEventProxy(dbus_server_->GetDBEventProxy());
    flow_manager->RegisterStorageManagerProxy(dbus_server_->GetStorageProxy());
  }
  return 0;
}

#endif

MediaServer::MediaServer() {
  LOG_DEBUG("media servers setup ...\n");
#ifdef ENABLE_DBUS
  InitDbusServer();
#endif
  FlowManagerPtr &flow_manager = FlowManager::GetInstance();
  assert(flow_manager);
#ifdef ENABLE_DBUS
  RegisterDbusProxy(flow_manager);
#endif
  flow_manager->ConfigParse(media_config);
  flow_manager->CreatePipes();
#ifdef ENABLE_DBUS
#ifdef ENABLE_SCHEDULES_SERVER
  if (need_dbserver)
    flow_manager->CreateSchedules();
#endif
#endif

#ifdef ENABLE_ZBAR
  flow_manager->CreateScanImage();
#endif

#ifdef ENABLE_SHM_SERVER
  auto camera_pipe = GetFlowPipe(0, StreamType::CAMERA);
  if (camera_pipe) {
    auto link_flow = camera_pipe->GetFlow(StreamType::LINK);
    if (link_flow)
      link_flow->SetUserCallBack(nullptr, ShmControl::PushUserHandler);
    auto rockx_filter =
        camera_pipe->GetFlow(StreamType::FILTER, RKMEDIA_FILTER_ROCKX_FILTER);
    LOG("RegisterCallBack ***********rockx_filter=%p\n", rockx_filter);
    if (rockx_filter)
      rockx_filter->Control(easymedia::S_NN_CALLBACK,
                            ShmControl::PushUserHandler);
  }
  auto file_pipe = GetFlowPipe(0, StreamType::FILE);
  if (file_pipe) {
    auto link_flow = file_pipe->GetFlow(StreamType::LINK);
    if (link_flow)
      link_flow->SetUserCallBack(nullptr, ShmControl::PushUserHandler);
  }
#endif

#ifdef LINK_API_ENABLE
  InitMediaLink(); // will block until connecting to aliyun
#endif

  LOG_DEBUG("media servers setup ok\n");
}

MediaServer::~MediaServer() {
  FlowManagerPtr &flow_manager = FlowManager::GetInstance();

#ifdef LINK_API_ENABLE
  DeInitMediaLink();
#endif

#ifdef ENABLE_ZBAR
  flow_manager->DestoryScanImage();
#endif

#ifdef ENABLE_DBUS
#ifdef ENABLE_SCHEDULES_SERVER
  if (need_dbserver)
    flow_manager->DestorySchedules();
#endif
  DeInitDbusServer();
#endif

  flow_manager->SaveConfig(FLOWS_CONF_BAK);
  flow_manager->DestoryPipes();
  flow_manager.reset();
  LOG_DEBUG("media servers upload finish\n");
}

} // namespace mediaserver
} // namespace rockchip

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
#ifdef LINK_API_ENABLE
#ifdef THUNDER_BOOT
  // StopRecord(0);
  system("echo timer > /sys/class/leds/red/trigger");
  auto &link_manager = rockchip::mediaserver::LinkManager::GetInstance();
  if (link_manager) {
    link_manager->ReportWakeUpData1();
    link_manager->StopLink();
    link_manager->DeInitDevice();
  }
  auto link_flow_unit = link_manager->GetLinkFlowUnit(0, "video:");
  char cmd[1024];
  sprintf(cmd, "mqtt-rrpc-demo -p %s -n %s -s %s 2>&1 &",
                link_flow_unit->GetProductKey().c_str(),
                link_flow_unit->GetDeviceName().c_str(),
                link_flow_unit->GetDeviceSecret().c_str());
  printf("cmd is %s\n", cmd);
  system(cmd);
#endif
#endif
  quit = true;
}

int main(int argc, char *argv[]) {
#ifdef ENABLE_SHM_SERVER
  shmc::SetLogHandler(shmc::kDebug, [](shmc::LogLevel lv, const char *s) {
    fprintf(stderr, "[%d] %s", lv, s);
  });
  queue_w_.InitForWrite(kShmKey, kQueueBufSize);
#endif
  parse_args(argc, argv);
  setenv("RKMEDIA_LOG_METHOD", "MINILOG", 0);
  //COnsti10setenv("RKMEDIA_LOG_LEVEL", "INFO", 0);
    setenv("RKMEDIA_LOG_LEVEL", "DEBUG", 0);
  LOG_INIT();
  __minilog_log_init(argv[0], NULL, false, enable_minilog_backtrace, argv[0],
                     "1.0.0");

  fprintf(stderr, "mediaserver[%d]: start config: %s\n", __LINE__,
          (char *)media_config.c_str());
  fprintf(stderr,"Hello from Consti\n");
  signal(SIGQUIT, sigterm_handler);
  signal(SIGINT, sigterm_handler);
  signal(SIGTERM, sigterm_handler);
  signal(SIGXCPU, sigterm_handler);
  signal(SIGPIPE, SIG_IGN);
  std::unique_ptr<rockchip::mediaserver::MediaServer> MS =
      std::unique_ptr<rockchip::mediaserver::MediaServer>(
          new rockchip::mediaserver::MediaServer());
  while (!quit) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  MS.reset();
  return 0;
}

static void usage_tip(FILE *fp, int argc, char **argv) {
  fprintf(
      fp,
      "Usage: %s [options]\n"
      "Version %s\n"
      "Options:\n"
      "-c | --config      mediaserver confg file \n"
      "-S | --system      Use system bus \n"
      "-s | --session     Use session bus \n"
      "-D | --database    Depend dbserver app \n"
      "-d | --no_database Not depend dbserver app \n"
      "-a | --stand_alone Not depend dbus server \n"
      "-h | --help        For help \n\n"
      "Environment variable:\n"
      "mediaserver_log_level        [0/1/2/3],   loglevel: "
      "error/warn/info/debug\n"
      "enable_minilog_backtrace     [0/1],       enable minilog backtrace \n"
      "enable_encoder_debug         [0/1],       enable encoder statistics \n"
      "\n",
      argv[0], "V1.1");
}

static const char short_options[] = "c:SsDdah";
static const struct option long_options[] = {
    {"config", required_argument, NULL, 'c'},
    {"system", no_argument, NULL, 'S'},
    {"session", no_argument, NULL, 's'},
    {"database", no_argument, NULL, 'D'},
    {"no_database", no_argument, NULL, 'd'},
    {"stand_alone", no_argument, NULL, 'a'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}};

static int get_env(const char *name, int *value, int default_value) {
  char *ptr = getenv(name);
  if (NULL == ptr) {
    *value = default_value;
  } else {
    char *endptr;
    int base = (ptr[0] == '0' && ptr[1] == 'x') ? (16) : (10);
    errno = 0;
    *value = strtoul(ptr, &endptr, base);
    if (errno || (ptr == endptr)) {
      errno = 0;
      *value = default_value;
    }
  }
  return 0;
}

static void parse_args(int argc, char **argv) {
  media_config = "";
#ifdef MEDIASERVER_CONF_PREFIX
  media_config.append(MEDIASERVER_CONF_PREFIX).append(FLOWS_CONF);
#else
  media_config.append(FLOWS_CONF);
#endif

#ifdef ENABLE_MINILOGGER
  enable_minilog = 1;
#else
  enable_minilog = 0;
#endif

  get_env("enable_minilog_backtrace", &enable_minilog_backtrace, 0);
  get_env("enable_encoder_debug", &enable_encoder_debug, 0);
  get_env("mediaserver_log_level", &mediaserver_log_level, LOG_LEVEL_DEBUG);
  LOG_INFO("mediaserver_log_level is %d\n", mediaserver_log_level);
  LOG_INFO("Hello from Consti2\n");
  for (;;) {
    int idx;
    int c;
    c = getopt_long(argc, argv, short_options, long_options, &idx);
    if (-1 == c)
      break;
    switch (c) {
    case 0: /* getopt_long() flag */
      break;
    case 'c':
      media_config = optarg;
      break;
    case 'S':
      is_session_bus = false;
      break;
    case 's':
      is_session_bus = true;
      break;
    case 'D':
      need_dbserver = true;
      break;
    case 'd':
      need_dbserver = false;
      break;
    case 'a':
      need_dbus = false;
      break;
    case 'h':
      usage_tip(stdout, argc, argv);
      exit(EXIT_SUCCESS);
    default:
      usage_tip(stderr, argc, argv);
      exit(EXIT_FAILURE);
    }
  }
  if (media_config.empty()) {
    usage_tip(stderr, argc, argv);
    exit(EXIT_FAILURE);
  }
}
