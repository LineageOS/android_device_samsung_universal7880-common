#!/bin/bash
# Copyright (C) 2017 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

VENDOR=samsung
DEVICE_COMMON=universal7880-common
QCA_CLD_BLD_DIR=vendor/qcom/opensource/wlan

# Load extractutils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$MY_DIR" ]]; then MY_DIR="$PWD"; fi

CM_ROOT="$MY_DIR"/../../..

HELPER="$CM_ROOT"/vendor/cm/build/tools/extract_utils.sh
if [ ! -f "$HELPER" ]; then
    echo "Unable to find helper script at $HELPER"
    exit 1
fi
. "$HELPER"

# Initialize the helper
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$CM_ROOT" true

# Copyright headers and guards
write_headers "a5y17lte a7y17lte"

# The standard blobs
write_makefiles "$MY_DIR"/proprietary-files.txt

# The BSP blobs - we put a conditional in case the BSP
# is actually being built
printf '\n%s\n' 'ifeq ($(WITH_EXYNOS_BSP),)' >> "$PRODUCTMK"
printf '\n%s\n' 'ifeq ($(WITH_EXYNOS_BSP),)' >> "$ANDROIDMK"

write_makefiles "$MY_DIR"/proprietary-files-bsp.txt

printf '%s\n' 'endif' >> "$PRODUCTMK"
printf '%s\n' 'endif' >> "$ANDROIDMK"

###################################################################################################
# CUSTOM PART START                                                                               #
###################################################################################################

OUTDIR=vendor/$VENDOR/$DEVICE_COMMON

