#include "cJSON.h"
#include "parser.h"
#include "render.h"

bool parse_render(parser_t *parser)
{
    PARSE_ENTER(parser, "render");
    display_settings_t settings = {0};
    v2_t screen_size = {0};
    int tile_size_px = 0;

    if (!parse_positive_v2(parser, "screen_size",
        cJSON_GetObjectItem(parser->current, "screen_size"), true, &screen_size))
        return false;
    settings.screen_width = screen_size.x;
    settings.screen_height = screen_size.y;
    if (!parse_positive_int(parser, "tile_size_px",
        cJSON_GetObjectItem(parser->current, "tile_size_px"), true, &tile_size_px))
        return false;
    settings.tile_size_px = tile_size_px;
    PARSE_EXIT(parser);
    return renderer_init(parser->renderer, &settings);
}