#pragma once

#include <sys/types.h>
#include "prop.h"
#include "str.h"
#include "registry.h"
#include "terrain.h"

#define BIOME_REGISTRY_BASE_SIZE 16
#define BIOME_HERITAGE_BASE_SIZE 5
#define BIOME_TERRAIN_BASE_SIZE  8
#define BIOME_PROPS_BASE_SIZE    64

typedef struct {
    name_t name;
    reg_t parents; // (biome_t *)
    reg_t terrains; // (terrain_t *)
    reg_t props;    // (prop_t *)
} biome_t;

bool biome_init(biome_t *biome, const char *name);
void biome_deinit(biome_t *biome);
bool biome_add_terrain(biome_t *biome, terrain_t *terrain);
bool biome_add_prop(biome_t *biome, prop_t *prop);
bool biome_add_parent(biome_t *biome, biome_t *parent);