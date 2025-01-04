#include "asset.h"
#include "cJSON.h"
#include "parser.h"
#include "prop.h"
#include "world.h"

static bool parse_mono_asset_str(parser_t *parser, prop_t *prop)
{
    PARSE_ENTER_ANY(parser, "assets");
    char *name = cJSON_GetStringValue(parser->current);
    asset_t *asset = world_get_asset(parser->world, name);

    if (!asset)
        return parser_raise_invalid_value(parser, "assets", name, "Existing asset name");
    PARSE_EXIT(parser);
    return prop_set_mono_asset(prop, asset);
}

static bool parse_mono_asset_arr(parser_t *parser, prop_t *prop)
{
    PARSE_ENTER_ANY(parser, "assets");
    asset_t *asset = NULL;

    if (!parse_asset(parser, parser->current, NULL, &asset))
        return false;
    PARSE_EXIT(parser);
    return prop_set_mono_asset(prop, asset);
}

static bool get_asset(parser_t *parser, const char *direction, asset_t **asset)
{
    *asset = NULL;
    PARSE_OPTIONAL_ENTER_ANY(parser, direction);
    if (!parse_asset(parser, parser->current, NULL, asset))
        return false;
    PARSE_EXIT(parser);
    return true;
}

static bool parse_multi_asset(parser_t *parser, prop_t *prop)
{
    PARSE_ENTER(parser, "assets");
    asset_t *assets[4] = {0};
    bool success = get_asset(parser, "up", &assets[0])
                && get_asset(parser, "down", &assets[1])
                && get_asset(parser, "left", &assets[2])
                && get_asset(parser, "right", &assets[3]);
    if (!success)
        return false;
    PARSE_EXIT(parser);
    return prop_set_multi_asset(prop, assets[0], assets[1], assets[2], assets[3]);
}

static bool parse_prop_assets(parser_t *parser, prop_t *prop)
{
    cJSON *item = cJSON_GetObjectItem(parser->current, "assets");

    if (!item)
        return parser_raise_missing_value(parser, "assets", "String | Object | Array");
    if (cJSON_IsString(item))
        return parse_mono_asset_str(parser, prop);
    if (cJSON_IsArray(item))
        return parse_mono_asset_arr(parser, prop);
    if (cJSON_IsObject(item))
        return parse_multi_asset(parser, prop);
    return parser_raise_invalid_type(parser, item->string, item, "String | Object | Array");
}

static bool parse_prop_children(parser_t *parser, prop_t *prop)
{
    PARSE_OPTIONAL_ENTER_ANY(parser, "children");
    PARSE_EXIT(parser);
    return false;
}

bool parse_prop(parser_t *parser, const char *name)
{
    PARSE_ENTER(parser, name);
    prop_t *prop = world_new_prop(parser->world, name);

    if (!prop)
        return parser_raise_error(parser, "Failed to create prop");
    if (!parse_prop_assets(parser, prop))
        return false;
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