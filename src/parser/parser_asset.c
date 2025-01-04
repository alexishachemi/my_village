#include <stdio.h>
#include "asset.h"
#include "parser.h"
#include "cJSON.h"
#include "render.h"
#include "world.h"

static bool get_asset(parser_t *parser, const char *name, asset_t **asset)
{
    if (!asset)
        return false;
    *asset = world_get_asset(parser->world, name);
    if (!*asset)
        return parser_raise_invalid_value(parser, name, name, "Existing asset name");
    return true;
}

static bool new_asset(parser_t *parser, const char *name,
    const char *texture_name, Rectangle draw_rect, asset_t **asset_ptr)
{
    static size_t id = 0;
    char buf[128] = {0};
    asset_t *asset = NULL;
    texture_t *texture = renderer_get_texture(parser->renderer, texture_name);

    if (!texture)
        return parser_raise_invalid_value(parser, name, texture_name, "Existing texture name");
    if (!name) {
        snprintf(buf, sizeof(buf), "Anonymous asset #%ld", id++);
        name = buf;
    }
    asset = world_new_asset(parser->world, name, texture, draw_rect);
    if (asset_ptr)
        *asset_ptr = asset;
    return asset;
}

static bool parse_array_asset_rect(parser_t *parser, cJSON *item, Rectangle *buf)
{
    cJSON *curr = NULL;
    int rect[4] = {0};

    for (unsigned int i = 1; i < 5; i++) {
        curr = cJSON_GetArrayItem(item, i);
        if (!parse_positive_int(parser, NULL, curr, rect + (i - 1)))
            return false;
    }
    buf->x = rect[0];
    buf->y = rect[1];
    buf->width = rect[2];
    buf->height = rect[3];
    return true;
}

static bool parse_array_asset(parser_t *parser, cJSON *item, const char *name, asset_t **asset)
{
    cJSON *curr = NULL;
    Rectangle rect = {0};
    char *texture_name = NULL;
    unsigned int size = cJSON_GetArraySize(item);

    if (size != 5)
        return parser_raise_invalid_value(parser, name, get_json_array_size(item), "Array (5)");
    curr = cJSON_GetArrayItem(item, 0);
    if (!cJSON_IsString(curr))
        return parser_raise_invalid_type(parser, NULL, curr, "String");
    texture_name = cJSON_GetStringValue(curr);
    if (!parse_array_asset_rect(parser, item, &rect))
        return false;
    return new_asset(parser, name, texture_name, rect, asset);
}

bool parse_asset(parser_t *parser, cJSON *item, const char *name, asset_t **asset)
{
    if (!asset)
        return false;
    if (cJSON_IsString(item))
        return get_asset(parser, cJSON_GetStringValue(item), asset);
    if (cJSON_IsArray(item))
        return parse_array_asset(parser, item, name, asset);
    return parser_raise_invalid_type(parser, name, item, "String | Array");
}

bool parse_new_asset(parser_t *parser, cJSON *item, const char *name)
{
    if (!cJSON_IsArray(item))
        return parser_raise_invalid_type(parser, name, item, "Array");
    return parse_array_asset(parser, item, name, NULL);
}

bool parse_assets(parser_t *parser)
{
    PARSE_OPTIONAL_ENTER(parser, "assets");
    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!parse_new_asset(parser, curr, curr->string))
            return false;
    }
    PARSE_EXIT(parser);
    return true;
}