#include "cJSON.h"
#include "csp.h"
#include "parser.h"
#include "world.h"

static bool add_prop(parser_t *parser, cJSON *item, csp_object_t *obj)
{
    prop_t *prop = NULL;
    cJSON *curr = NULL;

    if (!item)
        return parser_raise_missing_value(parser, "prop", "String | Array");
    if (cJSON_IsString(item)) {
        prop = world_get_prop(parser->world, cJSON_GetStringValue(item));
        if (!prop)
            return parser_raise_error(parser, "Failed to locate prop %s", cJSON_GetStringValue(item));
        if (!csp_obj_add_prop(obj, prop))
            return parser_raise_error(parser, "Failed to set obj prop %s", cJSON_GetStringValue(item));
        return true;
    }
    if (!cJSON_IsArray(item))
        return parser_raise_invalid_type(parser, "prop", item, "String | Array");
    cJSON_ArrayForEach(curr, item) {
        if (!cJSON_IsString(curr))
            return parser_raise_invalid_type(parser, "prop", curr, "String");
        prop = world_get_prop(parser->world, cJSON_GetStringValue(curr));
        if (!prop)
            return parser_raise_error(parser, "Failed to locate prop %s", cJSON_GetStringValue(curr));
        if (!csp_obj_add_prop(obj, prop))
            return parser_raise_error(parser, "Failed to add obj prop %s", cJSON_GetStringValue(curr));
    }
    return true;
}

static bool parse_amount(parser_t *parser, cJSON *item, csp_object_t *obj)
{
    int nb = 0;
    v2_t v = {0};

    if (!item)
        return true;
    if (cJSON_IsNumber(item)) {
        if (!parse_non_zero_positive_int(parser, "amount", item, &nb))
            return false;
        if (!csp_set_amount(obj, nb))
            return parser_raise_error(parser, "Failed to set csp obj amount");
        return true;
    }
    if (!cJSON_IsArray(item))
        return parser_raise_invalid_type(parser, "amount", item, "Int (Positive, Non Zero) | Array");
    if (!parse_non_zero_positive_v2(parser, "amount", item, &v))
        return false;
    if (!csp_set_amount_range(obj, v.x, v.y))
        return parser_raise_error(parser, "Failed to set csp obj amount range");
    return true;
}

bool parse_csp_obj(parser_t *parser, cJSON *item, csp_object_t *buf)
{
    if (!item)
        parser_raise_missing_value(parser, NULL, "Object");
    if (!parser || !buf)
        return false;
    if (!cJSON_IsObject(item))
        return parser_raise_invalid_type(parser, NULL, item, "Object");
    return add_prop(parser, cJSON_GetObjectItem(item, "prop"), buf)
        && parse_rate(parser, "chance", cJSON_GetObjectItem(item, "chance"), &buf->chance)
        && parse_amount(parser, cJSON_GetObjectItem(item, "amount"), buf)
        && parse_csp_local_constraints(parser, NULL, cJSON_GetObjectItem(item, "constraints"), buf);
}
