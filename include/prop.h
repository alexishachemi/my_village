#pragma once

#include <sys/types.h>
#include <stdbool.h>
#include "asset.h"
#include "orientation.h"
#include "str.h"
#include "registry.h"
#include "v2.h"

#define PROP_REGISTRY_BASE_SIZE 128
#define PROP_CHILD_REGISTRY_BASE_SIZE 1
#define PROP_CHILD_NAME_FORMAT "Child prop of [%s] at [%d,%d]"

typedef enum { PTYPE_PARENT, PTYPE_CHILD } prop_type_t;
typedef enum { AMODE_NONE, AMODE_MONO, AMODE_MULTI } prop_asset_mode_t;
typedef int8_t z_index_t;

typedef struct {
    prop_asset_mode_t asset_mode;
    union {
        asset_t *asset;
        struct {
            asset_t *asset_up;
            asset_t *asset_down;
            asset_t *asset_left;
            asset_t *asset_right;
        };
    };
} prop_asset_map_t;

typedef struct prop_s prop_t;

struct prop_s {
    name_t name;
    prop_type_t type;
    prop_asset_map_t asset_map;
    union {
        struct { // PTYPE_PARENT
            z_index_t z_index;
            bool has_child;
            reg_t children;
        };
        struct { //PTYPE_CHILD
            prop_t *parent;
            v2_t offset;
        };
    };
};

bool prop_init(prop_t *prop, const char *name);
void prop_deinit(prop_t *prop);

bool prop_set_z_index(prop_t *prop, z_index_t z_index);
bool prop_set_mono_asset(prop_t *prop, asset_t *asset);
bool prop_set_multi_asset(
    prop_t *prop,
    asset_t *asset_up,
    asset_t *asset_down,
    asset_t *asset_left,
    asset_t *asset_right
);
asset_t *prop_get_asset(const prop_t *prop, orient_t orient);

bool prop_has_child(const prop_t *prop);
prop_t *prop_add_child(prop_t *prop, v2_t offset);
prop_t *prop_get_child(prop_t *prop, v2_t offset);
list_t *prop_get_coverage(prop_t *prop, v2_t origin, orient_t orient);