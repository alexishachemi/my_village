#pragma once

#include "registry.h"
#include "asset.h"

#define W_INIT_REG(wp, t, s) (reg_init(&wp->t##_reg, sizeof(t##_t), s))
#define W_DEINIT_REG(wp, t) (reg_deinit(&wp->t##_reg))

typedef struct {
    reg_t asset_reg; // asset_t
} world_t;

bool world_init(world_t *world);
void world_deinit(world_t *world);

// Asset

asset_t *world_new_asset(world_t *world, const char *name, const char *path);
asset_t *world_get_asset(world_t *world, const char *name);

