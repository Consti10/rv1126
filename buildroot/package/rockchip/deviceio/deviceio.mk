DEVICEIO_SITE = $(TOPDIR)/../external/deviceio
DEVICEIO_SITE_METHOD = local
DEVICEIO_INSTALL_STAGING = YES

ifeq ($(call qstrip,$(BR2_PACKAGE_RKWIFIBT_VENDOR)), CYPRESS)
DEVICEIO_CYPRESS_BSA = $(TOPDIR)/../external/bluetooth_bsa/3rdparty/embedded/bsa_examples/linux
DEVICEIO_CONF_OPTS += -DBSA=TRUE -DCYPRESS=TRUE \
		-DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -I$(DEVICEIO_CYPRESS_BSA)/libbsa/include -I$(DEVICEIO_CYPRESS_BSA)/app_common/include" \
		-DCMAKE_CXX_FLAGS="${CMAKE_C_FLAGS} -I$(DEVICEIO_CYPRESS_BSA)/libbsa/include -I$(DEVICEIO_CYPRESS_BSA)/app_common/include"
DEVICEIO_DEPENDENCIES += cypress_bsa
else ifeq ($(call qstrip,$(BR2_PACKAGE_RKWIFIBT_VENDOR)), BROADCOM)
DEVICEIO_BROADCOM_BSA = $(TOPDIR)/../external/broadcom_bsa/3rdparty/embedded/bsa_examples/linux
DEVICEIO_CONF_OPTS += -DBSA=TRUE -DBROADCOM=TRUE \
                -DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -I$(DEVICEIO_BROADCOM_BSA)/libbsa/include -I$(DEVICEIO_BROADCOM_BSA)/app_common/include" \
                -DCMAKE_CXX_FLAGS="${CMAKE_C_FLAGS} -I$(DEVICEIO_BROADCOM_BSA)/libbsa/include -I$(DEVICEIO_BROADCOM_BSA)/app_common/include"
DEVICEIO_DEPENDENCIES += broadcom_bsa
else
ifeq ($(call qstrip,$(BR2_PACKAGE_RKWIFIBT_VENDOR)), REALTEK)
DEVICEIO_CONF_OPTS += -DBLUEZ5_UTILS=TRUE
DEVICEIO_CONF_OPTS += -DBLUEZ=TRUE
DEVICEIO_DEPENDENCIES += readline bluez5_utils libglib2
endif
endif

ifeq ($(BR2_PACKAGE_RKWIFIBT_RTL8723DS),y)
DEVICEIO_CONF_OPTS += -DREALTEK=TRUE
endif

ifeq ($(BR2_PACKAGE_RKWIFIBT_RTL8189FS),y)
DEVICEIO_CONF_OPTS += -DREALTEK=TRUE
endif

ifeq ($(call qstrip,$(BR2_ARCH)), arm)
DEVICEIO_BUILD_TYPE = arm
else ifeq ($(call qstrip, $(BR2_ARCH)), aarch64)
DEVICEIO_BUILD_TYPE = arm64
endif

ifeq ($(BR2_PACKAGE_BR2_PACKAGE_GSTREAMER1),y)
DEVICEIO_CONF_OPTS += -DBUILD_RK_MEDIAPLAYER
endif

DEVICEIO_CONF_OPTS += -DCPU_ARCH=$(BR2_ARCH) -DBUILD_TYPE=$(DEVICEIO_BUILD_TYPE)

DEVICEIO_DEPENDENCIES += wpa_supplicant alsa-lib

DEVICEIO_CONF_OPTS += -DCMAKE_INSTALL_STAGING=$(STAGING_DIR)

$(eval $(cmake-package))
