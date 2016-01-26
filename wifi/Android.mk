# Copyright 2006 The Android Open Source Project

ifdef WIFI_DRIVER_MODULE_PATH
LOCAL_CFLAGS += -DWIFI_DRIVER_MODULE_PATH=\"$(WIFI_DRIVER_MODULE_PATH)\"
endif
ifdef WIFI_DRIVER_MODULE_ARG
LOCAL_CFLAGS += -DWIFI_DRIVER_MODULE_ARG=\"$(WIFI_DRIVER_MODULE_ARG)\"
endif
ifdef WIFI_DRIVER_MODULE_NAME
LOCAL_CFLAGS += -DWIFI_DRIVER_MODULE_NAME=\"$(WIFI_DRIVER_MODULE_NAME)\"
endif
ifdef WIFI_FIRMWARE_LOADER
LOCAL_CFLAGS += -DWIFI_FIRMWARE_LOADER=\"$(WIFI_FIRMWARE_LOADER)\"
endif
ifdef WIFI_DRIVER_FW_PATH_STA
LOCAL_CFLAGS += -DWIFI_DRIVER_FW_PATH_STA=\"$(WIFI_DRIVER_FW_PATH_STA)\"
endif
ifdef WIFI_DRIVER_FW_PATH_AP
LOCAL_CFLAGS += -DWIFI_DRIVER_FW_PATH_AP=\"$(WIFI_DRIVER_FW_PATH_AP)\"
endif
ifdef WIFI_DRIVER_FW_PATH_P2P
LOCAL_CFLAGS += -DWIFI_DRIVER_FW_PATH_P2P=\"$(WIFI_DRIVER_FW_PATH_P2P)\"
endif
ifdef WIFI_DRIVER_FW_PATH_PARAM
LOCAL_CFLAGS += -DWIFI_DRIVER_FW_PATH_PARAM=\"$(WIFI_DRIVER_FW_PATH_PARAM)\"
endif

ifdef WIFI_DRIVER_STATE_CTRL_PARAM
LOCAL_CFLAGS += -DWIFI_DRIVER_STATE_CTRL_PARAM=\"$(WIFI_DRIVER_STATE_CTRL_PARAM)\"
endif
ifdef WIFI_DRIVER_STATE_ON
LOCAL_CFLAGS += -DWIFI_DRIVER_STATE_ON=\"$(WIFI_DRIVER_STATE_ON)\"
endif
ifdef WIFI_DRIVER_STATE_OFF
LOCAL_CFLAGS += -DWIFI_DRIVER_STATE_OFF=\"$(WIFI_DRIVER_STATE_OFF)\"
endif
ifeq ($(BOARD_WIFI_VENDOR),realtek)
LOCAL_CFLAGS += -DBOARD_WIFI_REALTEK
endif

ifeq ($(USB_WIFI_SUPPORT),true)
LOCAL_CFLAGS += -DUSB_WIFI_SUPPORT
endif


ifeq ($(MULTI_WIFI_SUPPORT),true)
LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
                    external/libusb/libusb-0.1.12
LOCAL_CFLAGS += -DSYSFS_PATH_MAX=256
LOCAL_CFLAGS += -DMULTI_WIFI_SUPPORT
LOCAL_CFLAGS += -DUSB_WIFI_SUPPORT
LOCAL_CFLAGS += -DUSB_FS_DIR=\"/dev/bus/usb\"
LOCAL_SHARED_LIBRARIES += \
			libcutils \
			libusb \
			lib_driver_load

LOCAL_SRC_FILES += wifi/multi_dongle.c

endif

ifeq ($(BCM_USB_WIFI),true)
LOCAL_CFLAGS += -DBCM_USB_WIFI
LOCAL_CFLAGS += -DUSB_WIFI_SUPPORT
endif

LOCAL_SRC_FILES += wifi/wifi.c

ifdef WPA_SUPPLICANT_VERSION
LOCAL_CFLAGS += -DLIBWPA_CLIENT_EXISTS
LOCAL_SHARED_LIBRARIES += libwpa_client
endif
LOCAL_SHARED_LIBRARIES += libnetutils
