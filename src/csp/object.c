#include "csp.h"
#include "linked.h"
#include "orientation.h"
#include "registry.h"
#include "v2.h"

bool csp_obj_init(csp_object_t *obj, prop_t *prop)
{
    if (!obj || !prop)
        return false;
    obj->is_collection = false;
    obj->prop = prop;
    return reg_init(&obj->constraints, sizeof(csp_constraint_t), CSP_CONSTRAINT_SIZE);
}

void csp_obj_deinit(csp_object_t *obj)
{
    if (!obj)
        return;
    if (obj->is_collection) {
        reg_map(&obj->objs, (reg_callback_t)csp_obj_deinit);
        reg_deinit(&obj->objs);
    } else {
        reg_map(&obj->constraints, (reg_callback_t)csp_constraint_deinit);
        reg_deinit(&obj->constraints);
    }
}

static bool add_positions(csp_placement_t *placement, csp_constraint_t *constraint, v2_t origin, orient_t orient)
{
    v2_t pos = {0};

    for (unsigned int i = 0; i < REG_SIZE(constraint->positions); i++) {
        pos = *REG_AT(v2_t, &constraint->positions, i);
        if (!list_add_copy(placement, &V2_ADD(origin, v2_orient(pos, orient)), sizeof(v2_t)))
            return false;
    }
    return true;
}

csp_placement_t *csp_obj_make_placement(csp_object_t *obj, v2_t pos, unsigned int layer, orient_t orient)
{
    csp_constraint_t *constraint = NULL;
    csp_placement_t *placement = NULL;

    if (!obj)
        return NULL;
    placement = list_create();
    if (!placement)
        return NULL;
    if (!list_add_copy(placement, &(csp_pos_t){v2_orient(pos, orient), layer}, sizeof(csp_pos_t))) {
        list_destroy(placement, NULL);
        return NULL;
    }
    constraint = csp_get_constraint(obj, C_RESERVED_SPACE, false);
    if (constraint && !add_positions(placement, constraint, pos, orient)) {
        list_destroy_free(placement);
        return NULL;
    }
    return placement;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_obj, init)
{
    csp_object_t obj = {0};
    prop_t prop = {0};

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(obj.constraints.vec.size, CSP_CONSTRAINT_SIZE);
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_not(obj.is_collection);
    cr_assert_eq(obj.prop, &prop);
    csp_obj_deinit(&obj);
}

Test(csp_obj, make_placement_single_pos)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_placement_t *placement = NULL;
    v2_t *pos_buf = NULL;
    v2_t pos = {12, 8};

    cr_assert(csp_obj_init(&obj, &prop));
    placement = csp_obj_make_placement(&obj, pos, 0, ORIENT_DOWN);
    cr_assert_not_null(placement);
    cr_assert_eq(placement->size, 1);
    pos_buf = placement->head->data;
    cr_assert_not_null(pos_buf);
    cr_assert(V2_EQ(pos, (*pos_buf)));
    csp_placement_destroy(placement);
    csp_obj_deinit(&obj);
}

static bool pos_is_reserved(v2_t pos, v2_t *reserved, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++) {
        if (V2_EQ(reserved[i], pos))
            return true;
    }
    return false;
}

Test(csp_obj, make_placement_mutiple_pos)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_placement_t *placement = NULL;
    v2_t *pos_buf = NULL;
    v2_t pos = {12, 8};
    v2_t reserved[] = { {3, 5}, {10, 5}, {20, 8} };
    unsigned int size = sizeof(reserved) / sizeof(v2_t);

    cr_assert(csp_obj_init(&obj, &prop));
    for (unsigned int i = 0; i < size; i++) {
        cr_assert(csp_set_reserved_space(&obj, reserved[i]));
    }
    placement = csp_obj_make_placement(&obj, pos, 0, ORIENT_DOWN);
    cr_assert_not_null(placement);
    cr_assert_eq(placement->size, size + 1);
    pos_buf = placement->head->data;
    cr_assert_not_null(pos_buf);
    cr_assert(V2_EQ(pos, (*pos_buf)));
    for (unsigned int i = 0; i < size; i++) {
        pos_buf = csp_placement_get(placement, i);
        cr_assert_not_null(pos_buf);
        cr_assert(pos_is_reserved(*pos_buf, reserved, size) || V2_EQ((*pos_buf), pos));
    }
    csp_placement_destroy(placement);
    csp_obj_deinit(&obj);
}

#endif