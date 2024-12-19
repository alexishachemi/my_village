#include "csp.h"

static bool init_layers(csp_map_t *map)
{
    map->cells = vec_create(sizeof(csp_cell_t));
    return vec_resize(&map->cells, (map->size.x * map->size.y) * map->layers);
}

bool csp_map_init(csp_map_t *map, v2_t size, unsigned int layers)
{
    bool g_constraint_initialized = false;

    if (!map || size.x < 2 || size.y < 2 || layers == 0
        || !reg_init(&map->objs, sizeof(csp_object_t), CSP_MAP_OBJS_SIZE))
        return false;
    map->size = size;
    map->layers = layers;
    map->area = size.x * size.y;
    list_init(&map->placement_history);
    g_constraint_initialized = reg_init(
        &map->global_constraints,
        sizeof(csp_global_constraint_t),
        CSP_GLOBAL_CONSTRAINT_SIZE
    );
    if (!g_constraint_initialized || !init_layers(map)) {
        csp_map_deinit(map);
        return false;
    }
    map->floor = NULL;
    return true;
}

void csp_map_deinit(csp_map_t *map)
{
    if (!map)
        return;
    reg_deinit(&map->global_constraints);
    reg_map(&map->objs, (reg_callback_t)csp_obj_deinit);
    reg_deinit(&map->objs);
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

    cr_assert(csp_map_init(&map, size, layers));
    cr_assert_eq(map.size.x, 10);
    cr_assert_eq(map.size.y, 20);
    cr_assert_eq(map.area, 200);
    cr_assert_eq(map.placement_history.size, 0);
    cr_assert_eq(map.cells.size, (size.x * size.y) * layers);
    csp_map_deinit(&map);
}

#endif