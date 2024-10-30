#include <stdlib.h>
#include <string.h>
#include "assets.h"


bool assetm_init(asset_manager_t *am)
{
    if (!am)
        return false;
    am->last_free_index = 0;
    am->assets = vec_create(sizeof(asset_t));
    vec_fast_resize(&am->assets, ASSET_MANAGER_BASE_SIZE);
    return true;
}

void assetm_deinit(asset_manager_t *am)
{
    vec_free_data(&am->assets);
}

ssize_t assetm_add(asset_manager_t *am, const char *name,
    const char *path)
{
    asset_t *asset = NULL;

    if (!am || !name || !path)
        return -1;
    asset = &VEC_SAFE_AT(asset_t, &am->assets, am->last_free_index);
    strncpy(asset->name, name, ASSET_NAME_SIZE);
    strncpy(asset->path, path, ASSET_PATH_SIZE);
    return am->last_free_index++;
}
