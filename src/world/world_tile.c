#include "chunk.h"
#include "orientation.h"
#include "prop.h"
#include "registry.h"
#include "v2.h"
#include "world.h"
#include <stdbool.h>
#include <stdio.h>

static bool valid_pos(world_t *world, v2_t pos)
{
    return pos.x >= 0 && pos.y >= 0
        && (unsigned int)pos.x < world->size && (unsigned int)pos.y < world->size;
}

tile_t *world_get_tile(world_t *world, v2_t pos)
{
    return chunk_get_tile_absolute(world_get_chunk(world, pos), pos);
}

bool world_place_terrain(world_t *world, v2_t pos, terrain_t *terrain)
{
    tile_t *tile = NULL;

    if (!world || !terrain || !valid_pos(world, pos))
        return false;
    tile = world_get_tile(world, pos);
    if (!tile)
        return false;
    tile->terrain = terrain;
    return true;
}

bool world_place_prop(world_t *world, prop_t *prop, v2_t pos, orient_t orientation, bool force)
{
    tile_t *tile = NULL;
    prop_t *child = NULL;
    v2_t new_pos = {0};

    if (!world || !prop || (!force && !world_can_place_prop(world, prop, pos, orientation)))
        return false;
    tile = world_get_tile(world, pos);
    if (!tile || (!force && tile->prop))
        return false;
    tile->prop = prop;
    tile->prop_orient = orientation;
    for (unsigned int i = 0; i < REG_SIZE(prop->children); i++) {
        child = REG_AT(prop_t, &prop->children, i);
        if (!child)
            break;
        new_pos = V2_ADD(pos, v2_orient(child->offset, orientation));
        tile = world_get_tile(world, new_pos);
        if (!tile)
            continue;
        tile->prop = child;
        tile->prop_orient = orientation;
    }
    return true;
}

bool world_can_place_prop(world_t *world, prop_t *prop, v2_t pos, orient_t orientation)
{
    prop_t *child = NULL;
    tile_t *tile = NULL;

    if (!world || !prop || !valid_pos(world, pos))
        return false;
    if (prop->type != PTYPE_PARENT || !prop->has_child)
        return true;
    for (unsigned int i = 0; i < REG_SIZE(prop->children); i++) {
        child = REG_AT(prop_t, &prop->children, i);
        if (!child)
            return false;
        tile = world_get_tile(world, V2_ADD(pos, v2_orient(child->offset, orientation)));
        if (!tile || tile->prop)
            return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, get_tile)
{
    world_t world = {0};

    cr_assert(world_init(&world, 100, 10));
    cr_assert_eq(world_get_tile(&world, (v2_t){0, 0}),
        chunk_get_tile_relative(REG_AT(chunk_t, &world.chunk_reg, 0), (v2_t){0, 0}));
    cr_assert_eq(world_get_tile(&world, (v2_t){10, 10}),
        chunk_get_tile_relative(REG_AT(chunk_t, &world.chunk_reg, 11), (v2_t){0, 0}));
    cr_assert_eq(world_get_tile(&world, (v2_t){5, 26}),
        chunk_get_tile_relative(REG_AT(chunk_t, &world.chunk_reg, 20), (v2_t){5, 6}));
}

Test(world, get_tile_bad)
{
    world_t world = {0};

    cr_assert(world_init(&world, 100, 10));
    cr_assert_eq(world_get_tile(&world, (v2_t){100, 0}), NULL);
    cr_assert_eq(world_get_tile(&world, (v2_t){0, 100}), NULL);
    cr_assert_eq(world_get_tile(&world, (v2_t){100, 100}), NULL);
    cr_assert_eq(world_get_tile(&world, (v2_t){-1, 12}), NULL);
}

#endif