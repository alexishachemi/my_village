#include "terrain.h"

bool terrain_init(terrain_t *terrain, const char *name, size_t asset_id)
{
    if (!terrain || !name)
        return false;
    namecpy(terrain->name, name);
    terrain->asset_id = asset_id;
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(terrain, init)
{
    terrain_t terrain = {0};

    cr_assert_eq(terrain_init(&terrain, "name", 0), true);
    cr_assert_str_eq(terrain.name, "name");
    cr_assert_eq(terrain.asset_id, 0);
}

#endif