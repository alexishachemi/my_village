#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "raylib.h"
#include "registry.h"

bool parser_init(parser_t *parser)
{
    if (!parser)
        return false;
    memset(parser, 0, sizeof(parser_t));
    return reg_init(&parser->history, sizeof(parser_history_t), PARSER_HISTORY_BASE_SIZE);
}

void parser_deinit(parser_t *parser)
{
    if (!parser)
        return;
    reg_deinit(&parser->history);
    if (parser->filepath) {
        free(parser->filepath);
        parser->filepath = NULL;
    }
    if (parser->raw) {
        UnloadFileText(parser->raw);
        parser->raw = NULL;
    }
    if (parser->root) {
        cJSON_Delete(parser->root);
        parser->root = NULL;
    }
    parser->world = NULL;
}
