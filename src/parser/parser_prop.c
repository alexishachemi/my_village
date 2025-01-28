#include "asset.h"
#include "cJSON.h"
#include "parser.h"
#include "prop.h"
#include "world.h"

static bool parse_mono_asset_str(parser_t *parser, cJSON *item, prop_t *prop)
{
    char *name = cJSON_GetStringValue(item);
    asset_t *asset = world_get_asset(parser->world, name);

    if (!asset)
        return parser_raise_invalid_value(parser, "assets", name, "Existing asset name");
    return prop_set_mono_asset(prop, asset);
}

static bool parse_mono_asset_arr(parser_t *parser, cJSON *item, prop_t *prop)
{
    asset_t *asset = NULL;

    if (!parse_asset(parser, item, NULL, &asset))
        return false;
    return prop_set_mono_asset(prop, asset);
}

static bool get_asset(parser_t *parser, cJSON *item, const char *direction, asset_t **asset)
{
    *asset = NULL;
    cJSON *dir_item = cJSON_GetObjectItem(item, direction);

    if (!dir_item)
        return true;
    if (!parse_asset(parser, dir_item, NULL, asset))
        return false;
    return true;
}

static bool parse_multi_asset(parser_t *parser, cJSON *item, prop_t *prop)
{
    asset_t *assets[4] = {0};
    bool success = get_asset(parser, item, "up", &assets[0])
                && get_asset(parser, item, "down", &assets[1])
                && get_asset(parser, item, "left", &assets[2])
                && get_asset(parser, item, "right", &assets[3]);
    if (!success)
        return false;
    return prop_set_multi_asset(prop, assets[0], assets[1], assets[2], assets[3]);
}

static bool parse_prop_assets(parser_t *parser, cJSON *item, prop_t *prop)
{
    if (!item)
        return parser_raise_missing_value(parser, "assets", "String | Object | Array");
    if (cJSON_IsString(item))
        return parse_mono_asset_str(parser, item, prop);
    if (cJSON_IsArray(item))
        return parse_mono_asset_arr(parser, item, prop);
    if (cJSON_IsObject(item))
        return parse_multi_asset(parser, item, prop);
    return parser_raise_invalid_type(parser, item->string, item, "String | Object | Array");
}

static bool parse_prop_child(parser_t *parser, cJSON *item, prop_t *prop)
{
    prop_t *child = NULL;
    v2_t offset = {0};

    if (!cJSON_IsObject(item))
        return parser_raise_invalid_type(parser, NULL, item, "Object");
    if (!parse_v2(parser, "offset", cJSON_GetObjectItem(item, "offset"), &offset))
        return false;
    child = prop_add_child(prop, offset);
    if (!child)
        return parser_raise_error(parser, "Failed to create child for prop \"%s\"", prop->name);
    return parse_prop_assets(parser, cJSON_GetObjectItem(item, "assets"), child);
}

static bool parse_prop_children(parser_t *parser, prop_t *prop)
{
    PARSE_OPTIONAL_ENTER_ANY(parser, "children");
    cJSON *curr = NULL;

    if (!cJSON_IsArray(parser->current))
        return parser_raise_invalid_type(parser, "children", parser->current, "Array");
    cJSON_ArrayForEach(curr, parser->current) {
        if (!parse_prop_child(parser, curr, prop))
            return false;
    }
    PARSE_EXIT(parser);
    return true;
}

bool parse_prop(parser_t *parser, const char *name)
{
    PARSE_ENTER(parser, name);
    prop_t *prop = world_new_prop(parser->world, name);
    cJSON *item = NULL;
    int z_index = 0;

    if (!prop)
        return parser_raise_error(parser, "Failed to create prop \"%s\"", name);
    if (!parse_prop_assets(parser, cJSON_GetObjectItem(parser->current, "assets"), prop))
        return false;
    item = cJSON_GetObjectItem(parser->current, "z_index");
    if (item) {
        if (!parse_int(parser, "z_index" , item, &z_index))
            return false;
        prop_set_z_index(prop, z_index);
    }
    if (!parse_prop_children(parser, prop))
        return false;
    PARSE_EXIT(parser);
    return true;
}

bool parse_props(parser_t *parser)
{
    PARSE_OPTIONAL_ENTER(parser, "props");
    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!parse_prop(parser, curr->string))
            return false;
    }
    PARSE_EXIT(parser);
    return true;
}