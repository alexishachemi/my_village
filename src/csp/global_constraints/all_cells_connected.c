#include "csp.h"
#include "registry.h"

bool csp_set_all_cell_connected(csp_map_t *map)
{
    if (!map)
        return false;
    return csp_get_global_constraint(map, GC_ALL_CELLS_CONNECTED, true);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_global_constraint, all_cells_connected)
{
    csp_map_t map = {0};
    csp_global_constraint_t *gconstraint = NULL;

    cr_assert(csp_map_init(&map, (v2_t){10, 10}, 1));
    cr_assert_eq(REG_SIZE(map.global_constraints), 0);
    cr_assert(csp_set_all_cell_connected(&map));
    cr_assert_eq(REG_SIZE(map.global_constraints), 1);
    gconstraint = REG_AT(csp_global_constraint_t, &map.global_constraints, 0);
    cr_assert_not_null(gconstraint);
    cr_assert_eq(gconstraint->type, GC_ALL_CELLS_CONNECTED);
    csp_map_deinit(&map);
}

#endif