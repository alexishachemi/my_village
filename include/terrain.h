#pragma once

#include <stdbool.h>
#include <sys/types.h>
#include "str.h"

#define TERRAIN_REGISTRY_BASE_SIZE 128

typedef struct {
    name_t name;
    ssize_t asset_id;
} terrain_t;

bool terrain_init(terrain_t *terrain, const char *name, size_t asset_id);