#include "prop.h"

bool prop_init(prop_t *prop, const char *name)
{
    if (!prop || !name)
        return false;
    namecpy(prop->name, name);
    prop->asset_map.asset_mode = AMODE_NONE;
    prop->z_index = 0;
    return true;
}

bool prop_set_z_index(prop_t *prop, z_index_t z_index)
{
    if (!prop)
        return false;
    prop->z_index = z_index;
    return true;
}

bool prop_set_mono_asset(prop_t *prop, size_t asset_id)
{
    if (!prop)
        return false;
    prop->asset_map.asset_mode = AMODE_MONO;
    prop->asset_map.asset_id = asset_id;
    return true;
}

bool prop_set_multi_asset(
    prop_t *prop,
    size_t asset_id_up,
    size_t asset_id_down,
    size_t asset_id_left,
    size_t asset_id_right
)
{
    if (!prop)
        return false;
    prop->asset_map.asset_mode = AMODE_MULTI;
    prop->asset_map.asset_id_up = asset_id_up;
    prop->asset_map.asset_id_down = asset_id_down;
    prop->asset_map.asset_id_left = asset_id_left;
    prop->asset_map.asset_id_right = asset_id_right;
    return true;
}

ssize_t prop_get_asset_id(const prop_t *prop, orient_t orient)
{
    if (!prop || prop->asset_map.asset_mode == AMODE_NONE)
        return -1;
    if (prop->asset_map.asset_mode == AMODE_MONO)
        return prop->asset_map.asset_id;
    switch (orient) {
        case ORIENT_UP: return prop->asset_map.asset_id_up;
        case ORIENT_DOWN: return prop->asset_map.asset_id_down;
        case ORIENT_LEFT: return prop->asset_map.asset_id_left;
        case ORIENT_RIGHT: return prop->asset_map.asset_id_right;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(prop, init)
{
    prop_t prop = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert_str_eq(prop.name, "name");
    cr_assert_eq(prop.asset_map.asset_mode, AMODE_NONE);
    cr_assert_eq(prop.z_index, 0);
}

Test(prop, set_mono_asset)
{
    prop_t prop = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert(prop_set_mono_asset(&prop, 1));
    cr_assert_eq(prop.asset_map.asset_mode, AMODE_MONO);
    cr_assert_eq(prop.asset_map.asset_id, 1);
}

Test(prop, set_multi_asset)
{
    prop_t prop = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert(prop_set_multi_asset(&prop, 1, 2, 3, 4));
    cr_assert_eq(prop.asset_map.asset_mode, AMODE_MULTI);
    cr_assert_eq(prop.asset_map.asset_id_up, 1);
    cr_assert_eq(prop.asset_map.asset_id_down, 2);
    cr_assert_eq(prop.asset_map.asset_id_left, 3);
    cr_assert_eq(prop.asset_map.asset_id_right, 4);
}

Test(prop, get_asset_id)
{
    prop_t prop = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert_eq(prop_get_asset_id(&prop, ORIENT_UP), -1);
    cr_assert(prop_set_mono_asset(&prop, 1));
    cr_assert_eq(prop_get_asset_id(&prop, ORIENT_UP), 1);
    cr_assert(prop_set_multi_asset(&prop, 1, 2, 3, 4));
    cr_assert_eq(prop_get_asset_id(&prop, ORIENT_UP), 1);
    cr_assert_eq(prop_get_asset_id(&prop, ORIENT_DOWN), 2);
    cr_assert_eq(prop_get_asset_id(&prop, ORIENT_LEFT), 3);
    cr_assert_eq(prop_get_asset_id(&prop, ORIENT_RIGHT), 4);
}

#endif
