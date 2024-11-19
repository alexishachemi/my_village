#pragma once

#include <stdbool.h>
#include "str.h"

#define ASSET_REGISTRY_BASE_SIZE 128

typedef struct {
    name_t name;
    path_t path;
} asset_t;

bool asset_init(asset_t *asset, const char *name, const char *path);