#pragma once

#include <sys/types.h>
#include "orientation.h"
#include "cvec.h"

#define PROP_NAME_SIZE 32
#define PROP_MANAGER_BASE_SIZE 128

typedef enum { MONO_ASSET, MULTI_ASSET } prop_asset_mode_t;

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
    char name[PROP_NAME_SIZE];
    prop_asset_map_t asset_map;
} prop_t;

typedef struct {
    vec_t props;
    size_t last_free_index;
} prop_manager_t;

// Factory

bool propm_init(prop_manager_t *pm);
void propm_deinit(prop_manager_t *pm);

/// @brief Creates a new prop in the prop manager
/// @param pm the prop manager to add the props to
/// @param name the name of the prop
/// @param asset_map the asset map of the prop
/// @return the newly added prop
prop_t *propm_new(prop_manager_t *pm, const char *name,
    const prop_asset_map_t *asset_map);

// Usage

/// @brief Gets the asset id of a prop for a specific orientation
/// @param prop the prop to get the asset id from
/// @param orient the desired orientation. If the prop is mono asset
/// (only one asset for all directions), the same id is returned no matter
/// the orientation
/// @return the desired asset id, -1 on error
ssize_t prop_get_asset_id(const prop_t *prop, orient_t orient);
