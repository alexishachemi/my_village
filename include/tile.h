#pragma once

#include <stdbool.h>
#include <sys/types.h>
#include "biome.h"
#include "orientation.h"
#include "prop.h"
#include "terrain.h"

typedef struct {
    terrain_t *terrain;
    prop_t *prop;
    biome_t *biome;
    orient_t prop_orient;
} tile_t;

bool tile_init(tile_t *tile);