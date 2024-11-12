#include <stdlib.h>
#include <string.h>
#include "prop.h"

bool propm_init(prop_manager_t *pm)
{
    if (!pm)
        return false;
    pm->last_free_index = 0;
    pm->props = vec_create(sizeof(prop_t));
    return vec_fast_resize(&pm->props, PROP_MANAGER_BASE_SIZE);
}

void propm_deinit(prop_manager_t *am)
{
    if (!am)
        return;
    vec_free_data(&am->props);
}

prop_t *propm_new(prop_manager_t *am, const char *name,
    const prop_asset_map_t *asset_map)
{
    prop_t *prop = NULL;

    if (!am || !name)
        return NULL;
    prop = &VEC_SAFE_AT(prop_t, &am->props, am->last_free_index);
    strncpy(prop->name, name, PROP_NAME_SIZE);
    if (asset_map) {
        prop->asset_map = *asset_map;
    } else {
        memset(&prop->asset_map, 0, sizeof(prop_asset_map_t));
        prop->asset_map.asset_mode = NO_ASSET_MODE;
    }
    am->last_free_index++;
    return prop;
}