(cat << EOF) >> $CM_ROOT/$OUTDIR/Android.mk
include \$(CLEAR_VARS)
LOCAL_MODULE := libGLES_mali
LOCAL_MODULE_OWNER := samsung
LOCAL_SRC_FILES_64 := proprietary/vendor/lib64/egl/libGLES_mali.so
LOCAL_SRC_FILES_32 := proprietary/vendor/lib/egl/libGLES_mali.so
LOCAL_MULTILIB := both
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE_PATH_32 := \$(\$(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_OUT_VENDOR_SHARED_LIBRARIES)/egl
LOCAL_MODULE_PATH_64 := \$(TARGET_OUT_VENDOR_SHARED_LIBRARIES)/egl

SYMLINKS := \$(TARGET_OUT)/vendor
\$(SYMLINKS):
	@echo "Symlink: vulkan.exynos5.so"
	@mkdir -p \$@/lib/hw
	@mkdir -p \$@/lib64/hw
	\$(hide) ln -sf ../egl/libGLES_mali.so \$@/lib/hw/vulkan.exynos5.so
	\$(hide) ln -sf ../egl/libGLES_mali.so \$@/lib64/hw/vulkan.exynos5.so
	@echo "Symlink: libOpenCL.so.1.1"
	\$(hide) ln -sf egl/libGLES_mali.so \$@/lib/libOpenCL.so.1.1
	\$(hide) ln -sf egl/libGLES_mali.so \$@/lib64/libOpenCL.so.1.1

ALL_MODULES.\$(LOCAL_MODULE).INSTALLED := \\
	\$(ALL_MODULES.\$(LOCAL_MODULE).INSTALLED) \$(SYMLINKS)

include \$(BUILD_PREBUILT)

EOF

(cat << EOF) >> $CM_ROOT/$OUTDIR/$DEVICE_COMMON-vendor.mk

# Create Mali links for Vulkan and OpenCL
PRODUCT_PACKAGES += libGLES_mali

# Qualcomm WiFi driver
PRODUCT_PACKAGES += qca_cld_wlan.ko
EOF

(cat << EOF) >> $CM_ROOT/$QCA_CLD_BLD_DIR/qcacld-2.0/Kbuild

EXTRA_CFLAGS += -Wno-pointer-sign
EXTRA_CFLAGS += -Wno-unused-but-set-variable

EOF

(cat << EOF) > $CM_ROOT/$QCA_CLD_BLD_DIR/qcacld-2.0/Android.mk
# Android makefile for the WLAN Module

ifeq ($(BOARD_HAS_QCOM_WLAN), true)
WLAN_CHIPSET := qca_cld
WLAN_SELECT := CONFIG_QCA_CLD_WLAN=m

KERNEL_TO_BUILD_ROOT_OFFSET := ../../../

# Check for supported kernel
ifeq ($(TARGET_KERNEL_VERSION),3.18)
$(info "WLAN: supported kernel detected, building qcacld-2.0")
endif

LOCAL_PATH := $(call my-dir)

# This makefile is only for DLKM
ifneq ($(findstring vendor,$(LOCAL_PATH)),)

WLAN_PROPRIETARY := 0
WLAN_OPEN_SOURCE := 1
WLAN_BLD_DIR := vendor/qcom/opensource/wlan

DLKM_DIR := $(TOP)/device/qcom/common/dlkm

###########################################################
# This is set once per LOCAL_PATH, not per (kernel) module
KBUILD_OPTIONS += WLAN_ROOT=$(KERNEL_TO_BUILD_ROOT_OFFSET)$(WLAN_BLD_DIR)/qcacld-2.0
# We are actually building wlan.ko here, as per the
# requirement we are specifying <chipset>_wlan.ko as LOCAL_MODULE.
# This means we need to rename the module to <chipset>_wlan.ko
# after wlan.ko is built.
KBUILD_OPTIONS += MODNAME=wlan
KBUILD_OPTIONS += CONFIG_CLD_HL_SDIO_CORE=y
KBUILD_OPTIONS += CONFIG_QCACLD_WLAN_LFR3=y
KBUILD_OPTIONS += CONFIG_PRIMA_WLAN_OKC=y
KBUILD_OPTIONS += CONFIG_PRIMA_WLAN_11AC_HIGH_TP=y
KBUILD_OPTIONS += CONFIG_WLAN_FEATURE_11W=y
KBUILD_OPTIONS += CONFIG_WLAN_FEATURE_LPSS=y
KBUILD_OPTIONS += CONFIG_QCOM_VOWIFI_11R=y
KBUILD_OPTIONS += CONFIG_WLAN_FEATURE_NAN=y
KBUILD_OPTIONS += CONFIG_WLAN_FEATURE_NAN_DATAPATH=y
KBUILD_OPTIONS += CONFIG_QCOM_TDLS=y
KBUILD_OPTIONS += CONFIG_QCOM_LTE_COEX=y
KBUILD_OPTIONS += CONFIG_WLAN_SYNC_TSF=y
KBUILD_OPTIONS += CONFIG_WLAN_FEATURE_MEMDUMP=y
KBUILD_OPTIONS += CONFIG_WLAN_OFFLOAD_PACKETS=y
KBUILD_OPTIONS += CONFIG_QCA_WIFI_AUTOMOTIVE_CONC=y
KBUILD_OPTIONS += CONFIG_WLAN_UDP_RESPONSE_OFFLOAD=y
KBUILD_OPTIONS += CONFIG_WLAN_FEATURE_RX_WAKELOCK=y
KBUILD_OPTIONS += CONFIG_WLAN_WOW_PULSE=y
KBUILD_OPTIONS += BOARD_PLATFORM=$(TARGET_BOARD_PLATFORM)
KBUILD_OPTIONS += $(WLAN_SELECT)
KBUILD_OPTIONS += WLAN_OPEN_SOURCE=$(WLAN_OPEN_SOURCE)
KBUILD_OPTIONS += CFLAGS_MODULE=-Wno-pointer-sign\
-Wno-unused-but-set-variable

#module to be built for all user,userdebug and eng tags
include $(CLEAR_VARS)
LOCAL_MODULE              := $(WLAN_CHIPSET)_wlan.ko
LOCAL_MODULE_KBUILD_NAME  := wlan.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(TARGET_OUT)/lib/modules/$(WLAN_CHIPSET)


# Assign external kernel modules to the DLKM class
LOCAL_MODULE_CLASS := DLKM

# Set the default install path to system/lib/modules
LOCAL_MODULE_PATH := $(strip $(LOCAL_MODULE_PATH))
ifeq ($(LOCAL_MODULE_PATH),)
  LOCAL_MODULE_PATH := $(TARGET_OUT)/lib/modules
endif

# Set the default Kbuild file path to LOCAL_PATH
KBUILD_FILE := $(strip $(KBUILD_FILE))
ifeq ($(KBUILD_FILE),)
  KBUILD_FILE := $(LOCAL_PATH)/Kbuild
endif

# Get rid of any whitespace
LOCAL_MODULE_KBUILD_NAME := $(strip $(LOCAL_MODULE_KBUILD_NAME))

include $(BUILD_SYSTEM)/base_rules.mk


# The kernel build system doesn't support parallel kernel module builds
# that share the same output directory. Thus, in order to build multiple
# kernel modules that reside in a single directory (and therefore have
# the same output directory), there must be just one invocation of the
# kernel build system that builds all the modules of a given directory.
#
# Therefore, all kernel modules must depend on the same, unique target
# that invokes the kernel build system and builds all of the modules
# for the directory. The $(KBUILD_TARGET) target serves this purpose.
# To ensure the value of KBUILD_TARGET is unique, it is essentially set
# to the path of the source directory, i.e. LOCAL_PATH.
#
# Since KBUILD_TARGET is used as a target and a variable name, it should
# not contain characters other than letters, numbers, and underscores.
KBUILD_TARGET := $(strip            \
                   $(subst .,_,     \
                     $(subst -,_,   \
                       $(subst :,_, \
                         $(subst /,_,$(LOCAL_PATH))))))

# Intermediate directory where the kernel modules are created
# by the kernel build system. Ideally this would be the same
# directory as LOCAL_BUILT_MODULE, but because we're using
# relative paths for both O= and M=, we don't have much choice
KBUILD_OUT_DIR := $(TARGET_OUT_INTERMEDIATES)/$(LOCAL_PATH)

# Path to the intermediate location where the kernel build
# system creates the kernel module.
KBUILD_MODULE := $(KBUILD_OUT_DIR)/$(LOCAL_MODULE)

# Since we only invoke the kernel build system once per directory,
# each kernel module must depend on the same target.
$(KBUILD_MODULE): kbuild_out := $(KBUILD_OUT_DIR)/$(LOCAL_MODULE_KBUILD_NAME)
$(KBUILD_MODULE): $(KBUILD_TARGET)
ifneq "$(LOCAL_MODULE_KBUILD_NAME)" ""
	mv -f $(kbuild_out) $@
endif

# Simply copy the kernel module from where the kernel build system
# created it to the location where the Android build system expects it.
# If LOCAL_MODULE_DEBUG_ENABLE is set, strip debug symbols. So that,
# the final images generated by ABS will have the stripped version of
# the modules
$(LOCAL_BUILT_MODULE): $(KBUILD_MODULE) | $(ACP)
ifneq "$(LOCAL_MODULE_DEBUG_ENABLE)" ""
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_STRIP) --strip-debug $< -o $@
else
	$(transform-prebuilt-to-target)
