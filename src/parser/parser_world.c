#include "cJSON.h"
#include "parser.h"

bool parse_world(parser_t *parser)
{
    PARSE_ENTER(parser, "world");
    int size = 0;
    int chunk_size = 0;

    if (!parse_positive_int(parser, "size",
        cJSON_GetObjectItem(parser->current, "size"), &size))
        return false;
    if (!parse_positive_int(parser, "chunk_size",
        cJSON_GetObjectItem(parser->current, "chunk_size"), &chunk_size))
        return false;
    PARSE_EXIT(parser);
    return world_init(parser->world, size, chunk_size);
}