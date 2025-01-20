#include "csp.h"
#include "prop.h"
#include "registry.h"

csp_object_t *csp_map_add_obj(csp_map_t *map)
{
    csp_object_t *obj = NULL;

    if (!map)
        return NULL;
    obj = reg_new_elem(&map->objs);
    if (!csp_obj_init(obj))
        return NULL;
    return obj;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_map, add_object)
{
    csp_map_t map = {0};
    csp_object_t *obj = NULL;

    cr_assert(csp_map_init(&map, (v2_t){5, 5}, 3));
    cr_assert_not_null((obj = csp_map_add_obj(&map)));
    cr_assert_eq(obj->chance, 1.0);

    csp_map_deinit(&map);
}

#endif