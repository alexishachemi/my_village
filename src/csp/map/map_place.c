#include "csp.h"
#include "linked.h"
#include "orientation.h"
#include "prop.h"
#include "registry.h"
#include "utils.h"
#include "v2.h"

static bool can_place_at(
    csp_map_t *map,
    csp_object_t *obj,
    prop_t *prop,
    v2_t pos,
    unsigned int layer,
    orient_t orient,
    bool is_last
)
{
    csp_placement_t *placement = csp_obj_make_placement(obj, prop, pos, layer, orient);
    csp_pos_t *pos_buf = NULL;
    bool can_place = true;
    csp_constraint_t *constraint = NULL;
    csp_global_constraint_t *gconstraint = NULL;

    if (!placement)
        return false;
    for (node_t *itt = placement->head; can_place && itt; itt = itt->next) {
        pos_buf = itt->data;
        can_place &= !csp_map_cell_occupied(map, pos_buf->position, pos_buf->layer);
    }
    for (unsigned int i = 0; can_place && i < REG_SIZE(obj->constraints); i++) {
        constraint = REG_AT(csp_constraint_t, &obj->constraints, i);
        if (!constraint->validate)
            continue;
        can_place &= constraint->validate(map, prop, constraint, pos, layer, orient) == constraint->expected;
    }
    if (!can_place || !csp_map_occupy_placement(map, placement)) {
        csp_placement_destroy(placement);
        return false;
    }
    for (unsigned int i = 0; is_last && can_place && i < REG_SIZE(*map->global_constraints); i++) {
        gconstraint = REG_AT(csp_global_constraint_t, map->global_constraints, i);
        if (!gconstraint->validate)
            continue;
        can_place &= gconstraint->validate(map, gconstraint, pos, layer) == gconstraint->expected;
    }
    csp_map_clear_placement(map, placement);
    return can_place;
}

bool csp_get_possible_pos(
    csp_map_t *map,
    csp_object_t *obj,
    prop_t *prop,
    orient_t orient,
    bool is_last,
    list_t *buf
)
{
    csp_pos_t pos = {0};

    if (!map || !obj || !buf)
        return false;
    list_init(buf);
    for (unsigned int layer = 0; layer < map->layers; layer++) {
        pos.layer = layer;
        for (unsigned int i = 0; i < map->area; i++) {
            pos.position = (v2_t){i % map->size.x, i / map->size.x};
            if (!can_place_at(map, obj, prop, pos.position, pos.layer, orient, is_last))
                continue;
            if (!list_add_copy(buf, &pos, sizeof(pos))) {
                list_clear_free(buf);
                return false;
            }
        }
    }
    return list_shuffle(buf);
}

static bool place_at(csp_map_t *map, v2_t pos, unsigned int layer, prop_t *occupant, orient_t occupant_orient)
{
    csp_cell_t *cell = NULL;

    cell = csp_map_get_cell(map, pos, layer);
    if (!cell || cell->occupied)
        return false;
    cell->occupied = true;
    cell->occupant = occupant;
    cell->occupant_orient = occupant_orient;
    return true;
}

bool csp_place_prop(csp_map_t *map, prop_t *prop, v2_t pos, unsigned int layer, orient_t orient)
{
    prop_t *child = NULL;

    if (!place_at(map, pos, layer, prop, orient))
        return false;
    for (unsigned int i = 0; prop->has_child && i < REG_SIZE(prop->children); i++) {
        child = REG_AT(prop_t, &prop->children, i);
        if (!place_at(map, V2_ADD(pos, v2_orient(child->offset, orient)), layer, child, orient))
            return false;
    }
    return true;
}

static void remove_children(csp_map_t *map, csp_cell_t *cell, v2_t pos, unsigned int layer)
{
    orient_t orient = cell->occupant_orient;
    prop_t *prop = cell->occupant;
    prop_t *child = NULL;
    v2_t offset = {0};

    cell->occupant = NULL;
    cell->occupied = false;
    if (!prop_has_child(prop))
        return;
    for (unsigned int i = 0; i < REG_SIZE(prop->children); i++) {
        child = REG_AT(prop_t, &prop->children, i);
        offset = v2_orient(V2_ADD(pos, child->offset), orient);
        cell = csp_map_get_cell(map, offset, layer);
        if (!cell)
            continue;
        cell->occupant = NULL;
        cell->occupied = false;
    }
}

void csp_map_remove_at(csp_map_t *map, v2_t pos, unsigned int layer)
{
    csp_cell_t *cell = NULL;
    v2_t new_pos = {0}; 

    cell = csp_map_get_cell(map, pos, layer);
    if (!cell || !cell->occupant)
        return;
    if (cell->occupant->type == PTYPE_CHILD) {
        new_pos = V2_SUB(v2_orient(pos, cell->occupant_orient), pos);
        csp_map_remove_at(map, new_pos, layer);
        return;
    }
    remove_children(map, cell, pos, layer);
}