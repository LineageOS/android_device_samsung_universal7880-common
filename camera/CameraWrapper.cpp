/*
 * Copyright (C) 2017, The LineageOS Project
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

/**
* @file CameraWrapper.cpp
*
* This file wraps a vendor camera module.
*
*/

#define LOG_NDEBUG 0

#define LOG_TAG "CameraWrapper"
#include <cutils/log.h>

#include <hardware/hardware.h>
#include <hardware/camera3.h>
#include <utils/threads.h>
#include <camera/CameraMetadata.h>

#include "SECCameraProperties.h"

static android::Mutex gCameraWrapperLock;
static camera_module_t *gVendorModule = 0;

static int camera_device_open(const hw_module_t *module, const char *name,
        hw_device_t **device);
static int camera_get_number_of_cameras(void);
static int camera_get_camera_info(int camera_id, struct camera_info *info);
static int camera_set_callbacks(const camera_module_callbacks_t *callbacks);
static void camera_get_vendor_tag_ops(vendor_tag_ops_t *ops);
static int camera_open_legacy(const struct hw_module_t *module,
        const char *id, uint32_t halVersion, struct hw_device_t **device);
static int camera_set_torch_mode(const char *camera_id, bool enabled);
static int camera_init();

static struct hw_module_methods_t camera_module_methods = {
    .open = camera_device_open
};

camera_module_t HAL_MODULE_INFO_SYM = {
    .common = {
         .tag = HARDWARE_MODULE_TAG,
         .module_api_version = CAMERA_MODULE_API_VERSION_2_4,
         .hal_api_version = HARDWARE_HAL_API_VERSION,
         .id = CAMERA_HARDWARE_MODULE_ID,
         .name = "Samsung Camera Wrapper",
         .author = "The LineageOS Project",
         .methods = &camera_module_methods,
         .dso = NULL, /* remove compilation warnings */
         .reserved = {0}, /* remove compilation warnings */
    },
    .get_number_of_cameras = camera_get_number_of_cameras,
    .get_camera_info = camera_get_camera_info,
    .set_callbacks = camera_set_callbacks,
    .get_vendor_tag_ops = camera_get_vendor_tag_ops,
    .open_legacy = camera_open_legacy,
    .set_torch_mode = camera_set_torch_mode,
    .init = camera_init,
    .reserved = {0}, /* remove compilation warnings */
};

typedef struct wrapper_camera_device {
    camera3_device_t base;
    int id;
    camera3_device_t *vendor;
} wrapper_camera_device_t;

#define VENDOR_CALL(device, func, ...) ({ \
    wrapper_camera_device_t *__wrapper_dev = (wrapper_camera_device_t*) device; \
    __wrapper_dev->vendor->ops->func(__wrapper_dev->vendor, ##__VA_ARGS__); \
})

/*******************************************************************
 * common functions
 *******************************************************************/

static int check_vendor_module()
{
    int rv = 0;
    ALOGV("%s", __FUNCTION__);

    if (gVendorModule)
        return 0;

    rv = hw_get_module_by_class("camera", "vendor",
            (const hw_module_t**)&gVendorModule);

    if (rv)
        ALOGE("failed to open vendor camera module");
    return rv;
}

/*******************************************************************
 * implementation of camera3_device_ops functions
 *******************************************************************/

