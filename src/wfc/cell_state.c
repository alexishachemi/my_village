#include "wfc.h"

bool is_collapsed(cell_state_t state)
{
    // does state has only one bit set? (power of 2)
    return (state & (state - 1)) == 0 && state != 0;
}

bool is_state_subset(cell_state_t subset, cell_state_t superset)
{
    return (subset & ~superset) == 0;
}

cell_state_t intersect_states(cell_state_t state1, cell_state_t state2)
{
    return state1 & state2;
}

cell_state_t get_possible_neighbor_states(
    cell_config_t* cfgs,
    state_count_t nb_states,
    cell_state_t cell_state,
    direction_t direction)
{
    cell_state_t possible_neighbor_states = 0;

    for (state_count_t id = 0; id < nb_states; id++) {
        if (cell_state & (1ULL << id)) {
            possible_neighbor_states |= cfgs[id].possible_neighbors[direction];
        }
    }
    return possible_neighbor_states;
}