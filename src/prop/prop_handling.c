#include "prop.h"

ssize_t prop_get_asset_id(const prop_t *prop, orient_t orient)
{
    if (!prop)
        return -1;
    if (prop->asset_map.asset_mode == MONO_ASSET)
        return prop->asset_map.asset_id;
    switch (orient) {
        case ORIENT_UP: return prop->asset_map.asset_id_up;
        case ORIENT_DOWN: return prop->asset_map.asset_id_down;
        case ORIENT_LEFT: return prop->asset_map.asset_id_left;
        case ORIENT_RIGHT: return prop->asset_map.asset_id_right;
    }
    return -1;
}
