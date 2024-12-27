#include "cJSON.h"
#include "parser.h"
#include "v2.h"

bool parse_int(parser_t *parser, const char *name, cJSON *item, int *buf)
{
    if (!item)
        parser_raise_missing_value(parser, name, "Number");
    if (!parser || !item || !buf)
        return false;
    if (!cJSON_IsNumber(item))
        return parser_raise_invalid_type(parser, item->string, item, "Number");
    *buf = (int)cJSON_GetNumberValue(item);
    return true;
}

bool parse_positive_int(parser_t *parser, const char *name, cJSON *item, int *buf)
{
    if (!item)
        parser_raise_missing_value(parser, name, "Number (Positive)");
    if (!parser || !item || !buf)
        return false;
    if (!cJSON_IsNumber(item))
        return parser_raise_invalid_type(parser, item->string, item, "Number (Positive)");
    *buf = (int)cJSON_GetNumberValue(item);
    if (*buf < 0)
       return parser_raise_invalid_value(parser, name, "Number (Negative)", "Number (Positive)"); 
    return true;
}

bool parse_v2(parser_t *parser, const char *name, cJSON *item, v2_t *buf)
{
    if (!item)
        parser_raise_missing_value(parser, name, "Vector2");
    if (!item || !parser || !buf)
        return false;
    if (!cJSON_IsArray(item) || cJSON_GetArraySize(item) != 2)
        return parser_raise_invalid_type(parser, item->string, item, "Vector2");
    return parse_int(parser, name, cJSON_GetArrayItem(item, 0), &buf->x)
        && parse_int(parser, name, cJSON_GetArrayItem(item, 1), &buf->y);
}

bool parse_positive_v2(parser_t *parser, const char *name, cJSON *item, v2_t *buf)
{
    if (!item)
        parser_raise_missing_value(parser, name, "Vector2 (Positive)");
    if (!item || !parser || !buf)
        return false;
    if (!cJSON_IsArray(item) || cJSON_GetArraySize(item) != 2)
        return parser_raise_invalid_type(parser, item->string, item, "Vector2");
    return parse_positive_int(parser, name, cJSON_GetArrayItem(item, 0), &buf->x)
        && parse_positive_int(parser, name, cJSON_GetArrayItem(item, 1), &buf->y);
}