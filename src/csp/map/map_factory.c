#include "csp.h"

static bool init_layers(csp_map_t *map)
{
    map->cells = vec_create(sizeof(csp_cell_t));
    return vec_resize(&map->cells, (map->size.x * map->size.y) * map->layers);
}

bool csp_map_init(csp_map_t *map, csp_room_t *room, v2_t size)
{
    if (!map || !room || size.x < 2 || size.y < 2 || room->layers == 0)
        return false;
    map->size = size;
    map->layers = room->layers;
    map->area = size.x * size.y;
    map->objs = &room->objs;
    map->terrain = room->terrain;
    map->global_constraints = &room->constraints;
    list_init(&map->placement_history);
    if (!init_layers(map)) {
        csp_map_deinit(map);
        return false;
    }
    return true;
}

void csp_map_deinit(csp_map_t *map)
{
    if (!map)
        return;
    list_clear(&map->placement_history, (callback_t)list_destroy_free);
    vec_free_data(&map->cells);
    map->size = (v2_t){0, 0};
    map->layers = 0;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_map, init)
{
    csp_map_t map = {0};
    v2_t size = {10, 20};
    int layers = 3;
    csp_room_t room = {0};

    cr_assert(csp_room_init(&room, "foo"));
    cr_assert(csp_map_init(&map, &room, size));
    cr_assert_eq(map.size.x, 10);
    cr_assert_eq(map.size.y, 20);
    cr_assert_eq(map.area, 200);
    cr_assert_eq(map.placement_history.size, 0);
    cr_assert_eq(map.cells.size, (size.x * size.y) * layers);
    cr_assert_eq(map.objs, &room.objs);
    csp_map_deinit(&map);
}

#endif