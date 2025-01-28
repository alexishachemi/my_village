#pragma once

#include <stdbool.h>
#include <sys/types.h>
#include "biome.h"
#include "orientation.h"
#include "prop.h"
#include "terrain.h"

typedef struct {
    prop_t *prop;
    orient_t orient;
} placed_prop_t;

typedef struct {
    terrain_t *terrain;
    list_t props; // placed_prop_t
    biome_t *biome;
} tile_t;

bool tile_init(tile_t *tile);
void tile_deinit(tile_t *tile);
bool tile_place_prop(tile_t *tile, prop_t *prop, orient_t orient);
void tile_remove_prop(tile_t *tile, prop_t *prop);
void tile_remove_props(tile_t *tile);