LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := fstab.samsungexynos7880
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := fstab.samsungexynos7880
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.baseband.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := init.baseband.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.power.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := init.power.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.rilchip.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := init.rilchip.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.samsung.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := init.samsung.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.samsungexynos7880.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := init.samsungexynos7880.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.samsungexynos7880.usb.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := init.samsungexynos7880.usb.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.wifi.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := init.wifi.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.wifi_qcom.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := init.wifi_qcom.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := ueventd.samsungexynos7880.rc
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := BOOT
LOCAL_SRC_FILES    := ueventd.samsungexynos7880.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)
