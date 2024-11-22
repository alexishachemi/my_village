#pragma once

#include "str.h"
#include "registry.h"
#include <sys/types.h>

#define BIOME_REGISTRY_BASE_SIZE 16
#define BIOME_HERITAGE_BASE_SIZE 5
#define BIOME_TERRAIN_BASE_SIZE  8
#define BIOME_PROPS_BASE_SIZE    64

typedef struct {
    name_t name;
    reg_t heritage; // size_t (parent-biome id)
    reg_t terrains; // size_t (terrain id)
    reg_t props;    // size_t (prop id)
} biome_t;

bool biome_init(biome_t *biome, const char *name);
void biome_deinit(biome_t *biome);
bool biome_add_terrain(biome_t *biome, size_t terrain_id);
bool biome_add_terrain_by_name(biome_t *biome, reg_t *terrains, const char *name);
bool biome_add_prop(biome_t *biome, size_t prop_id);
bool biome_add_prop_by_name(biome_t *biome, reg_t *props, const char *name);