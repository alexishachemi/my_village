#include "csp.h"

bool csp_map_cell_occupied(csp_map_t *map, v2_t pos, unsigned int layer)
{
    csp_cell_t *cell = csp_map_get_cell(map, pos, layer);

    return !cell || cell->occupied;
}

void csp_map_clear_cell(csp_map_t *map, v2_t pos, unsigned int layer)
{
    csp_cell_t *cell = csp_map_get_cell(map, pos, layer);
    
    if (!cell)
        return;
    cell->occupied = false;
    cell->occupant = NULL;
}

bool csp_map_occupy_cell(csp_map_t *map, v2_t pos, unsigned int layer)
{
    csp_cell_t *cell = csp_map_get_cell(map, pos, layer);

    if (!cell || cell->occupied)
        return false;
    cell->occupied = true;
    return true;
}

void csp_map_clear_placement(csp_map_t *map, csp_placement_t *placement)
{
    csp_pos_t *pos = NULL;

    if (!map || !placement)
        return;
    for (node_t *itt = placement->head; itt; itt = itt->next) {
        pos = itt->data;
        csp_map_clear_cell(map, pos->position, pos->layer);
    }
}

bool csp_map_occupy_placement(csp_map_t *map, csp_placement_t *placement)
{
    csp_pos_t *pos = NULL;

    if (!map || !placement)
        return false;
    for (node_t *itt = placement->head; itt; itt = itt->next) {
        pos = itt->data;
        if (csp_map_cell_occupied(map, pos->position, pos->layer))
            return false;
    }
    for (node_t *itt = placement->head; itt; itt = itt->next) {
        pos = itt->data;
        csp_map_occupy_cell(map, pos->position, pos->layer);
    }
    return true;
}
