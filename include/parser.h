#pragma once

#include "asset.h"
#include "cJSON.h"
#include "csp.h"
#include "render.h"
#include "texture.h"

#define PARSER_HISTORY_BASE_SIZE 3
#define PARSER_HISTORY_NAME_SIZE 16

#define PARSE_ENTER(parserp, name) if (!parser_enter((parserp), (name), true)) return false
#define PARSE_ENTER_ANY(parserp, name) if (!parser_enter((parserp), (name), false)) return false
#define PARSE_OPTIONAL(parserp, name) if (!cJSON_HasObjectItem((parserp)->current, name)) return true
#define PARSE_OPTIONAL_ENTER(parserp, name) PARSE_OPTIONAL(parserp, name); PARSE_ENTER(parserp, name)
#define PARSE_OPTIONAL_ENTER_ANY(parserp, name) PARSE_OPTIONAL(parserp, name); PARSE_ENTER_ANY(parserp, name)
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


typedef bool (*constraint_parser_t)(parser_t*, const char*, csp_object_t*, bool, cJSON*);
typedef struct {
    const char *name;
    constraint_parser_t parse;
} constraint_parser_map_t;

typedef bool (*global_constraint_parser_t)(parser_t*, const char*, csp_room_t*, bool, cJSON*);
typedef struct {
    const char *name;
    global_constraint_parser_t parse;
} global_constraint_parser_map_t;

bool parser_init(parser_t *parser);
void parser_deinit(parser_t *parser);
bool parse_config(world_t *world, renderer_t *renderer, const char *path);

// Path

bool parser_enter(parser_t *parser, const char *name, bool is_obj);
bool parser_exit(parser_t *parser);
char *parser_get_path(parser_t *parser);

// Error (Note: raise functions always return false)

bool parser_raise_syntax_error(parser_t *parser);
bool parser_raise_missing_value(parser_t *parser, const char *missing, const char *expected);
bool parser_raise_invalid_type(parser_t *parser, const char *name, cJSON *got, const char *expected);
bool parser_raise_invalid_value(parser_t *parser, const char *name, const char *got, const char *expected);
bool parser_raise_error(parser_t *parser, const char *msg, ...);

// Utils

const char *get_json_array_size(cJSON *item);
const char *get_json_type(cJSON *item);

// Sub-Parsers

bool parse_render(parser_t *parser);
bool parse_world(parser_t *parser);
bool parse_textures(parser_t *parser);
bool parse_assets(parser_t *parser);
bool parse_terrains(parser_t *parser);
bool parse_props(parser_t *parser);
bool parse_biomes(parser_t *parser);
bool parse_csp_rooms(parser_t *parser);

bool parse_int(parser_t *parser, const char *name, cJSON *item, int *buf);
bool parse_positive_int(parser_t *parser, const char *name, cJSON *item, int *buf);
bool parse_non_zero_positive_int(parser_t *parser, const char *name, cJSON *item, int *buf);
bool parse_v2(parser_t *parser, const char *name, cJSON *item, v2_t *buf);
bool parse_positive_v2(parser_t *parser, const char *name, cJSON *item, v2_t *buf);
bool parse_non_zero_positive_v2(parser_t *parser, const char *name, cJSON *item, v2_t *buf);
bool parse_texture(parser_t *parser, cJSON *item, texture_t **texture);
bool parse_asset(parser_t *parser, cJSON *item, const char *name, asset_t **asset);
bool parse_array_asset(parser_t *parser, cJSON *item, const char *name, asset_t **buf);
bool parse_prop(parser_t *parser, const char *name);
bool parse_rate(parser_t *parser, const char *name, cJSON *item, float *buf);
bool parse_csp_obj(parser_t *parser, cJSON *item, csp_object_t *buf);
bool parse_csp_local_constraint(parser_t *parser, const char *name, cJSON *item, csp_object_t *obj);
bool parse_csp_local_constraints(parser_t *parser, const char *name, cJSON *item, csp_object_t *obj);
bool parse_csp_global_constraint(parser_t *parser, const char *name, cJSON *item, csp_room_t *room);
bool parse_csp_global_constraints(parser_t *parser, const char *name, cJSON *item, csp_room_t *room);
bool parse_terrain(parser_t *parser, cJSON *item, const char *name, terrain_t **buf);
bool parse_csp_room(parser_t *parser, const char *name, cJSON *item, csp_room_t *buf);
bool parse_orient(parser_t *parser, const char *name, cJSON *item, orient_t *buf);

/// Constraints

bool parser_get_csp_arg(parser_t *parser, cJSON *args, unsigned int index, cJSON **buf);

// Global Constraints

bool parse_csp_all_cell_connected(
    parser_t *parser,
    const char *name,
    csp_room_t *room,
    bool expected,
    cJSON *args
);

// Local Constraints

bool parse_csp_all_cell_connected(
    parser_t *parser,
    const char *name,
    csp_room_t *room,
    bool expected,
    cJSON *args
);
bool parse_csp_set_adjacent_to_prop(
    parser_t *parser,
    const char *name,
    csp_object_t *obj,
    bool expected,
    cJSON *args
);
bool parse_csp_set_adjacent_to_wall(
    parser_t *parser,
    const char *name,
    csp_object_t *obj,
    bool expected,
    cJSON *args
);
bool parse_csp_set_has_orient(
    parser_t *parser,
    const char *name,
    csp_object_t *obj,
    bool expected,
    cJSON *args
);
bool parse_csp_set_in_corner(
    parser_t *parser,
    const char *name,
    csp_object_t *obj,
    bool expected,
    cJSON *args
);
bool parse_csp_set_on_ground(
    parser_t *parser,
    const char *name,
    csp_object_t *obj,
    bool expected,
    cJSON *args
);
bool parse_csp_set_on_top_of_prop(
    parser_t *parser,
    const char *name,
    csp_object_t *obj,
    bool expected,
    cJSON *args
);