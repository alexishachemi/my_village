
#include "assets.h"
#include "orientation.h"
#include "prop.h"
#include <stdio.h>

int main(void)
{
    asset_manager_t am;

    assetm_init(&am);
    size_t up_id = assetm_add(&am, "up", "path/to/asset");
    size_t down_id = assetm_add(&am, "down", "path/to/asset");
    size_t left_id = assetm_add(&am, "left", "path/to/asset");
    size_t right_id = assetm_add(&am, "right", "path/to/asset");

    prop_manager_t pm;

    propm_init(&pm);
    prop_t *prop = propm_new(&pm, "my_prop", &(prop_asset_map_t){
        .asset_mode = MULTI_ASSET,
        .asset_id_up = up_id,
        .asset_id_down = down_id,
        .asset_id_left = left_id,
        .asset_id_right = right_id
    });

    printf("prop up -> %ld\n", prop_get_asset_id(prop, ORIENT_UP));
    printf("prop down -> %ld\n", prop_get_asset_id(prop, ORIENT_DOWN));
    printf("prop left -> %ld\n", prop_get_asset_id(prop, ORIENT_LEFT));
    printf("prop right -> %ld\n", prop_get_asset_id(prop, ORIENT_RIGHT));

    assetm_deinit(&am);
    propm_deinit(&pm);
    return 0;
}
