#pragma once

#include "cvec.h"

#define ASSET_NAME_SIZE 32
#define ASSET_PATH_SIZE 128
#define ASSET_MANAGER_BASE_SIZE 128

typedef struct {
    char name[ASSET_NAME_SIZE];
    char path[ASSET_PATH_SIZE];
} asset_t;

typedef struct {
    vec_t assets;
    size_t last_free_index;
} asset_manager_t;

bool assetm_init(asset_manager_t *am);
void assetm_deinit(asset_manager_t *am);
ssize_t assetm_add(asset_manager_t *am, const char *name, const char *path);

const char *assetm_get_by_name(asset_manager_t *am, const char *name);
const char *assetm_get_by_id(asset_manager_t *am, size_t id);
void assetm_print(asset_manager_t *am);
void assetm_print_loaded(asset_manager_t *am);
