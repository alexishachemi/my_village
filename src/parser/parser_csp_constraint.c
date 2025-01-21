#include "cJSON.h"
#include "csp.h"
#include "parser.h"
#include "str.h"

////////////////////////////////////////////////////////////// LOCAL

constraint_parser_map_t c_parsers[] = {
    {"adjacent_to_prop", parse_csp_set_adjacent_to_prop},
    {NULL, NULL}
};

static bool run_constraint_parser(
    parser_t *parser,
    const char *c_name,
    const char *name,
    csp_object_t *obj,
    bool expected,
    cJSON *args
)
{
    constraint_parser_map_t *map = NULL;

    for (unsigned int i = 0; c_parsers[i].name != NULL; i++) {
        map = c_parsers + i;
        if (STR_EQ(map->name, c_name))
            return map->parse(parser, name, obj, expected, args);
    }
    return parser_raise_error(parser, "Unknown constraint %s", c_name);
}

bool parse_csp_local_constraint(
    parser_t *parser,
    const char *name,
    cJSON *item,
    csp_object_t *obj)
{
    cJSON *curr = NULL;
    const char *c_name = NULL;
    bool c_expected = false;

    if (!cJSON_IsArray(item))
        return parser_raise_invalid_type(parser, name, item, "Array");
    curr = cJSON_GetArrayItem(item, 0);
    if (!curr)
        parser_raise_invalid_value(parser, name, "Array (< 2)", "Array (>= 2)");
    if (!cJSON_IsBool(curr))
        return parser_raise_invalid_type(parser, name, curr, "Bool");
    c_expected = cJSON_IsTrue(curr);
    curr = cJSON_GetArrayItem(item, 1);
    if (!curr)
        parser_raise_invalid_value(parser, name, "Array (< 2)", "Array (>= 2)");
    if (!cJSON_IsString(curr))
        return parser_raise_invalid_type(parser, name, curr, "String");
    c_name = cJSON_GetStringValue(curr);
    return run_constraint_parser(parser, c_name, name, obj, c_expected, cJSON_GetArrayItem(item, 2));
}

bool parse_csp_local_constraints(parser_t *parser, const char *name, cJSON *item, csp_object_t *obj)
{
    cJSON *curr = NULL;

    if (!item)
        parser_raise_missing_value(parser, name, "Array");
    if (!parser || !obj)
        return false;
    if (!cJSON_IsArray(item))
        return parser_raise_invalid_type(parser, name, item, "Array");
    cJSON_ArrayForEach(curr, item) {
        if (!parse_csp_local_constraint(parser, name, curr, obj))
            return false;
    }
    return true;
}

///////////////////////////////////////////////////////////// GLOBAL

global_constraint_parser_map_t gc_parsers[] = {
    {"all_cells_connected", parse_csp_all_cell_connected},
    {NULL, NULL}
};

static bool run_global_constraint_parser(
    parser_t *parser,
    const char *c_name,
    const char *name,
    csp_room_t *room,
    bool expected,
    cJSON *args
)
{
    global_constraint_parser_map_t *map = NULL;

    for (unsigned int i = 0; gc_parsers[i].name != NULL; i++) {
        map = gc_parsers + i;
        if (STR_EQ(map->name, c_name))
            return map->parse(parser, name, room, expected, args);
    }
    return parser_raise_error(parser, "Unknown global constraint %s", c_name);
}

bool parse_csp_global_constraint(
    parser_t *parser,
    const char *name,
    cJSON *item,
    csp_room_t *room)
{
    cJSON *curr = NULL;
    const char *c_name = NULL;
    bool c_expected = false;

    if (!cJSON_IsArray(item))
        return parser_raise_invalid_type(parser, name, item, "Array");
    curr = cJSON_GetArrayItem(item, 0);
    if (!curr)
        parser_raise_invalid_value(parser, name, "Array (< 2)", "Array (>= 2)");
    if (!cJSON_IsBool(curr))
        return parser_raise_invalid_type(parser, name, curr, "Bool");
    c_expected = cJSON_IsTrue(curr);
    curr = cJSON_GetArrayItem(item, 1);
    if (!curr)
        parser_raise_invalid_value(parser, name, "Array (< 2)", "Array (>= 2)");
    if (!cJSON_IsString(curr))
        return parser_raise_invalid_type(parser, name, curr, "String");
    c_name = cJSON_GetStringValue(curr);
    return run_global_constraint_parser(
        parser,
        c_name,
        name,
        room,
        c_expected,
        cJSON_GetArrayItem(item, 2)
    );
}

bool parse_csp_global_constraints(parser_t *parser, const char *name, cJSON *item, csp_room_t *room)
{
    cJSON *curr = NULL;

    if (!item)
        parser_raise_missing_value(parser, name, "Array");
    if (!parser || !room)
        return false;
    if (!cJSON_IsArray(item))
        return parser_raise_invalid_type(parser, name, item, "Array");
    cJSON_ArrayForEach(curr, item) {
        if (!parse_csp_global_constraint(parser, name, curr, room))
            return false;
    }
    return true;
}
