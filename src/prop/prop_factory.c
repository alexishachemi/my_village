#include "prop.h"
#include "registry.h"
#include <string.h>

bool prop_init(prop_t *prop, const char *name)
{
    if (!prop || !name)
        return false;
    memset(prop, 0, sizeof(prop_t));
    prop->type = PTYPE_PARENT;
    namecpy(prop->name, name);
    prop->asset_map.asset_mode = AMODE_NONE;
    prop->z_index = 0;
    return true;
}

void prop_deinit(prop_t *prop)
{
    if (!prop || prop->type != PTYPE_PARENT || !prop->has_child)
        return;
    reg_deinit(&prop->childs);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(prop, init)
{
    prop_t prop = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert_str_eq(prop.name, "name");
    cr_assert_eq(prop.asset_map.asset_mode, AMODE_NONE);
    cr_assert_eq(prop.z_index, 0);
}

#endif
