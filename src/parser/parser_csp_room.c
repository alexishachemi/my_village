#include "cJSON.h"
#include "csp.h"
#include "parser.h"
#include "world.h"

bool parse_csp_room(parser_t *parser, const char *name, cJSON *item, csp_room_t *buf)
{
    cJSON *curr = NULL;
    csp_object_t *obj = NULL;

    if (!parser || !buf)
        return false;
    if (!item)
        return parser_raise_missing_value(parser, name, "Object");
    if (!cJSON_IsObject(item))
        return parser_raise_invalid_type(parser, name, item, "Object");
    if (!parse_terrain(parser, cJSON_GetObjectItem(item, "terrain"), name, &buf->terrain))
        return false;
    if (!parse_non_zero_positive_int(parser, name, cJSON_GetObjectItem(item, "layers"), (int*)&buf->layers))
        return false;
    if (cJSON_HasObjectItem(item, "constraints") && !parse_csp_global_constraints(parser,
            name, cJSON_GetObjectItem(item, "constraints"), buf))
        return false;
    cJSON_ArrayForEach(curr, cJSON_GetObjectItem(item, "objects")) {
        obj = csp_room_add_obj(buf);
        if (!obj)
            return parser_raise_error(parser, "Failed to create csp object");
        if (!parse_csp_obj(parser, curr, obj))
            return false;
    }
    return true;
}

static bool parse_new_csp_room(parser_t *parser, cJSON *item)
{
    csp_room_t *room = NULL;

    room = world_new_room(parser->world, item->string);
    if (!room)
        return parser_raise_error(parser, "Failed to create room %s", item->string);
    return parse_csp_room(parser, item->string, item, room);
}

bool parse_csp_rooms(parser_t *parser)
{
    PARSE_OPTIONAL_ENTER(parser, "rooms");
    for (cJSON *curr = parser->current->child; curr; curr = curr->next) {
        if (!parse_new_csp_room(parser, curr))
            return false;
    }
    PARSE_EXIT(parser);
    return true;
}
