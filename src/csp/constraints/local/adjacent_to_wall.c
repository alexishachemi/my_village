#include "csp.h"

bool csp_add_adjacent_to_wall(csp_object_t *obj)
{
    if (!obj || obj->is_collection)
        return false;
    return csp_get_constraint(obj, C_ADJACENT_TO_WALL)
        || csp_add_constraint(obj, C_ADJACENT_TO_WALL);
}