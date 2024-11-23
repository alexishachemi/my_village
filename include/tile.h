#pragma once

#include "orientation.h"
#include <stdbool.h>
#include <sys/types.h>

typedef struct {
    ssize_t terrain_id;
    ssize_t prop_id;
    ssize_t biome_id;
    orient_t prop_orient;
} tile_t;

bool tile_init(tile_t *tile);