#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "log.h"
#include "property_service.h"
#include "util.h"
#include "vendor_init.h"

#include "init_sec.h"


static std::string bootloader;

static void property_override(char const prop[], char const value[]) {
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

static device_variant parse_variant(std::string bl) {
    device_variant ret = VARIANT_MAX;

    if (bl.find("A520F") != std::string::npos)
        ret = VARIANT_A520F;

    return ret;
}

static device_variant get_variant_from_cmdline()
{
    bootloader = property_get("ro.bootloader");
    device_variant ret = parse_variant(bootloader);

    if (ret >= VARIANT_MAX) {
        INFO("Unknown bootloader id: %s, forcing international (F) variant\n", bootloader.c_str());
        ret = VARIANT_A520F;
    }

    return ret;
}

void vendor_load_properties()
{
    const device_variant variant = get_variant_from_cmdline();

    std::string model, device, description, fingerprint;

    switch (variant) {
    case VARIANT_A520F:
        model = "SM-A520F";
        device = "a5y17lte";
        description = "a5y17ltexx-user 7.0 NRD90M A520FXXU2BQH4 release-keys";
        fingerprint = "samsung/a5y17ltexx/a5y17lte:7.0/NRD90M/A520FXXU2BQH4:user/release-keys";
        break;
    default:
        break;
    }

    INFO("Found bootloader id: %s setting build properties for: %s device\n", bootloader.c_str(), device.c_str());

    property_override("ro.product.model", model.c_str());
    property_override("ro.product.device", device.c_str());
    property_override("ro.build.product", device.c_str());
    property_override("ro.build.description", description.c_str());
    property_override("ro.build.fingerprint", fingerprint.c_str());
}
