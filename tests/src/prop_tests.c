#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <stddef.h>
#include "prop.h"

Test(prop, initializing_manager)
{
    prop_manager_t pm = {0};

    cr_assert(propm_init(&pm));
    cr_assert_eq(pm.last_free_index, 0);
    cr_assert_eq(vec_size(&pm.props), PROP_MANAGER_BASE_SIZE);
    propm_deinit(&pm);
}

Test(prop, creating)
{
    prop_manager_t pm = {0};
    prop_t *prop = NULL;
    prop_asset_map_t asset_map = {
        .asset_mode = MULTI_ASSET,
        .asset_id_up = 1,
        .asset_id_down = 2,
        .asset_id_left = 3,
        .asset_id_right = 4
    };

    propm_init(&pm);
    prop = propm_new(&pm, "my_prop", &asset_map);
    cr_assert_not_null(prop);
    cr_assert_str_eq(prop->name, "my_prop");
    cr_assert_eq(prop->asset_map.asset_mode, MULTI_ASSET);
    cr_assert_eq(prop->asset_map.asset_id_up, 1);
    cr_assert_eq(prop->asset_map.asset_id_down, 2);
    cr_assert_eq(prop->asset_map.asset_id_left, 3);
    cr_assert_eq(prop->asset_map.asset_id_right, 4);
    propm_deinit(&pm);
}

Test(prop, accessing_multi_assets)
{
    prop_manager_t pm = {0};
    prop_t *prop = NULL;
    prop_asset_map_t asset_map = {
        .asset_mode = MULTI_ASSET,
        .asset_id_up = 1,
        .asset_id_down = 2,
        .asset_id_left = 3,
        .asset_id_right = 4
    };

    propm_init(&pm);
    prop = propm_new(&pm, "my_prop", &asset_map);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_UP), 1);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_DOWN), 2);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_LEFT), 3);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_RIGHT), 4);
    propm_deinit(&pm);
}

Test(prop, accessing_mono_assets)
{
    prop_manager_t pm = {0};
    prop_t *prop = NULL;
    prop_asset_map_t asset_map = {
        .asset_mode = MONO_ASSET,
        .asset_id = 1
    };

    propm_init(&pm);
    prop = propm_new(&pm, "my_prop", &asset_map);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_UP), 1);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_DOWN), 1);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_LEFT), 1);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_RIGHT), 1);
    propm_deinit(&pm);
}

Test(prop, error_handling)
{
    prop_manager_t pm = {0};
    prop_t *prop = NULL;

    cr_assert_not(propm_init(NULL));
    cr_assert(propm_init(&pm));
    cr_assert_not(propm_new(NULL, "my_prop", NULL));
    cr_assert_not(propm_new(&pm, NULL, NULL));
    prop = propm_new(&pm, "my_prop", NULL);
    cr_assert(prop);
    cr_assert_eq(prop->asset_map.asset_mode, NO_ASSET_MODE);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_UP), -1);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_DOWN), -1);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_LEFT), -1);
    cr_assert_eq(prop_get_asset_id(prop, ORIENT_RIGHT), -1);
    propm_deinit(&pm);
}