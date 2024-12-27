#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "parser.h"
#include "p_color.h"

static char json_str[64] = {0};

const char *get_json_array_size(cJSON *item)
{
    snprintf(json_str, sizeof(json_str), "Array (%d)", cJSON_GetArraySize(item));
    return json_str;
}

const char *get_json_type(cJSON *item)
{
    if (cJSON_IsNumber(item))
        return strcpy(json_str, "Number");
    if (cJSON_IsBool(item))
        return strcpy(json_str, "Bool");
    if (cJSON_IsNull(item))
        return strcpy(json_str, "Null");
    if (cJSON_IsObject(item))
        return strcpy(json_str, "Object");
    if (cJSON_IsString(item))
        return strcpy(json_str, "String");
    if (cJSON_IsArray(item)) {
        if (cJSON_GetArraySize(item) == 2 
            && cJSON_IsNumber(cJSON_GetArrayItem(item, 0))
            && cJSON_IsNumber(cJSON_GetArrayItem(item, 1))) {
            return strcpy(json_str, "Vector2");
        }
        return strcpy(json_str, "Array");
    }
    return strcpy(json_str, "Unknown");
}

static void print_begin(parser_t *parser)
{
    char *path = parser_get_path(parser);

    if (!path)
        return;
    dprintf(2, "=== " P_RED "Error " P_END "at " P_CYAN "[%s]" P_END " ", path);
    free(path);
}

static void print_end()
{
    dprintf(2, " ===\n");
}

static unsigned int count_lines(const char *str)
{
    unsigned int lines = 0;

    for (unsigned int i = 0; str[i]; i++) {
        lines += str[i] == '\n';
    }
    return lines;
}

bool parser_raise_syntax_error(parser_t *parser)
{
    unsigned int line = 0;

    if (!parser)
        return false;
    line = count_lines(parser->raw) - count_lines(cJSON_GetErrorPtr());
    dprintf(2, "=== " P_RED "Syntax Error " P_END "near " P_CYAN "[%s:%d]" P_END " ===\n", parser->filepath, line);
    return false;
}

bool parser_raise_missing_value(parser_t *parser, const char *missing, const char *expected)
{
    if (!parser)
        return false;
    print_begin(parser);
    dprintf(2, "Unexpected missing value: " P_WHITE "%s" P_END " (expected: %s)",
        missing ? missing : "...", expected);
    print_end();
    return false;
}

bool parser_raise_invalid_type(parser_t *parser, const char *name, cJSON *got, const char *expected)
{
    if (!parser || !got || !expected)
        return false;
    print_begin(parser);
    dprintf(2, "Invalid type: " P_WHITE "%s" P_END " (got: %s, expected: %s)",
        name ? name : "...", get_json_type(got), expected);
    print_end();
    return false;
}

bool parser_raise_invalid_value(parser_t *parser, const char *name, const char *got, const char *expected)
{
    if (!parser || !got || !expected)
        return false;
    print_begin(parser);
    dprintf(2, "Invalid value: " P_WHITE "%s" P_END " (got: %s, expected: %s)",
        name ? name : "...", got, expected);
    print_end();
    return false;
}