#include "asset.h"
#include "parser.h"
#include "world.h"

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