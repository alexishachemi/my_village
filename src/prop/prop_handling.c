#include "prop.h"

bool prop_set_z_index(prop_t *prop, z_index_t z_index)
{
    if (!prop || (prop->type == PTYPE_CHILD && !prop->parent))
        return false;
    if (prop->type == PTYPE_PARENT)
        prop->z_index = z_index;
    else
        prop->parent->z_index = z_index;
    return true;
}

bool prop_set_mono_asset(prop_t *prop, asset_t *asset)
{
    if (!prop || !asset)
        return false;
    prop->asset_map.asset_mode = AMODE_MONO;
    prop->asset_map.asset = asset;
    return true;
}

bool prop_set_multi_asset(
    prop_t *prop,
    asset_t *asset_up,
    asset_t *asset_down,
    asset_t *asset_left,
    asset_t *asset_right
)
{
    if (!prop || !asset_up || !asset_down || !asset_left || !asset_right)
        return false;
    prop->asset_map.asset_mode = AMODE_MULTI;
    prop->asset_map.asset_up = asset_up;
    prop->asset_map.asset_down = asset_down;
    prop->asset_map.asset_left = asset_left;
    prop->asset_map.asset_right = asset_right;
    return true;
}

asset_t *prop_get_asset(const prop_t *prop, orient_t orient)
{
    if (!prop || prop->asset_map.asset_mode == AMODE_NONE)
        return NULL;
    if (prop->asset_map.asset_mode == AMODE_MONO)
        return prop->asset_map.asset;
    switch (orient) {
        case ORIENT_UP: return prop->asset_map.asset_up;
        case ORIENT_DOWN: return prop->asset_map.asset_down;
        case ORIENT_LEFT: return prop->asset_map.asset_left;
        case ORIENT_RIGHT: return prop->asset_map.asset_right;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(prop, set_mono_asset)
{
    prop_t prop = {0};
    asset_t asset = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert(prop_set_mono_asset(&prop, &asset));
    cr_assert_eq(prop.asset_map.asset_mode, AMODE_MONO);
    cr_assert_eq(prop.asset_map.asset, &asset);
}

Test(prop, set_multi_asset)
{
    prop_t prop = {0};
    asset_t asset_up = {0}, asset_down = {0}, asset_left = {0}, asset_right = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert(prop_set_multi_asset(&prop, &asset_up, &asset_down, &asset_left, &asset_right));
    cr_assert_eq(prop.asset_map.asset_mode, AMODE_MULTI);
    cr_assert_eq(prop.asset_map.asset_up, &asset_up);
    cr_assert_eq(prop.asset_map.asset_down, &asset_down);
    cr_assert_eq(prop.asset_map.asset_left, &asset_left);
    cr_assert_eq(prop.asset_map.asset_right, &asset_right);
}

Test(prop, get_asset)
{
    prop_t prop = {0};
    asset_t asset = {0}, asset_up = {0}, asset_down = {0}, asset_left = {0}, asset_right = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert_null(prop_get_asset(&prop, ORIENT_UP));
    cr_assert(prop_set_mono_asset(&prop, &asset));
    cr_assert_eq(prop_get_asset(&prop, ORIENT_UP), &asset);
    cr_assert(prop_set_multi_asset(&prop, &asset_up, &asset_down, &asset_left, &asset_right));
    cr_assert_eq(prop_get_asset(&prop, ORIENT_UP), &asset_up);
    cr_assert_eq(prop_get_asset(&prop, ORIENT_DOWN), &asset_down);
    cr_assert_eq(prop_get_asset(&prop, ORIENT_LEFT), &asset_left);
    cr_assert_eq(prop_get_asset(&prop, ORIENT_RIGHT), &asset_right);
}

#endif