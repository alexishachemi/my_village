#include "terrain.h"

bool terrain_init(terrain_t *terrain, const char *name, asset_t *asset)
{
    if (!terrain || !name || !asset)
        return false;
    namecpy(terrain->name, name);
    terrain->asset = asset;
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(terrain, init)
{
    terrain_t terrain = {0};
    asset_t asset = {0};

    cr_assert(terrain_init(&terrain, "name", &asset));
    cr_assert_str_eq(terrain.name, "name");
    cr_assert_eq(terrain.asset, &asset);
}

Test(terrain, init_null)
{
    terrain_t terrain = {0};
    asset_t asset = {0};

    cr_assert_not(terrain_init(NULL, "name", &asset));
    cr_assert_not(terrain_init(&terrain, NULL, &asset));
    cr_assert_not(terrain_init(&terrain, "name", NULL));
}

#endif