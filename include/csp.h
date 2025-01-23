#pragma once

#include <stdbool.h>
#include "linked.h"
#include "orientation.h"
#include "prop.h"
#include "registry.h"
#include "str.h"
#include "v2.h"
#include "terrain.h"

#define CSP_ROOM_REGISTRY_BASE_SIZE 10
#define CSP_CONSTRAINT_SIZE 10
#define CSP_GLOBAL_CONSTRAINT_SIZE 3
#define CSP_POS_REG_BASE_SIZE 5
#define CSP_PROP_REG_BASE_SIZE 5
#define CSP_ROOM_OBJ_SIZE 10
#define CSP_OBJ_PROP_SIZE 1
#define CSP_ROOM_DEFAULT_LAYERS 3

typedef struct world_s world_t;
typedef struct csp_map_s csp_map_t;

//////////////////////////////////////////////////// CONSTRAINT

typedef struct csp_constraint_s csp_constraint_t;

typedef enum {
    C_ADJACENT_TO_WALL,
    C_ADJACENT_TO_PROP,
    C_IN_CORNER,
    C_AMOUNT_RANGE,
    C_RESERVED_SPACE,
    C_ON_TOP_OF_PROP,
    C_ON_GROUND,
    C_HAS_ORIENT,
} csp_constraint_type_t;

typedef bool(*csp_validator_t)(
    csp_map_t *map,
    prop_t *prop,
    csp_constraint_t *constraint,
    v2_t pos,
    unsigned int layer,
    orient_t orient
);

struct csp_constraint_s {
    csp_constraint_type_t type;
    csp_validator_t validate;
    bool expected;
    union {
        unsigned int range[2];
        reg_t positions;
        reg_t orientations;
        reg_t props;
    };
};

//////////////////////////////////////////////////// GLOBAL CONSTRAINT

typedef struct csp_global_constraint_s csp_global_constraint_t;

typedef enum {
    GC_ALL_CELLS_CONNECTED,
} csp_global_constraint_type_t;

typedef bool(*csp_global_validator_t)(csp_map_t *map, csp_global_constraint_t *gconstraint, v2_t pos, unsigned int layer);

struct csp_global_constraint_s {
    csp_global_constraint_type_t type;
    csp_global_validator_t validate;
    bool expected;
};

//////////////////////////////////////////////////// OBJECT

typedef struct csp_object_s csp_object_t;

struct csp_object_s {
    reg_t props; // prop_t
    float chance;
    reg_t constraints; // constraint_t
};

//////////////////////////////////////////////////// ROOM

typedef struct {
    name_t name;
    terrain_t *terrain;
    unsigned int layers;
    reg_t constraints; // csp_global_constraint_t
    reg_t objs;
} csp_room_t;

//////////////////////////////////////////////////// MAP

typedef struct {
    v2_t position;
    unsigned int layer;
} csp_pos_t;

typedef list_t csp_placement_t; // csp_pos_t

#define csp_placement_destroy list_destroy_free
#define csp_placement_get list_at

typedef struct {
    bool occupied;
    prop_t *occupant;
    orient_t occupant_orient;
} csp_cell_t;

struct csp_map_s {
    terrain_t *terrain;
    v2_t size;
    unsigned int area;
    unsigned int layers;
    vec_t cells; // csp_cell (size.x * size.y * layers)
    reg_t *objs;  // csp_object_t
    reg_t *global_constraints; // csp_global_constraint_t
    list_t placement_history; // csp_placement_t
};

/// Contraint

csp_constraint_t *csp_get_constraint(csp_object_t *obj, csp_constraint_type_t type,
    bool add_if_absent, bool expected_match);
csp_constraint_t *csp_add_constraint(csp_object_t *obj, csp_constraint_type_t type,
    bool expected_match);
csp_constraint_t *csp_get_props_constraint(csp_object_t *obj, csp_constraint_type_t type,
    bool add_if_absent, bool expected_match);
bool csp_constraint_has_prop(csp_constraint_t *constraint, prop_t *prop);
void csp_constraint_deinit(csp_constraint_t *constraint);

bool csp_set_adjacent_to_wall(csp_object_t *obj, bool expected);
bool csp_set_adjacent_to_prop(csp_object_t *obj, bool expected, prop_t *prop);
bool csp_set_in_corner(csp_object_t *obj, bool expected);
bool csp_set_amount_range(csp_object_t *obj, unsigned int min, unsigned int max);
bool csp_set_amount(csp_object_t *obj, unsigned int nb);
bool csp_set_reserved_space(csp_object_t *obj, bool expected, v2_t position);
bool csp_set_on_ground(csp_object_t *obj, bool expected);
bool csp_set_has_orient(csp_object_t *obj, bool expected, orient_t orient);
bool csp_set_on_top_of_prop(csp_object_t *obj, bool expected, prop_t *prop);

/// Global Constraint

csp_global_constraint_t *csp_get_global_constraint(csp_room_t *room, csp_global_constraint_type_t type, bool add_if_absent);
csp_global_constraint_t *csp_add_global_constraint(csp_room_t *room, csp_global_constraint_type_t type);
void csp_global_constraint_deinit(csp_global_constraint_t *gconstraint);

bool csp_set_all_cell_connected(csp_room_t *room, bool expected);

/// Object

bool csp_obj_init(csp_object_t *obj);
void csp_obj_deinit(csp_object_t *obj);
bool csp_obj_add_prop(csp_object_t *obj, prop_t *prop);
prop_t *csp_obj_pick_prop(csp_object_t *obj);
csp_placement_t *csp_obj_make_placement(csp_object_t *obj, prop_t *prop, v2_t pos, unsigned int layer, orient_t orient);

/// Room

bool csp_room_init(csp_room_t *room, const char *name);
void csp_room_deinit(csp_room_t *room);
csp_object_t *csp_room_add_obj(csp_room_t *room);
bool csp_room_set_layers(csp_room_t *room, unsigned int layers);

/// Map

bool csp_map_init(csp_map_t *map, csp_room_t *room, v2_t size);
void csp_map_deinit(csp_map_t *map);

bool csp_pos_is_valid(csp_map_t *map, v2_t pos, unsigned int layer);
csp_cell_t *csp_map_get_cell(csp_map_t *map, v2_t pos, unsigned int layer);
void csp_map_print(csp_map_t *map);

bool csp_map_cell_occupied(csp_map_t *map, v2_t pos, unsigned int layer);
void csp_map_clear_cell(csp_map_t *map, v2_t pos, unsigned int layer);
bool csp_map_occupy_cell(csp_map_t *map, v2_t pos, unsigned int layer);
void csp_map_clear_placement(csp_map_t *map, csp_placement_t *placement);
bool csp_map_occupy_placement(csp_map_t *map, csp_placement_t *placement);

bool csp_get_possible_pos(
    csp_map_t *map,
    csp_object_t *obj,
    prop_t *prop,
    orient_t orient,
    bool is_last,
    list_t *buf
);
bool csp_place_prop(csp_map_t *map, prop_t *prop, v2_t pos, unsigned int layer, orient_t orient);
bool csp_map_dfs_cells(csp_map_t *map, unsigned int layer);
bool csp_map_generate(csp_map_t *map);
void csp_map_remove_at(csp_map_t *map, v2_t pos, unsigned int layer);


bool csp_map_apply(csp_map_t *map, world_t *world, v2_t pos);