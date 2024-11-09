#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_NEIGHBORS 4

#define DIR_UP    0
#define DIR_RIGHT 1
#define DIR_DOWN  2
#define DIR_LEFT  3

typedef struct position_s {
    int x;
    int y;
} position_t;


position_t add_positions(position_t pos1, position_t pos2);

#define MAX_STATES 64
#define ALL_POSSIBLE_STATES 0xFFFFFFFFFFFFFFFFULL

typedef uint64_t cell_state_t;
typedef uint8_t state_count_t;
typedef uint8_t direction_t;

typedef struct {
    cell_state_t possible_neighbors[MAX_NEIGHBORS];
} cell_config_t;

typedef struct {
    int width;
    int height;
    cell_state_t* cells;
} map_t;


void initialize_map(map_t* map);
bool is_out_of_bounds(map_t* map, position_t pos);
cell_state_t get_cell(map_t* map, position_t pos);
void set_cell(map_t* map, position_t pos, cell_state_t state);

bool is_collapsed(cell_state_t state);
bool is_state_subset(cell_state_t subset, cell_state_t superset);
cell_state_t intersect_states(cell_state_t state1, cell_state_t state2);
cell_state_t get_possible_neighbor_states(
    cell_config_t* cfgs, state_count_t nb_states,
    cell_state_t cell_state, direction_t direction);

typedef struct {
    map_t* map;
    cell_config_t* configs;
    state_count_t num_configs;
} collapser_t;

bool propagate(collapser_t* collapser, position_t pos, cell_state_t state);
bool propagate_step(collapser_t* collapser, position_t pos, int direction, cell_state_t state);
