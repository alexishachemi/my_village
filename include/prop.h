#pragma once

#include <sys/types.h>
#include <stdbool.h>
#include "orientation.h"
#include "str.h"

#define PROP_REGISTRY_BASE_SIZE 128

typedef enum { AMODE_NONE, AMODE_MONO, AMODE_MULTI } prop_asset_mode_t;
typedef int8_t z_index_t;

typedef struct {
    prop_asset_mode_t asset_mode;
    union {
        size_t asset_id;
        struct {
            size_t asset_id_up;
            size_t asset_id_down;
            size_t asset_id_left;
            size_t asset_id_right;
        };
    };
} prop_asset_map_t;

typedef struct {
    name_t name;
    prop_asset_map_t asset_map;
    z_index_t z_index;
} prop_t;

bool prop_init(prop_t *prop, const char *name);
bool prop_set_z_index(prop_t *prop, z_index_t z_index);
bool prop_set_mono_asset(prop_t *prop, size_t asset_id);
bool prop_set_multi_asset(
    prop_t *prop,
    size_t asset_id_up,
    size_t asset_id_down,
    size_t asset_id_left,
    size_t asset_id_right
);
ssize_t prop_get_asset_id(const prop_t *prop, orient_t orient);
