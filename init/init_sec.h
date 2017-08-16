#ifndef INIT_SEC_H
#define INIT_SEC_H

#include <string.h>

enum device_variant {
    VARIANT_A520F = 0,
    VARIANT_A520W,
    VARIANT_A520K,
    VARIANT_A520L,
    VARIANT_A520S,
    VARIANT_MAX
};

typedef struct {
    std::string model;
    std::string codename;
} variant;

static const variant international_models = {
    .model = "SM-A520F",
    .codename = "a5y17lte"
};

static const variant canada_models = {
    .model = "SM-A520W",
    .codename = "a5y17ltecan"
};

static const variant korea_docomo_models = {
    .model = "SM-A520K",
    .codename = "a5y17ltektt"
};

static const variant korea_uplus_models = {
    .model = "SM-A520L",
    .codename = "a5y17ltelgt"
};

static const variant korea_telecom_models = {
    .model = "SM-A520S",
    .codename = "a5y17lteskt"
};

static const variant *all_variants[VARIANT_MAX] = {
    &international_models,
    &canada_models,
    &korea_docomo_models,
    &korea_uplus_models,
    &korea_telecom_models
};

#endif // INIT_SEC_H
