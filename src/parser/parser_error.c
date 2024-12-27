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

static unsigned int get_error_line(const char *str, const char *at,
    const char **prev)
{
    unsigned int lines = 1;
    const char *curr = NULL;

    *prev = str;
    curr = str;
    for (unsigned int i = 0; str[i] && (str + i) != at; i++) {
        if (str[i] != '\n')
            continue;
        lines += 1;
        *prev = curr;
        curr = str + (i + 1);
    }
    return lines;
}

static void print_err_line(const char **line, unsigned int err_line, unsigned int line_nb)
{
    if (!**line)
        return;
    dprintf(2, "=== %d\t| ", line_nb + (err_line == 1));
    while (**line && **line != '\n') {
        dprintf(2, "%c", **line);
        (*line)++;
    }
    if (**line == '\n')
        (*line)++;
    dprintf(2, "\n");
}

bool parser_raise_syntax_error(parser_t *parser)
{
    unsigned int line = 0;
    const char *line_ptr = NULL;

    if (!parser)
        return false;
    line = get_error_line(parser->raw, cJSON_GetErrorPtr(), &line_ptr);
    dprintf(2, "=== " P_RED "Syntax Error " P_END "near " P_CYAN "[%s:%d]" P_END " ===\n===\n",
        parser->filepath, line);
    print_err_line(&line_ptr, line, line - 1);
    print_err_line(&line_ptr, line, line);
    print_err_line(&line_ptr, line, line + 1);
    dprintf(2, "===\n");
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