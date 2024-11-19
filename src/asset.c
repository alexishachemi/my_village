#include "asset.h"

bool asset_init(asset_t *asset, const char *name, const char *path)
{
    if (!asset || !name || !path)
        return false;
    namecpy(asset->name, name);
    pathcpy(asset->path, path);
    return true;
}
