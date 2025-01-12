#include "tile.h"
#include "linked.h"
#include "orientation.h"
#include "prop.h"

bool tile_init(tile_t *tile)
{
    if (!tile)
        return false;
    tile->terrain = NULL;
    tile->biome = NULL;
    list_init(&tile->props);
    return true;
}

void tile_deinit(tile_t *tile)
{
    if (!tile)
        return;
    list_clear_free(&tile->props);
}

bool tile_place_prop(tile_t *tile, prop_t *prop, orient_t orient)
{
    placed_prop_t placed = {.prop = prop, .orient = orient};

    if (!tile || !prop)
        return false;
    return list_add_copy(&tile->props, &placed, sizeof(placed_prop_t));
}

static bool placed_is_prop(placed_prop_t *placed, prop_t *prop)
{
    return placed && prop && placed->prop == prop;
}

void tile_remove_prop(tile_t *tile, prop_t *prop)
{
    if (!tile || !prop)
        return;
    list_remove_if_free(&tile->props, (comparator_t)placed_is_prop, prop);
}

void tile_remove_props(tile_t *tile)
{
    if (!tile)
        return;
    list_clear_free(&tile->props);
}

#ifdef TEST
#include <criterion/criterion.h>

Test(tile, init)
{
    tile_t tile = {0};

    cr_assert(tile_init(&tile));
    cr_assert_null(tile.terrain);
    cr_assert_null(tile.biome);
    tile_deinit(&tile);
}

Test(tile, place_prop)
{
    tile_t tile = {0};
    prop_t prop = {0};
    orient_t orient = ORIENT_DOWN;
    placed_prop_t *placed = NULL;

    cr_assert(tile_init(&tile));    
    cr_assert_not(tile_place_prop(NULL, &prop, orient));
    cr_assert_not(tile_place_prop(&tile, NULL, orient));
    cr_assert_null(tile.props.head);
    cr_assert(tile_place_prop(&tile, &prop, orient));
    cr_assert_not_null(tile.props.head);
    placed = tile.props.head->data;
    cr_assert_not_null(placed);
    cr_assert_eq(placed->prop, &prop);
    cr_assert_eq(placed->orient, orient);
    tile_deinit(&tile);
}

#endif