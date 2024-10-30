#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assets.h"

const char *assetm_get_by_name(asset_manager_t *am, const char *name)
{
    asset_t *asset = NULL;

    if (!am || !name)
        return NULL;  
    for (size_t i = 0; i < am->last_free_index; i++) {
        asset = &VEC_FAST_AT(asset_t, &am->assets, i);
        if (strcmp(asset->name, name) == 0)
            return asset->path;
    }  
    return NULL;
}

const char *assetm_get_by_id(asset_manager_t *am, size_t id)
{
    if (!am || id >= am->last_free_index)
        return NULL;
    return VEC_FAST_AT(asset_t, &am->assets, id).path;
}

void assetm_print(asset_manager_t *am)
{
    asset_t *asset = NULL;

    for (size_t i = 0; i < am->assets.size; i++) {
        asset = &VEC_FAST_AT(asset_t, &am->assets, i);
        if (i != 0)
            printf("---");
        printf("[%s]", i < am->last_free_index ? asset->name : "");
    }
    printf("\n");
}

void assetm_print_loaded(asset_manager_t *am)
{
    asset_t *asset = NULL;

    for (size_t i = 0; i < am->last_free_index; i++) {
        asset = &VEC_FAST_AT(asset_t, &am->assets, i);
        printf("%s -> %s\n", asset->name, asset->path);
    }
}
