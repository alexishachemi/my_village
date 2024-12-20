#include "csp.h"
#include "tile.h"
#include "v2.h"
#include "world.h"

static void apply(csp_map_t *map, world_t *world, v2_t tile_pos, v2_t cell_pos, unsigned int layer)
{
    csp_cell_t *cell = csp_map_get_cell(map, cell_pos, layer);
    tile_t *tile = world_get_tile(world, tile_pos);

    if (!cell || !tile)
        return;
    tile->prop = cell->occupant;
    tile->prop_orient = cell->occupant_orient;
    tile->terrain = map->floor;
}

static void apply_layer(csp_map_t *map, world_t *world, v2_t origin, unsigned int layer)
{
    v2_t pos = {0};

    for (int y = 0; y < map->size.y; y++) {
        for (int x = 0; x < map->size.x; x++) {
            pos = (v2_t){x, y};
            apply(map, world, V2_ADD(origin, pos), pos, layer);
        }
    }
}

static void clear_area(csp_map_t *map, world_t *world, v2_t origin)
{
    v2_t pos = {0};

    for (int y = 0; y < map->size.y; y++) {
        for (int x = 0; x < map->size.x; x++) {
            pos = (v2_t){x, y};
            world_remove_prop(world, V2_ADD(origin, pos));
        }
    }
}

bool csp_map_apply(csp_map_t *map, world_t *world, v2_t origin)
{
    if (!map || !world_pos_is_valid(world, origin))
        return false;
    clear_area(map, world, origin);
    for (unsigned int layer = 0; layer < 1; layer++) {
        apply_layer(map, world, origin, layer);
    }
    return true;
}
