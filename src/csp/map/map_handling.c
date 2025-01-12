#include <stdio.h>
#include "csp.h"

bool csp_pos_is_valid(csp_map_t *map, v2_t pos, unsigned int layer)
{
    return map && pos.x >= 0 && pos.x < map->size.x
        && pos.y >= 0 && pos.y < map->size.y
        && layer < map->layers;
}

csp_cell_t *csp_map_get_cell(csp_map_t *map, v2_t pos, unsigned int layer)
{
    if (!map || !csp_pos_is_valid(map, pos, layer))
        return NULL;
    return vec_at(&map->cells, (map->area * layer) + pos.y * map->size.x + pos.x);
}

static void print_layer(csp_map_t *map, unsigned int layer)
{
    csp_cell_t *cell = NULL;

    for (int y = 0; y < map->size.y; y++) {
        for (int x = 0; x < map->size.x; x++) {
            cell = csp_map_get_cell(map, (v2_t){x, y}, layer);
            if (!cell)
                return;
            printf("%c", cell->occupied ? 'X' : '.');
        }
        printf("\n");
    }
}

void csp_map_print(csp_map_t *map)
{
    if (!map)
        printf("(null)\n");
    for (unsigned int layer = 0; layer < map->layers; layer++) {
        printf("Layer %d\n", layer);
        print_layer(map, layer);
        printf("\n");
    }
    printf("\n");
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

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