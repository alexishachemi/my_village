#pragma once

#include "registry.h"
#include "asset.h"
#include "prop.h"
#include <sys/types.h>

#define W_INIT_REG(wp, t, s) (reg_init(&wp->t##_reg, sizeof(t##_t), s))
#define W_DEINIT_REG(wp, t) (reg_deinit(&wp->t##_reg))

typedef struct {
    reg_t asset_reg; // asset_t
    reg_t prop_reg; // prop_t
} world_t;

bool world_init(world_t *world);
void world_deinit(world_t *world);

// Asset

ssize_t world_new_asset(world_t *world, const char *name, const char *path);
ssize_t world_get_asset(world_t *world, const char *name);
char *world_get_asset_path(world_t *world, const char *name);
asset_t *world_get_asset_ptr(world_t *world, ssize_t asset_id);

// Prop

ssize_t world_new_prop(world_t *world, const char *name);
ssize_t world_get_prop(world_t *world, const char *name);
prop_t *world_get_prop_ptr(world_t *world, ssize_t prop_id);