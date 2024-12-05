#include "csp.h"
#include "registry.h"

bool csp_collection_init(csp_collection_t *collection)
{
    if (!collection)
        return false;
    collection->is_collection = true;
    return reg_init(&collection->objs, sizeof(csp_object_t), CSP_COLLECTION_OBJ_SIZE);
}

void csp_collection_deinit(csp_collection_t *collection)
{
    if (!collection)
        return;
    reg_deinit(&collection->objs);
}

csp_object_t *csp_collection_add_obj(csp_collection_t *collection, prop_t *prop)
{
    csp_object_t *obj = NULL;

    if (!collection)
        return NULL;
    obj = reg_new_elem(&collection->objs);
    if (!csp_obj_init(obj, prop))
        return NULL;
    return obj;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_collection, init)
{
    csp_collection_t collection = {0};

    cr_assert(csp_collection_init(&collection));
    cr_assert_eq(collection.objs.vec.size, CSP_COLLECTION_OBJ_SIZE);
    cr_assert_eq(REG_SIZE(collection.objs), 0);
    cr_assert(collection.is_collection);
    csp_collection_deinit(&collection);
}

Test(csp_collection, add)
{
    csp_collection_t collection = {0};
    csp_object_t *obj = NULL;
    prop_t prop = {0};

    cr_assert(csp_collection_init(&collection));
    cr_assert_eq(collection.objs.vec.size, CSP_COLLECTION_OBJ_SIZE);
    cr_assert_eq(REG_SIZE(collection.objs), 0);
    cr_assert(collection.is_collection);
    obj = csp_collection_add_obj(&collection, &prop);
    cr_assert_eq(REG_SIZE(collection.objs), 1);
    cr_assert_not_null(obj);
    csp_collection_deinit(&collection);
}

#endif