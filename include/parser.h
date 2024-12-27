#pragma once

#include "asset.h"
#include "cJSON.h"
#include "render.h"
#include "texture.h"
#include "world.h"

#define PARSER_HISTORY_BASE_SIZE 3
#define PARSER_HISTORY_NAME_SIZE 16

#define PARSE_ENTER(parserp, name) if (!parser_enter((parserp), (name))) return false
#define PARSE_EXIT(parserp) if (!parser_exit((parserp))) return false

typedef struct {
    char name[PARSER_HISTORY_NAME_SIZE];
    cJSON *item;
} parser_history_t;

typedef struct {
    char *filepath;
    char *raw;
    cJSON *root;
    cJSON *current;
    world_t *world;
    renderer_t *renderer;
    reg_t history;
} parser_t;

bool parser_init(parser_t *parser);
void parser_deinit(parser_t *parser);
bool parse_config(world_t *world, renderer_t *renderer, const char *path);

// Path

bool parser_enter(parser_t *parser, const char *name);
bool parser_exit(parser_t *parser);
char *parser_get_path(parser_t *parser);

// Error (Note: raise functions always return false)

bool parser_raise_syntax_error(parser_t *parser);
bool parser_raise_missing_value(parser_t *parser, const char *missing, const char *expected);
bool parser_raise_invalid_type(parser_t *parser, const char *name, cJSON *got, const char *expected);
bool parser_raise_invalid_value(parser_t *parser, const char *name, const char *got, const char *expected);

// Sub-Parsers

bool parse_render(parser_t *parser);
bool parse_world(parser_t *parser);
bool parse_textures(parser_t *parser);

bool parse_int(parser_t *parser, const char *name, cJSON *item, bool raise_on_fail, int *buf);
bool parse_positive_int(parser_t *parser, const char *name, cJSON *item, bool raise_on_fail, int *buf);
bool parse_v2(parser_t *parser, const char *name, cJSON *item, bool raise_on_fail, v2_t *buf);
bool parse_positive_v2(parser_t *parser, const char *name, cJSON *item, bool raise_on_fail, v2_t *buf);
bool parse_asset(parser_t *parser, const char *name, cJSON *item,
    bool raise_on_fail, const char *n, asset_t **buf);
bool parse_texture(parser_t *parser, cJSON *item, bool raise_on_fail, texture_t **texture);