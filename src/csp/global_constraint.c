#include "csp.h"
#include "registry.h"
#include "v2.h"
#include "utils.h"

csp_global_constraint_t *csp_get_global_constraint(csp_map_t *map, csp_global_constraint_type_t type, bool add_if_absent)
{
    csp_global_constraint_t *gconstraint = NULL;

    if (!map)
        return NULL;
    for (unsigned int i = 0; i < REG_SIZE(map->global_constraints); i++) {
        gconstraint = REG_AT(csp_global_constraint_t, &map->global_constraints, i);
        if (!gconstraint)
            return NULL;
        if (gconstraint->type == type)
            return gconstraint;
    }
    if (add_if_absent)
        return csp_add_global_constraint(map, type);
    return NULL;
}

csp_global_constraint_t *csp_add_global_constraint(csp_map_t *map, csp_global_constraint_type_t type)
{
    csp_global_constraint_t *gconstraint = NULL;

    if (!map)
        return NULL;
    gconstraint = reg_new_elem(&map->global_constraints);
    if (!gconstraint)
        return NULL;
    gconstraint->type = type;
    gconstraint->validate = NULL;
    return gconstraint;
}

void csp_global_constraint_deinit(UNUSED csp_global_constraint_t *gconstraint)
{
    return;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_global_constraint, add)
{
    csp_map_t map = {0};
    csp_global_constraint_t *gconstraint = NULL;
    csp_room_t room = {0};

    csp_room_init(&room, "same");
    cr_assert(csp_map_init(&map, &room, (v2_t){10, 10}));
    cr_assert_eq(REG_SIZE(map.global_constraints), 0);
    gconstraint = csp_add_global_constraint(&map, GC_ALL_CELLS_CONNECTED);
    cr_assert_not_null(gconstraint);
    cr_assert_eq(REG_SIZE(map.global_constraints), 1);
    cr_assert_eq(gconstraint->type, GC_ALL_CELLS_CONNECTED);
    csp_map_deinit(&map);
    csp_room_deinit(&room);
}

#endif