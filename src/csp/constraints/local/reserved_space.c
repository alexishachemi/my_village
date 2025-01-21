#include "csp.h"
#include "orientation.h"
#include "registry.h"
#include "v2.h"
#include "parser.h"

bool parse_csp_set_reserved_space(
    parser_t *parser,
    const char *name,
    csp_object_t *obj,
    bool expected,
    cJSON *args
)
{
    v2_t pos = {0};

    if (!args)
        return parser_raise_error(parser, "Failed to get csp argument");
    while (args) {
        if (!cJSON_IsString(args))
            return parser_raise_invalid_type(parser, name, args, "String");
        if (!parse_v2(parser, name, args, &pos))
            return false;
        if (!csp_set_reserved_space(obj, expected, pos))
            return parser_raise_error(parser, "Failed to set csp constraint");
        args = args->next;
    }
    return true;
}

static bool validate(
    csp_map_t *map,
    prop_t *prop,
    csp_constraint_t *constraint,
    v2_t pos,
    unsigned int layer,
    orient_t orient
)
{
    v2_t offset = {0};
    prop_t *child = NULL;

    if (!prop || csp_map_cell_occupied(map, pos, layer)) {
        return false;
    }
    if (prop->type == PTYPE_PARENT && prop->has_child) {
        for (unsigned int i = 0; i < REG_SIZE(prop->children); i++) {
            child = REG_AT(prop_t, &prop->children, i);
            offset = child->offset;
            if (csp_map_cell_occupied(map, V2_ADD(pos, v2_orient(offset, orient)), layer))
                return false;
        }
    }
    for (unsigned int i = 0; i < REG_SIZE(constraint->positions); i++) {
        offset = *REG_AT(v2_t, &constraint->positions, i);
        if (csp_map_cell_occupied(map, V2_ADD(pos, v2_orient(offset, orient)), layer))
            return false;
    }
    return true;
}

bool csp_set_reserved_space(csp_object_t *obj, bool expected, v2_t position)
{
    csp_constraint_t *constraint = NULL;

    if (!obj)
        return false;
    constraint = csp_get_constraint(obj, C_RESERVED_SPACE, false);
    if (!constraint) {
        constraint = csp_add_constraint(obj, C_RESERVED_SPACE);
        if (!constraint || !reg_init(&constraint->positions, sizeof(v2_t), CSP_POS_REG_BASE_SIZE))
            return false;
        constraint->validate = validate;
        constraint->expected = expected;
    }
    return reg_push_back(&constraint->positions, &position);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, reserved_space)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;
    v2_t *pos = NULL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_reserved_space(&obj, true, (v2_t){1, 0}));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_RESERVED_SPACE);
    cr_assert_eq(REG_SIZE(constraint->positions), 1);
    
    pos = REG_AT(v2_t, &constraint->positions, 0);
    cr_assert_not_null(pos);
    cr_assert_eq(pos->x, 1);
    cr_assert_eq(pos->y, 0);
    
    cr_assert(csp_set_reserved_space(&obj, true, (v2_t){3, 1}));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(REG_AT(csp_constraint_t, &obj.constraints, 0), constraint);
    cr_assert_eq(REG_AT(v2_t, &constraint->positions, 0), pos);
    cr_assert_eq(pos->x, 1);
    cr_assert_eq(pos->y, 0);

    pos = REG_AT(v2_t, &constraint->positions, 1);
    cr_assert_not_null(pos);
    cr_assert_eq(pos->x, 3);
    cr_assert_eq(pos->y, 1);

    csp_obj_deinit(&obj);
}

Test(csp_constraint, reserved_space_validation)
{
    csp_map_t map = {0};
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;
    csp_room_t room = {0};

    cr_assert(csp_room_init(&room, "foo"));
    prop.type = PTYPE_CHILD;
    cr_assert(csp_map_init(&map, &room, (v2_t){10, 10}));
    cr_assert(csp_obj_init(&obj));

    cr_assert(csp_set_reserved_space(&obj, true, (v2_t){0, -1})); //  #
    cr_assert(csp_set_reserved_space(&obj, true, (v2_t){-1, 0})); // #O 
    cr_assert(csp_set_reserved_space(&obj, true, (v2_t){0, 1}));  //  #

    csp_map_occupy_cell(&map, (v2_t){3, 2}, 0);
    csp_map_occupy_cell(&map, (v2_t){5, 5}, 1);
    csp_map_occupy_cell(&map, (v2_t){1, 1}, 2);

    constraint = csp_get_constraint(&obj, C_RESERVED_SPACE, false);
    cr_assert_not_null(constraint);
    cr_assert_not_null(constraint->validate);

    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){0, 0}, 0, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){9, 0}, 2, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){1, 1}, 2, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){3, 3}, 0, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){6, 5}, 1, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){9, 9}, 0, ORIENT_DOWN));

    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){1, 1}, 0, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){3, 3}, 1, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){6, 5}, 2, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){4, 5}, 1, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){9, 8}, 0, ORIENT_DOWN));

    csp_obj_deinit(&obj);
    csp_map_deinit(&map);
    csp_room_deinit(&room);
}

#endif