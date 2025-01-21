#include "csp.h"
#include "registry.h"
#include "utils.h"
#include "v2.h"
#include "parser.h"

bool parse_csp_all_cell_connected(
    parser_t *parser,
    UNUSED const char *name,
    csp_room_t *room,
    bool expected,
    UNUSED cJSON *args
)
{
    if (!csp_set_all_cell_connected(room, expected))
        return parser_raise_error(parser, "Failed to set global constraint");
    return true;
}

static bool validate(csp_map_t *map, UNUSED csp_global_constraint_t *gconstraint, v2_t pos, unsigned int layer)
{
    const v2_t surrounding[] = {
        {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}
    };
    int steps_since_last_occupied = -1;
    csp_cell_t *cell = NULL;

    for (unsigned int i = 0; i < 8; i++) {
        cell = csp_map_get_cell(map, V2_ADD(pos, surrounding[i]), layer);
        if (!cell || cell->occupied) {
            if (steps_since_last_occupied > 1 && steps_since_last_occupied < 7)
                return csp_map_dfs_cells(map, layer);
            steps_since_last_occupied = 0;
        }
        if (steps_since_last_occupied != -1)
            steps_since_last_occupied++;
    }
    return true;
}

bool csp_set_all_cell_connected(csp_room_t *room, bool expected)
{
    csp_global_constraint_t *gconstraint = NULL;

    if (!room)
        return false;
    gconstraint = csp_get_global_constraint(room, GC_ALL_CELLS_CONNECTED, true);
    if (!gconstraint)
        return false;
    gconstraint->validate = validate;
    gconstraint->expected = expected;
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_global_constraint, all_cells_connected)
{
    csp_map_t map = {0};
    csp_global_constraint_t *gconstraint = NULL;
    csp_room_t room = {0};

    cr_assert(csp_room_init(&room, "foo"));
    cr_assert(csp_map_init(&map, &room, (v2_t){10, 10}));
    cr_assert_eq(REG_SIZE(*map.global_constraints), 0);
    cr_assert(csp_set_all_cell_connected(&room, true));
    cr_assert_eq(REG_SIZE(*map.global_constraints), 1);
    gconstraint = REG_AT(csp_global_constraint_t, map.global_constraints, 0);
    cr_assert_not_null(gconstraint);
    cr_assert_eq(gconstraint->type, GC_ALL_CELLS_CONNECTED);
    csp_map_deinit(&map);
    csp_room_deinit(&room);
}

static bool cell_is_occupied(csp_map_t *map, v2_t pos, unsigned int layer)
{
    csp_cell_t *cell = csp_map_get_cell(map, pos, layer);

    return cell && cell->occupied;
}

static void try_position(csp_map_t *map, csp_global_constraint_t *gconstraint, v2_t pos, unsigned int layer, bool should_succeed)
{
    bool was_occupied = cell_is_occupied(map, pos, layer);

    csp_map_occupy_cell(map, pos, layer);
    cr_assert_eq(gconstraint->validate(map, gconstraint, pos, layer), should_succeed);
    if (!was_occupied)
        csp_map_clear_cell(map, pos, layer);
}

Test(csp_global_constraint, all_cells_connected_validation)
{
    csp_map_t map = {0};
    csp_global_constraint_t *gconstraint = NULL;
    csp_room_t room = {0};

    cr_assert(csp_room_init(&room, "foo"));
    cr_assert(csp_map_init(&map, &room, (v2_t){5, 5}));
    cr_assert(csp_set_all_cell_connected(&room, true));
    cr_assert_not_null((gconstraint = csp_get_global_constraint(&room, GC_ALL_CELLS_CONNECTED, false)));

    csp_map_occupy_cell(&map, (v2_t){2, 0}, 0);
    csp_map_occupy_cell(&map, (v2_t){2, 1}, 0);
    csp_map_occupy_cell(&map, (v2_t){2, 3}, 0);
    csp_map_occupy_cell(&map, (v2_t){2, 4}, 0);
    csp_map_occupy_cell(&map, (v2_t){3, 1}, 0);

    try_position(&map, gconstraint, (v2_t){0, 0}, 0, true);
    try_position(&map, gconstraint, (v2_t){1, 3}, 1, true);
    try_position(&map, gconstraint, (v2_t){4, 4}, 2, true);
    try_position(&map, gconstraint, (v2_t){2, 4}, 0, true);

    try_position(&map, gconstraint, (v2_t){2, 2}, 0, false);
    try_position(&map, gconstraint, (v2_t){4, 1}, 0, false);

    csp_map_deinit(&map);
    csp_room_deinit(&room);
}

#endif