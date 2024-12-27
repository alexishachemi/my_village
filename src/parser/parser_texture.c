#include "cJSON.h"
#include "parser.h"
#include "render.h"
#include "texture.h"

bool parse_texture(parser_t *parser, cJSON *item, texture_t **texture)
{
    if (!item)
        parser_raise_missing_value(parser, "...", "Object");
    if (!parser || !item || !texture)
        return false;
    if (!cJSON_IsString(item))
        return parser_raise_invalid_type(parser, item->string, item, "String");
    *texture = renderer_new_texture(parser->renderer, item->string, cJSON_GetStringValue(item));
    return *texture;
}

bool parse_textures(parser_t *parser)
{
    PARSE_OPTIONAL_ENTER(parser, "textures");
    texture_t *texture = NULL;

    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!parse_texture(parser, curr, &texture))
            return false;
    }
    PARSE_EXIT(parser);
    return true;
}
