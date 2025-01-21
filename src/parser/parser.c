#include <stdbool.h>
#include <string.h>
#include "parser.h"
#include "cJSON.h"
#include "raylib.h"
#include "render.h"
#include "world.h"

static bool load(parser_t *parser, world_t *world, renderer_t *renderer, const char *path)
{
    if (!world || !renderer || !path)
        return false;
    parser->filepath = strdup(path);
    if (!parser->filepath)
        return false;
    parser->raw = LoadFileText(path);
    if (!parser->raw) {
        parser_deinit(parser);
        return false;
    }
    parser->root = cJSON_Parse(parser->raw);
    if (!parser->root) {
        parser_raise_syntax_error(parser);
        parser_deinit(parser);
        return false;
    }
    parser->world = world;
    parser->renderer = renderer;
    parser->current = parser->root;
    return true;
}

static bool parse(parser_t *parser)
{
    return parse_render(parser)
        && parse_world(parser)
        && parse_textures(parser)
        && parse_assets(parser)
        && parse_terrains(parser)
        && parse_props(parser)
        && parse_biomes(parser)
        && parse_csp_rooms(parser);
}

bool parse_config(world_t *world, renderer_t *renderer, const char *path)
{
    parser_t parser = {0};
    bool success = false;

    if (!parser_init(&parser) || !load(&parser, world, renderer, path))
        return false;
    success = parse(&parser);
    parser_deinit(&parser);
    if (!success) {
        world_deinit(world);
        renderer_deinit(renderer);
    }
    return success;
}