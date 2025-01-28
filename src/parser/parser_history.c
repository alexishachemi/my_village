#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "parser.h"
#include "registry.h"

static bool push_history(parser_t *parser, const char *name, cJSON *item)
{
    parser_history_t *new = reg_new_elem(&parser->history);

    if (!new)
        return false;
    strncpy(new->name, name, PARSER_HISTORY_NAME_SIZE);
    new->item = item;
    return true;
}

bool parser_enter(parser_t *parser, const char *name, bool is_obj)
{
    if (!parser || !name)
        return false;
    parser->current = cJSON_GetObjectItem(parser->current, name);
    if (!parser->current)
        return parser_raise_missing_value(parser, name, "Object");
    if (is_obj) {
        if (!cJSON_IsObject(parser->current))
            return parser_raise_invalid_type(parser, name, parser->current, "Object");
    }
    return push_history(parser, name, parser->current);
}

bool parser_exit(parser_t *parser)
{
    if (!parser)
        return false;
    if (REG_SIZE(parser->history) == 0)
        return true;
    memset(
        REG_AT(parser_history_t, &parser->history, parser->history.last_free_index),
        0, parser->history.vec.elem_size
    );
    reg_pop_back(&parser->history, NULL);
    if (REG_SIZE(parser->history) != 0) {
        parser->current = REG_AT(parser_history_t, &parser->history,
            parser->history.last_free_index - 1)->item;
    } else {
        parser->current = parser->root;
    }
    return true;
}

static size_t get_size(reg_t *path)
{
    size_t size = 0;

    if (path->last_free_index == 0)
        return 0;
    for (unsigned int i = 0; i < path->last_free_index; i++) {
        size += strlen((REG_AT(parser_history_t, path, i))->name) + 1;
    }
    return size;
}

char *parser_get_path(parser_t *parser)
{
    size_t size = 3;
    char *path_str = NULL;

    if (!parser)
        return NULL;
    size += strlen(parser->filepath) + get_size(&parser->history);
    path_str = malloc(sizeof(char) * size);
    if (!path_str)
        return NULL;
    memset(path_str, 0, sizeof(char) * size);
    snprintf(path_str, size, "%s:/", parser->filepath);
    for (unsigned int i = 0; i < parser->history.last_free_index; i++) {
        strcat(path_str, REG_AT(parser_history_t, &parser->history, i)->name);
        if (i != parser->history.last_free_index - 1)
            strcat(path_str, "/");
    }
    return path_str;
}
