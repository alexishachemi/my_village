#include "csp.h"
#include "prop.h"
#include "registry.h"

csp_object_t *csp_map_add_obj(csp_map_t *map, prop_t *prop)
{
    csp_object_t *obj = NULL;

    if (!map || !prop)
        return NULL;
    obj = reg_new_elem(&map->objs);
    if (!csp_obj_init(obj, prop))
        return NULL;
    return obj;
}

csp_collection_t *csp_map_add_collection(csp_map_t *map)
{
    csp_collection_t *collection = NULL;

    if (!map)
        return NULL;
    collection = reg_new_elem(&map->objs);
    if (!csp_collection_init(collection))
        return false;
    return collection;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_map, add_object)
{
    csp_map_t map = {0};
    csp_object_t *obj = NULL;
    prop_t prop = {0};

    cr_assert(csp_map_init(&map, (v2_t){5, 5}, 3));
    cr_assert_not_null((obj = csp_map_add_obj(&map, &prop)));
    cr_assert_not(obj->is_collection);

    csp_map_deinit(&map);
}

Test(csp_map, add_collection)
{
    csp_map_t map = {0};
    csp_collection_t *collection = NULL;

    cr_assert(csp_map_init(&map, (v2_t){5, 5}, 3));
    cr_assert_not_null((collection = csp_map_add_collection(&map)));
    cr_assert(collection->is_collection);

    csp_map_deinit(&map);
}

#endif