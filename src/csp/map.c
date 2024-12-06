#include "csp.h"
#include "cvec.h"
#include "linked.h"
#include "registry.h"
#include "v2.h"

static bool init_layers(csp_map_t *map)
{
    map->cells = vec_create(sizeof(csp_cell_t));
    return vec_resize(&map->cells, (map->size.x * map->size.y) * map->layers);
}

bool csp_map_init(csp_map_t *map, v2_t size, unsigned int layers)
{
    bool g_constraint_initialized = false;

    if (!map || size.x < 2 || size.y < 2 || layers == 0)
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
    return true;
}

void csp_map_deinit(csp_map_t *map)
{
    if (!map)
        return;
    reg_deinit(&map->global_constraints);
    list_clear(&map->placement_history, (callback_t)list_destroy_free);
    vec_free_data(&map->cells);
    map->size = (v2_t){0, 0};
    map->layers = 0;
}

static bool pos_is_valid(csp_map_t *map, v2_t pos)
{
    return pos.x >= 0 && pos.x < map->size.x
        && pos.y >= 0 && pos.y < map->size.y;
}

csp_cell_t *csp_map_get_cell(csp_map_t *map, v2_t pos, unsigned int layer)
{
    if (!map || !pos_is_valid(map, pos) || layer >= map->layers)
        return NULL;
    return vec_at(&map->cells, (map->area * layer) + pos.y * map->size.x + pos.x);
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

Test(csp_map, get)
{
    csp_map_t map = {0};
    v2_t size = {10, 10};
    int layers = 3;
    csp_cell_t *cell = NULL;
    csp_cell_t *cell_ref = NULL;
    v2_t pos = {0, 0};

    cr_assert(csp_map_init(&map, size, layers));
    for (unsigned int layer = 0; layer < map.layers; layer++) {
        cell_ref = vec_at(&map.cells, (map.area * layer) + (size.y * pos.y + pos.x));
        cell = csp_map_get_cell(&map, pos, layer);
        cr_assert_eq(
            cell, cell_ref,
            "Cell at (%d,%d) and layer %d is not the same (expected %p, got %p)\n", 
            pos.x, pos.y, layer, cell_ref, cell 
        );
    }
    csp_map_deinit(&map);
}

#endif