endif

# This should really be cleared in build/core/clear-vars.mk, but for
# the time being, we need to clear it ourselves
LOCAL_MODULE_KBUILD_NAME :=
LOCAL_MODULE_DEBUG_ENABLE :=

# Ensure the kernel module created by the kernel build system, as
# well as all the other intermediate files, are removed during a clean.
$(cleantarget): PRIVATE_CLEAN_FILES := $(PRIVATE_CLEAN_FILES) $(KBUILD_OUT_DIR)

# To ensure KERNEL_OUT and TARGET_PREBUILT_INT_KERNEL are defined,
# kernel/AndroidKernel.mk must be included. While m and regular
# make builds will include kernel/AndroidKernel.mk, mm and mmm builds
# do not. Therefore, we need to explicitly include kernel/AndroidKernel.mk.
# It is safe to include it more than once because the entire file is
# guarded by "ifeq ($(TARGET_PREBUILT_KERNEL),) ... endif".
include vendor/cm/build/tasks/kernel.mk

# Kernel modules have to be built after:
#  * the kernel config has been created
#  * host executables, like scripts/basic/fixdep, have been built
#    (otherwise parallel invocations of the kernel build system will
#    fail as they all try to compile these executables at the same time)
#  * a full kernel build (to make module versioning work)
#
# For these reasons, kernel modules are dependent on
# TARGET_PREBUILT_INT_KERNEL which will ensure all of the above.
#
# NOTE: Due to a bug in the kernel build system when using a Kbuild file
#       and relative paths for both O= and M=, the Kbuild file must
#       be copied to the output directory.
#
# NOTE: The following paths are equivalent:
#         $(KBUILD_OUT_DIR)
#         $(KERNEL_OUT)/../$(LOCAL_PATH)
.PHONY: $(KBUILD_TARGET)
$(KBUILD_TARGET): local_path     := $(LOCAL_PATH)
$(KBUILD_TARGET): kbuild_out_dir := $(KBUILD_OUT_DIR)
$(KBUILD_TARGET): kbuild_options := $(KBUILD_OPTIONS)
$(KBUILD_TARGET): $(TARGET_PREBUILT_INT_KERNEL)
	@mkdir -p $(kbuild_out_dir)
	$(hide) cp -f $(local_path)/Kbuild $(kbuild_out_dir)/Kbuild
	$(MAKE) -C /lineage/kernel/samsung/universal7880 M=$(KERNEL_TO_BUILD_ROOT_OFFSET)$(local_path) O=$(KERNEL_OUT) ARCH=arm64 CROSS_COMPILE=aarch64-linux-android- modules $(kbuild_options)
# This is a fix for path issues, due to lineage kernel located in subfolders of kernel
	$(hide) cp -f $(KERNEL_OUT)/$(KERNEL_TO_BUILD_ROOT_OFFSET)$(local_path)/wlan.ko $(KERNEL_OUT)/../$(local_path)/wlan.ko

# Once the KBUILD_OPTIONS variable has been used for the target
# that's specific to the LOCAL_PATH, clear it. If this isn't done,
# then every kernel module would need to explicitly set KBUILD_OPTIONS,
# or the variable would have to be cleared in 'include $(CLEAR_VARS)'
# which would require a change to build/core.
KBUILD_OPTIONS :=
###########################################################

$(shell ln -sf /persist/wlan_mac.bin $(TARGET_OUT_ETC)/firmware/wlan/qca_cld/wlan_mac.bin)

endif # WLAN enabled check
endif # DLKM

EOF


###################################################################################################
# CUSTOM PART END                                                                                 #
###################################################################################################

# Done
write_footers

if [ ! -z $DEVICE ]; then
    setup_vendor "$DEVICE" "$VENDOR" "$CM_ROOT"

    # Copyright headers and guards
    write_headers

    # Blobs
    write_makefiles "$MY_DIR"/../"$DEVICE"/proprietary-files.txt

    # Done
    write_footers
fi
