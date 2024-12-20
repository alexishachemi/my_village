#include "csp.h"
#include "linked.h"
#include "orientation.h"
#include "raylib.h"
#include "registry.h"

static unsigned int get_placement_count(csp_object_t *obj)
{
    csp_constraint_t *amount_constraint = NULL;
 
    if (!obj || obj->is_collection)
        return 0;
    amount_constraint = csp_get_constraint(obj, C_AMOUNT_RANGE, false);
    if (!amount_constraint)
        return 1;
    return GetRandomValue(amount_constraint->range[0], amount_constraint->range[1]);
}

static csp_object_t *get_final_obj(csp_object_t *obj)
{
    if (!obj)
        return NULL;
    if (obj->is_collection)
        obj = REG_AT(csp_object_t, &obj->objs, GetRandomValue(0, obj->objs.last_free_index - 1));
    return obj;
}

static bool place_obj(csp_map_t *map, unsigned int idx, unsigned int nb_placements);

static bool try_possible_positions(csp_map_t *map, csp_object_t *obj, orient_t orient,
    unsigned int idx, unsigned int nb_placements)
{
    list_t possible_positions = {0};
    csp_pos_t *pos = {0};
    csp_placement_t *placement = NULL;

    if (!csp_get_possible_pos(map, obj, orient, &possible_positions))
        return false;
    for (node_t *itt = possible_positions.head; itt; itt = itt->next) {
        pos = itt->data;
        if (!csp_place_obj(map, obj, pos->position, pos->layer, orient))
            continue;
        if (place_obj(map, idx + (nb_placements == 1), nb_placements - 1)) {
            list_clear_free(&possible_positions);
            return true;
        }
        placement = csp_obj_make_placement(obj, pos->position, pos->layer, orient);
        csp_map_clear_placement(map, placement);
        csp_placement_destroy(placement);
    }
    list_clear_free(&possible_positions);
    return false;
}

static bool place_obj(csp_map_t *map, unsigned int idx, unsigned int nb_placements)
{
    csp_object_t *obj = NULL;
    orient_t orient = ORIENT_DOWN;

    if (idx >= REG_SIZE(map->objs))
        return true;
    obj = get_final_obj(REG_AT(csp_object_t, &map->objs, idx));
    if (!obj)
        return false;
    nb_placements = nb_placements == 0 ? get_placement_count(obj) : nb_placements;
    for (unsigned int o = 0; o < 4; o++) {
        orient = directions[o];
        if (try_possible_positions(map, obj, orient, idx, nb_placements))
            return true;
    }
    return false;
}

bool csp_map_generate(csp_map_t *map)
{
    return map && place_obj(map, 0, 0);
}