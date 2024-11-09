#include "wfc.h"

position_t add_positions(position_t pos1, position_t pos2)
{
    return (position_t){pos1.x + pos2.x, pos1.y + pos2.y};
}

void initialize_map(map_t* map)
{
    for (int i = 0; i < map->width * map->height; i++) {
        map->cells[i] = ALL_POSSIBLE_STATES;
    }
}

bool is_out_of_bounds(map_t* map, position_t pos)
{
    return pos.x < 0 || pos.x >= map->width || pos.y < 0 || pos.y >= map->height;
}
cell_state_t get_cell(map_t* map, position_t pos)
{
    if (is_out_of_bounds(map, pos)) {
        return 0;
    }
    return map->cells[pos.y * map->width + pos.x];
}

void set_cell(map_t* map, position_t pos, cell_state_t state)
{
    if (!is_out_of_bounds(map, pos)) {
        map->cells[pos.y * map->width + pos.x] = state;
    }
}