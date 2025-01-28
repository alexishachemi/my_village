#include "asset.h"

bool asset_init(asset_t *asset, const char *name,
    texture_t *texture, Rectangle draw_rect)
{
    if (!asset || !name || !texture)
        return false;
    namecpy(asset->name, name);
    asset->texture = texture;
    asset->draw_rect = draw_rect;
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(asset, init)
{
    asset_t asset = {0};
    texture_t texture = {0};
    Rectangle r = {12, 34, 56, 78};
 
    cr_assert_eq(asset_init(&asset, "my_asset", &texture, r), true);
    cr_assert_str_eq(asset.name, "my_asset");
    cr_assert_not_null(asset.texture);
    cr_assert_eq(asset.draw_rect.x, 12);
}

#endif