#include "asset.h"

bool asset_init(asset_t *asset, const char *name, const char *path)
{
    if (!asset || !name || !path)
        return false;
    namecpy(asset->name, name);
    pathcpy(asset->path, path);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(asset, init)
{
    asset_t asset = {0};
 
    cr_assert_eq(asset_init(&asset, "name", "path"), true);
    cr_assert_str_eq(asset.name, "name");
    cr_assert_str_eq(asset.path, "path");
}

#endif