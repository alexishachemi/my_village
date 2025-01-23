#include "csp.h"
#include "linked.h"
#include "orientation.h"
#include "prop.h"
#include "utils.h"
#include "parser.h"

bool parse_csp_set_adjacent_to_wall(
    parser_t *parser,
    UNUSED const char *name,
    csp_object_t *obj,
    bool expected,
    UNUSED cJSON *args
)
{
    if (!csp_set_adjacent_to_wall(obj, expected))
        return parser_raise_error(parser, "Failed to set csp constraint");
    return true;
}

static bool validate(
    csp_map_t *map,
    UNUSED prop_t *prop,
    UNUSED csp_constraint_t *constraint,
    v2_t pos,
    UNUSED unsigned int layer,
    UNUSED orient_t orient
)
{
    v2_t *curr = NULL;
    list_t *cov = prop_get_coverage(prop, pos, orient);

    if (!cov)
        return false;
    for (node_t *n = cov->head; n; n = n->next) {
        curr = n->data;
        if ( (curr->x == 0 || curr->x == map->size.x - 1) ||
             (curr->y == 0 || curr->y == map->size.y - 1) ) {
            list_destroy_free(cov);
            return true;
        }
    }
    list_destroy_free(cov);
    return false;
}

bool csp_set_adjacent_to_wall(csp_object_t *obj, bool expected)
{
    csp_constraint_t *constraint = NULL;

    if (!obj)
        return false;
    constraint = csp_get_constraint(obj, C_ADJACENT_TO_WALL, true);
    if (!constraint)
        return false;
    constraint->validate = validate;
    constraint->expected = expected;
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, adjacent_to_wall)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_adjacent_to_wall(&obj, true));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_ADJACENT_TO_WALL);
    csp_obj_deinit(&obj);
}

Test(csp_constraint, adjacent_to_wall_validation)
{
    csp_map_t map = {0};
    csp_constraint_t *constraint = NULL;
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_room_t room = {0};

    cr_assert(csp_room_init(&room, "foo"));
    cr_assert(csp_obj_init(&obj));
    cr_assert(csp_map_init(&map, &room, (v2_t){10, 10}));
    cr_assert(csp_set_adjacent_to_wall(&obj, true));
    constraint = csp_get_constraint(&obj, C_ADJACENT_TO_WALL, false);
    cr_assert_not_null(constraint);
    cr_assert_not_null(constraint->validate);

    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){0, 0}, 0, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){9, 5}, 1, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){9, 9}, 2, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){8, 0}, 1, ORIENT_DOWN));

    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){5, 5}, 2, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){8, 1}, 1, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){3, 7}, 0, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){1, 1}, 0, ORIENT_DOWN));

    csp_obj_deinit(&obj);
    csp_map_deinit(&map);
    csp_room_deinit(&room);
}

#endif