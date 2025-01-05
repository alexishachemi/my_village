#include "biome.h"
#include "cJSON.h"
#include "parser.h"
#include "world.h"

static bool parse_biome_parents(parser_t *parser, biome_t *biome)
{
    PARSE_OPTIONAL_ENTER_ANY(parser, "parents");
    biome_t *parent = NULL;
    const char *name = NULL;

    if (!cJSON_IsArray(parser->current))
        return parser_raise_invalid_type(parser, "parents", parser->current, "Array");
    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!cJSON_IsString(curr))
            return parser_raise_invalid_type(parser, NULL, curr, "String");
        name = curr->valuestring;
        parent = world_get_biome(parser->world, name);
        if (!parent)
            return parser_raise_error(parser, "Could not find biome \"%s\"", name);
        if (parent == biome)
            return parser_raise_error(parser, "Biome \"%s\" cannot have itself as a parent", name);
        if (!biome_add_parent(biome, parent))
            return parser_raise_error(parser, "Failed to add parent \"%s\" to biome \"%s\"",
                name, biome->name);
    }
    PARSE_EXIT(parser);
    return true;
}

static bool parse_biome_terrains(parser_t *parser, biome_t *biome)
{
    PARSE_OPTIONAL_ENTER_ANY(parser, "terrains");
    terrain_t *terrain = NULL;
    const char *name = NULL;

    if (!cJSON_IsArray(parser->current))
        return parser_raise_invalid_type(parser, "terrains", parser->current, "Array");
    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!cJSON_IsString(curr))
            return parser_raise_invalid_type(parser, NULL, curr, "String");
        name = curr->valuestring;
        terrain = world_get_terrain(parser->world, name);
        if (!terrain)
            return parser_raise_error(parser, "Could not find terrain \"%s\"", name);
        if (biome_get_terrain(biome, name))
            return parser_raise_error(parser, "Multiple instance of terrain \"%s\" in biome \"%s\"",
                name, biome->name);
        if (!biome_add_terrain(biome, terrain))
            return parser_raise_error(parser, "Failed to add terrain \"%s\" to biome \"%s\"",
                name, biome->name);
    }
    PARSE_EXIT(parser);
    return true;
}

static bool parse_biome_props(parser_t *parser, biome_t *biome)
{
    PARSE_OPTIONAL_ENTER_ANY(parser, "props");
    prop_t *prop = NULL;
    const char *name = NULL;

    if (!cJSON_IsArray(parser->current))
        return parser_raise_invalid_type(parser, "props", parser->current, "Array");
    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!cJSON_IsString(curr))
            return parser_raise_invalid_type(parser, NULL, curr, "String");
        name = curr->valuestring;
        prop = world_get_prop(parser->world, name);
        if (!prop)
            return parser_raise_error(parser, "Could not find prop \"%s\"", name);
        if (biome_get_prop(biome, name))
            return parser_raise_error(parser, "Multiple instance of prop \"%s\" in biome \"%s\"",
                name, biome->name);
        if (!biome_add_prop(biome, prop))
            return parser_raise_error(parser, "Failed to add prop \"%s\" to biome \"%s\"",
                name, biome->name);
    }
    PARSE_EXIT(parser);
    return true;
}

static bool parse_new_biome(parser_t *parser, const char *name)
{
    PARSE_ENTER(parser, name);
    biome_t *biome = world_new_biome(parser->world, name);
    bool success = false;

    if (!biome)
        return parser_raise_error(parser, "Failed to create biome \"%s\"", name);
    
    success = parse_biome_parents(parser, biome)
           && parse_biome_terrains(parser, biome)
           && parse_biome_props(parser, biome);
    PARSE_EXIT(parser);
    return success;
}

bool parse_biomes(parser_t *parser)
{
    PARSE_OPTIONAL_ENTER(parser, "biomes");
    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!parse_new_biome(parser, curr->string))
            return false;
    }
    PARSE_EXIT(parser);
    return true;
}