#include "cJSON.h"
#include "orientation.h"
#include "parser.h"
#include "v2.h"
#include <strings.h>

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

bool parse_non_zero_positive_int(parser_t *parser, const char *name, cJSON *item, int *buf)
{
    if (!parse_positive_int(parser, name, item, buf))
        return false;
    if (*buf == 0)
       return parser_raise_invalid_value(parser, name, "Number (Zero)", "Number (Positive, Non Zero)"); 
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

bool parse_non_zero_positive_v2(parser_t *parser, const char *name, cJSON *item, v2_t *buf)
{
    if (!parse_positive_v2(parser, name, item, buf))
        return false;
    if (buf->x == 0 || buf->y == 0)
       return parser_raise_invalid_value(parser, name, "Vector2 (Zero)", "Vector2 (Positive, Non Zero)"); 
    return true;
}

bool parse_rate(parser_t *parser, const char *name, cJSON *item, float *buf)
{
    if (!item)
        parser_raise_missing_value(parser, name, "Number (0.0 to 1.0)");
    if (!parser || !item || !buf)
        return false;
    if (!cJSON_IsNumber(item))
        return parser_raise_invalid_type(parser, item->string, item, "Number (0.0 to 1.0)");
    *buf = (float)cJSON_GetNumberValue(item);
    if (*buf < 0.0 || *buf > 1.0)
        return parser_raise_invalid_value(parser, name, "Number", "Number (0.0 to 1.0)");
    return true;
}

bool parser_get_csp_arg(parser_t *parser, cJSON *args, unsigned int index, cJSON **buf)
{
    if (!buf || !parser)
        return false;
    for (unsigned int i = 0; args && i < index; i++) {
        args = args->next;
    }
    if (!args)
        return parser_raise_error(parser, "Failed to get csp argument %ld", index);
    *buf = args;
    return true;
}

bool parse_orient(parser_t *parser, const char *name, cJSON *item, orient_t *buf)
{
    const char *orient_str = NULL;

    if (!parser || !buf)
        return false;
    if (!item)
        return parser_raise_missing_value(parser, name, "String");
    if (!cJSON_IsString(item))
        return parser_raise_invalid_type(parser, name, item, "String");
    orient_str = cJSON_GetStringValue(item);
    if (strcasecmp(orient_str, "UP")) {
        *buf = ORIENT_UP;
        return true;
    }
    if (strcasecmp(orient_str, "DOWN")) {
        *buf = ORIENT_DOWN;
        return true;
    }
    if (strcasecmp(orient_str, "LEFT")) {
        *buf = ORIENT_LEFT;
        return true;
    }
    if (strcasecmp(orient_str, "RIGHT")) {
        *buf = ORIENT_RIGHT;
        return true;
    }
    return parser_raise_invalid_value(parser, name, "String", "String (Orientation)");
}
