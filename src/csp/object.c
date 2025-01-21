#include "csp.h"
#include "linked.h"
#include "orientation.h"
#include "prop.h"
#include "raylib.h"
#include "registry.h"
#include "v2.h"

bool csp_obj_init(csp_object_t *obj)
{
    if (!obj)
        return false;
    if (!reg_init(&obj->props, sizeof(prop_t*), CSP_OBJ_PROP_SIZE)) {
        return false;
    }
    if (!reg_init(&obj->constraints, sizeof(csp_constraint_t), CSP_CONSTRAINT_SIZE)) {
        reg_deinit(&obj->props);
        return false;
    }
    obj->chance = 1.0;
    return true;
}

void csp_obj_deinit(csp_object_t *obj)
{
    if (!obj)
        return;
    reg_deinit(&obj->props);
    reg_map(&obj->constraints, (reg_callback_t)csp_constraint_deinit);
    reg_deinit(&obj->constraints);
}

bool csp_obj_add_prop(csp_object_t *obj, prop_t *prop)
{
    return obj && prop && reg_push_back(&obj->props, &prop);
}

prop_t *csp_obj_pick_prop(csp_object_t *obj)
{
    if (!obj || REG_EMPTY(obj->props))
        return NULL;
    return *REG_AT(
        prop_t *,
        &obj->props,
        GetRandomValue(0, REG_SIZE(obj->props) - 1)
    );
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

static bool add_children_positions(csp_placement_t *placement, prop_t *prop, v2_t pos, unsigned int layer, orient_t orient)
{
    prop_t *child = NULL;
    v2_t new_pos = {0};

    if (prop->type != PTYPE_PARENT || !prop->has_child)
        return true;
    for (unsigned int i = 0; i < REG_SIZE(prop->children); i++) {
        child = REG_AT(prop_t, &prop->children, i);
        new_pos = V2_ADD(pos, child->offset);
        if (!list_add_copy(placement, &(csp_pos_t){v2_orient(new_pos, orient), layer}, sizeof(csp_pos_t)))
            return false;
    }
    return true;
}

csp_placement_t *csp_obj_make_placement(csp_object_t *obj, prop_t *prop, v2_t pos, unsigned int layer, orient_t orient)
{
    csp_constraint_t *constraint = NULL;
    csp_placement_t *placement = NULL;

    if (!obj)
        return NULL;
    placement = list_create();
    if (!placement)
        return NULL;
    if (!list_add_copy(placement, &(csp_pos_t){v2_orient(pos, orient), layer}, sizeof(csp_pos_t))
        || !add_children_positions(placement, prop, pos, layer, orient)) {
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

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(obj.props.vec.size, CSP_OBJ_PROP_SIZE);
    cr_assert_eq(obj.constraints.vec.size, CSP_CONSTRAINT_SIZE);
    cr_assert_eq(REG_SIZE(obj.props), 0);
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    csp_obj_deinit(&obj);
}

Test(csp_obj, add)
{
    csp_object_t obj = {0};
    prop_t prop = {0};

    cr_assert(csp_obj_init(&obj));
    cr_assert(csp_obj_add_prop(&obj, &prop));
    cr_assert_eq(*REG_AT(prop_t*, &obj.props, 0), &prop);
    csp_obj_deinit(&obj);
}

Test(csp_obj, pick)
{
    csp_object_t obj = {0};
    prop_t prop1 = {0};
    prop_t prop2 = {0};
    prop_t *picked = NULL;

    cr_assert(csp_obj_init(&obj));
    cr_assert(csp_obj_add_prop(&obj, &prop1));
    cr_assert(csp_obj_add_prop(&obj, &prop2));
    picked = csp_obj_pick_prop(&obj);
    cr_assert(picked == &prop1 || picked == &prop2);
    csp_obj_deinit(&obj);
}

Test(csp_obj, make_placement_single_pos)
{
    csp_object_t obj = {0};
    csp_placement_t *placement = NULL;
    v2_t *pos_buf = NULL;
    v2_t pos = {12, 8};
    prop_t prop = {.type = PTYPE_CHILD};

    cr_assert(csp_obj_init(&obj));
    placement = csp_obj_make_placement(&obj, &prop, pos, 0, ORIENT_DOWN);
    cr_assert_not_null(placement);
    cr_assert_eq(placement->size, 1);
    pos_buf = placement->head->data;
    cr_assert_not_null(pos_buf);
    cr_assert(V2_EQ(pos, (*pos_buf)));
    csp_placement_destroy(placement);
    csp_obj_deinit(&obj);
}

static bool pos_is_reserved(v2_t origin, v2_t pos, v2_t *reserved, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++) {
        if (V2_EQ(V2_ADD(origin, reserved[i]), pos))
            return true;
    }
    return false;
}

Test(csp_obj, make_placement_mutiple_pos)
{
    csp_object_t obj = {0};
    csp_placement_t *placement = NULL;
    v2_t *pos_buf = NULL;
    v2_t pos = {12, 8};
    v2_t reserved[] = { {3, 5}, {10, 5}, {20, 8} };
    unsigned int size = sizeof(reserved) / sizeof(v2_t);
    prop_t prop = {.type = PTYPE_CHILD};

    cr_assert(csp_obj_init(&obj));
    for (unsigned int i = 0; i < size; i++) {
        cr_assert(csp_set_reserved_space(&obj, true, reserved[i]));
    }
    placement = csp_obj_make_placement(&obj, &prop, pos, 0, ORIENT_DOWN);
    cr_assert_not_null(placement);
    cr_assert_eq(placement->size, size + 1);
    pos_buf = placement->head->data;
    cr_assert_not_null(pos_buf);
    cr_assert(V2_EQ(pos, (*pos_buf)));
    for (unsigned int i = 0; i < size; i++) {
        pos_buf = csp_placement_get(placement, i);
        cr_assert_not_null(pos_buf);
        cr_assert(pos_is_reserved(pos, *pos_buf, reserved, size) || V2_EQ((*pos_buf), pos),
            "(%d,%d) is not a valid reserved position.", pos_buf->x, pos_buf->y);
    }
    csp_placement_destroy(placement);
    csp_obj_deinit(&obj);
}

#endif