int camera_initialize(const struct camera3_device *device,
    const camera3_callback_ops_t *callback_ops)
{
    if(!device)
        return -ENODEV;

    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device, (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    return VENDOR_CALL(device, initialize, callback_ops);
}

int camera_configure_streams(const struct camera3_device *device,
    camera3_stream_configuration_t *stream_list)
{
    if(!device)
        return -ENODEV;

    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device, (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    return VENDOR_CALL(device, configure_streams, stream_list);
}

const camera_metadata_t * camera_construct_default_request_settings(
    const struct camera3_device *device , int type)
{
    if(!device)
        return NULL;

    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device, (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    android::CameraMetadata metadata;
    metadata = VENDOR_CALL(device, construct_default_request_settings, type);

    /* enable phase detection auto focus by default */
    int32_t pafMode[1] = {PAF_MODE_ON};
    metadata.update(PAF_MODE, pafMode, 1);

#ifdef HAS_OIS
    /* enable optical image stabilization by default */
    uint8_t oisMode[1] = {ANDROID_LENS_OPTICAL_STABILIZATION_MODE_ON};
    metadata.update(ANDROID_LENS_OPTICAL_STABILIZATION_MODE, oisMode, 1);

    int32_t oisOpMode[1];
    /* video mode ois */
    if (type == CAMERA3_TEMPLATE_VIDEO_RECORD) {
        oisOpMode[0] = OIS_OPERATION_MODE_VIDEO;
    /* picture mode ois */
    } else {
        oisOpMode[0] = OIS_OPERATION_MODE_PICTURE;
    }
    metadata.update(OIS_OPERATION_MODE, oisOpMode, 1);
#endif

    return metadata.release();
}

int camera_process_capture_request(const struct camera3_device *device,
    camera3_capture_request_t *request)
{
    if(!device)
        return -ENODEV;

    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device, (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    return VENDOR_CALL(device, process_capture_request, request);
}

void camera_get_metadata_vendor_tag_ops(const struct camera3_device *device,
    vendor_tag_query_ops_t *ops)
{
    if(!device)
        return;

    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device, (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    VENDOR_CALL(device, get_metadata_vendor_tag_ops, ops);
}

void camera_dump(const struct camera3_device *device, int fd)
{
    if(!device)
        return;

    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device, (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    return VENDOR_CALL(device, dump, fd);
}

int camera_flush(const struct camera3_device *device)
{
    if(!device)
        return -ENODEV;

    ALOGV("%s->%08X->%08X", __FUNCTION__, (uintptr_t)device, (uintptr_t)(((wrapper_camera_device_t*)device)->vendor));

    return VENDOR_CALL(device, flush);
}

extern "C" void heaptracker_free_leaked_memory(void);

int camera_device_close(hw_device_t* device)
{
    int ret = 0;
    wrapper_camera_device_t *wrapper_dev = NULL;

    ALOGI("%s", __FUNCTION__);

    android::Mutex::Autolock lock(gCameraWrapperLock);

    if (!device) {
        ret = -EINVAL;
        goto done;
    }

    wrapper_dev = (wrapper_camera_device_t*) device;

    wrapper_dev->vendor->common.close((hw_device_t*)wrapper_dev->vendor);
    if (wrapper_dev->base.ops)
        free(wrapper_dev->base.ops);
    free(wrapper_dev);
done:
#ifdef HEAPTRACKER
    heaptracker_free_leaked_memory();
#endif
    return ret;
}

/*******************************************************************
 * implementation of camera_module functions
 *******************************************************************/

static int camera_device_open(const hw_module_t *module, const char *name,
        hw_device_t **device)
{
    int rv = 0;
    int num_cameras = 0;
    int cameraid;
    wrapper_camera_device_t* camera_device = NULL;
    camera3_device_ops_t* camera_ops = NULL;

    android::Mutex::Autolock lock(gCameraWrapperLock);

    ALOGI("camera_device open");

    if (name != NULL) {
        if (check_vendor_module())
            return -EINVAL;

        cameraid = atoi(name);
        num_cameras = gVendorModule->get_number_of_cameras();

        if(cameraid > num_cameras)
        {
            ALOGE("camera service provided cameraid out of bounds, "
                    "cameraid = %d, num supported = %d",
                    cameraid, num_cameras);
            rv = -EINVAL;
            goto fail;
        }

        camera_device = (wrapper_camera_device_t*)malloc(sizeof(*camera_device));
        if(!camera_device)
        {
            ALOGE("camera_device allocation fail");
            rv = -ENOMEM;
            goto fail;
        }
        memset(camera_device, 0, sizeof(*camera_device));
        camera_device->id = cameraid;

        rv = gVendorModule->common.methods->open(
                (const hw_module_t*)gVendorModule, name,
                (hw_device_t**)&(camera_device->vendor));
        if (rv) {
            ALOGE("vendor camera open fail");
            goto fail;
        }
        ALOGI("%s: got vendor camera device 0x%08X",
                __FUNCTION__, (uintptr_t)(camera_device->vendor));

        camera_ops = (camera3_device_ops_t*)malloc(sizeof(*camera_ops));
        if(!camera_ops)
        {
            ALOGE("camera_ops allocation fail");
            rv = -ENOMEM;
            goto fail;
        }

        memset(camera_ops, 0, sizeof(*camera_ops));

        camera_device->base.common.tag = HARDWARE_DEVICE_TAG;
        camera_device->base.common.version = CAMERA_DEVICE_API_VERSION_3_4;
        camera_device->base.common.module = (hw_module_t *)(module);
        camera_device->base.common.close = camera_device_close;
        camera_device->base.ops = camera_ops;

        camera_ops->initialize = camera_initialize;
        camera_ops->configure_streams = camera_configure_streams;
        camera_ops->register_stream_buffers = NULL;
        camera_ops->construct_default_request_settings = camera_construct_default_request_settings;
        camera_ops->process_capture_request = camera_process_capture_request;
        camera_ops->get_metadata_vendor_tag_ops = camera_get_metadata_vendor_tag_ops;
        camera_ops->dump = camera_dump;
        camera_ops->flush = camera_flush;

        *device = &camera_device->base.common;
    }

    return rv;

fail:
    if(camera_device) {
        free(camera_device);
        camera_device = NULL;
    }
    if(camera_ops) {
        free(camera_ops);
        camera_ops = NULL;
    }
    *device = NULL;
    return rv;
}

static int camera_get_number_of_cameras(void)
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return 0;

    return gVendorModule->get_number_of_cameras();
}

static int camera_get_camera_info(int camera_id, struct camera_info *info)
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return 0;

    return gVendorModule->get_camera_info(camera_id, info);
}

static int camera_set_callbacks(const camera_module_callbacks_t *callbacks)
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return 0;

    return gVendorModule->set_callbacks(callbacks);
}

static void camera_get_vendor_tag_ops(vendor_tag_ops_t *ops)
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return;

    gVendorModule->get_vendor_tag_ops(ops);
}

static int camera_open_legacy(const struct hw_module_t *module,
        const char *id, uint32_t halVersion, struct hw_device_t **device)
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return 0;

    return gVendorModule->open_legacy(module, id, halVersion, device);
}

static int camera_set_torch_mode(const char *camera_id, bool enabled)
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return 0;

    return gVendorModule->set_torch_mode(camera_id, enabled);
}

static int camera_init()
{
    ALOGV("%s", __FUNCTION__);
    if (check_vendor_module())
        return 0;

    return gVendorModule->init();
}
