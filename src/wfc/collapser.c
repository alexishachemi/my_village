#include "wfc.h"


position_t directions[4] = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}
};

bool propagate(collapser_t* collapser, position_t pos, uint64_t new_state)
{
    cell_state_t old_state = 0;
    cell_state_t intersected = 0;
    
    old_state = get_cell(collapser->map, pos);
    if ((old_state == 0) || ((old_state & new_state) == 0)) {
        // Invalid cell state or no overlap between current state and new state
        return false;
    }
    intersected = intersect_states(old_state, new_state);
    if (intersected == old_state) {
        // State not changed, no need to propagate further
        return true;
    }
    set_cell(collapser->map, pos, intersected);
    for (int i = 0; i < MAX_NEIGHBORS; i++) {
        if (!propagate_step(
            collapser,
            add_positions(pos, directions[i]), i, intersected)
            ) {
            return false;
        }
    }
    return true;
}

bool propagate_step(collapser_t* collapser, position_t pos, int direction, cell_state_t state)
{
    if (is_out_of_bounds(collapser->map, pos)) return true;

    uint64_t possible_neighbor_states = get_possible_neighbor_states(
        collapser->configs, collapser->num_configs, state, direction);

    return propagate(collapser, pos, possible_neighbor_states);
}
