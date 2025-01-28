#include "asset.h"
#include "cJSON.h"
#include "parser.h"
#include "str.h"
#include "world.h"
#include <stdio.h>

bool parse_terrain(parser_t *parser, cJSON *item, const char *name, terrain_t **buf)
{
    static size_t id = 0;
    asset_t *asset = NULL;
    name_t t_name = {0};

    if (!parser || !buf)
        return false;
    if (!item)
        return parser_raise_missing_value(parser, name, "String | Array (5)");
    if (cJSON_IsString(item)) {
        *buf = world_get_terrain(parser->world, cJSON_GetStringValue(item));
        if (!*buf)
            return parser_raise_error(parser, "Unknown terrain %s", cJSON_GetStringValue(item));
        return true;
    }
    if (!cJSON_IsArray(item))
        return parser_raise_invalid_type(parser, name, item, "String | Array (5)");
    if (!parse_array_asset(parser, item, name, &asset))
        return false;
    if (name) {
        namecpy(t_name, name);
    } else {
        snprintf(t_name, sizeof(t_name), "Anonymous terrain #%ld", id++);
    }
    *buf = world_new_terrain(parser->world, t_name, asset);
    if (!buf)
        return parser_raise_error(parser, "Failed to create terrain");
    return true;
}

static bool parse_new_terrain(parser_t *parser, cJSON *item, const char *name)
{
    asset_t *asset = NULL;

    if (!parse_asset(parser, item, NULL, &asset))
        return false;
    if (!world_new_terrain(parser->world, name, asset))
        return parser_raise_error(parser, "Failed to create terrain \"%s\"", name);
    return true;
}

bool parse_terrains(parser_t *parser)
{
    PARSE_OPTIONAL_ENTER(parser, "terrains");
    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!parse_new_terrain(parser, curr, curr->string))
            return false;
    }
    PARSE_EXIT(parser);
    return true;
}