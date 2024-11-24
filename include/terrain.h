#pragma once

#include <stdbool.h>
#include <sys/types.h>
#include "asset.h"
#include "str.h"

#define TERRAIN_REGISTRY_BASE_SIZE 128

typedef struct {
    name_t name;
    asset_t *asset;
} terrain_t;

bool terrain_init(terrain_t *terrain, const char *name, asset_t *asset);