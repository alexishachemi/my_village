#pragma once

#include <stdbool.h>
#include "prop.h"
#include "registry.h"
#include "v2.h"

#define CSP_CONSTRAINT_SIZE 10
#define CSP_GLOBAL_CONSTRAINT_SIZE 3

//////////////////////////////////////////////////// CONSTRAINT

typedef enum {
    C_ADJACENT_TO_WALL,
    C_ADJACENT_TO_PROP,
    C_IN_CORNER,
    C_AMOUNT_RANGE,
    C_SURROUNDING_SPACE,
    C_ON_TOP_OF_PROP,
} csp_constraint_type_t;

typedef struct {
    csp_constraint_type_t type;
    union {
        prop_t *prop;
        unsigned int range[2];
        reg_t positions;
    };
} csp_constraint_t;

//////////////////////////////////////////////////// GLOBAL CONSTRAINT

typedef enum {
    GC_ALL_CELLS_CONNECTED,
} csp_global_constraint_type_t;

typedef struct {
    csp_global_constraint_type_t type;
} csp_global_constraint_t;

//////////////////////////////////////////////////// OBJECT

typedef struct csp_object_s csp_object_t, csp_collection_t;

struct csp_object_s {
    bool is_collection;
    union {
        struct {
            prop_t *prop;
            reg_t constraints; // constraint_t
        };
        reg_t objs;
    };
};

typedef reg_t csp_placement_list_t; // csp_object_t

//////////////////////////////////////////////////// MAP

typedef struct {
    bool free;
    prop_t *occupant;
} csp_cell_t;

typedef struct {
    v2_t size;
    reg_t global_constraints; // csp_global_constraint_t
    vec_t layer; // vec_t // csp_cell;
    csp_placement_list_t placement_list;
} csp_map_t;

/// Contraint

csp_constraint_t *csp_get_constraint(csp_object_t *obj, csp_constraint_type_t type);
csp_constraint_t *csp_add_constraint(csp_object_t *obj, csp_constraint_type_t type);

bool csp_add_adjacent_to_wall(csp_object_t *obj);
bool csp_add_adjacent_to_prop(csp_object_t *obj, prop_t *prop);
bool csp_add_in_corner(csp_object_t *obj);
bool csp_add_amount_range(csp_object_t *obj, unsigned int min, unsigned int max);
bool csp_add_reserved_space(csp_object_t *obj, v2_t position);

/// Global Constraint

csp_global_constraint_t *csp_get_global_constraint(csp_map_t *map, csp_global_constraint_type_t type);

bool csp_add_all_cell_connected(csp_map_t *map);

/// Object

bool csp_obj_init(csp_object_t *obj, prop_t *prop);

bool csp_collection_init(csp_collection_t *coll);
csp_object_t *csp_collection_add_obj(csp_object_t *collection);
bool csp_obj_is_collection(csp_object_t *obj);

/// Map

bool csp_map_init(csp_map_t *map, v2_t size, unsigned int layers);
csp_cell_t *csp_map_get_cell(csp_map_t *map, v2_t pos, unsigned int layer);