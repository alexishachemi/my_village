#include "csp.h"
#include "registry.h"
#include "str.h"

bool csp_room_init(csp_room_t *room, const char *name)
{
    if (!room || !name)
        return false;
    namecpy(room->name, name);
    return reg_init(&room->objs, sizeof(csp_object_t), CSP_ROOM_OBJ_SIZE);
}

void csp_room_deinit(csp_room_t *room)
{
    if (!room)
        return;
    reg_map(&room->objs, (reg_callback_t)csp_obj_deinit);
    reg_deinit(&room->objs);
}

csp_object_t *csp_room_add_obj(csp_room_t *room)
{
    csp_object_t *obj = NULL;

    if (!room)
        return NULL;
    obj = reg_new_elem(&room->objs);
    if (!csp_obj_init(obj))
        return NULL;
    return obj;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(room, init)
{
    csp_room_t room = {0};
    const char *name = "my_room";

    cr_assert(csp_room_init(&room, name));
    cr_assert_str_eq(room.name, name);
    cr_assert_eq(REG_SIZE(room.objs), 0);

    csp_room_deinit(&room);
}

Test(room, add)
{
    csp_room_t room = {0};

    cr_assert(csp_room_init(&room, "foo"));
    cr_assert_not_null(csp_room_add_obj(&room));
    cr_assert_eq(REG_SIZE(room.objs), 1);
    cr_assert_not_null(csp_room_add_obj(&room));
    cr_assert_eq(REG_SIZE(room.objs), 2);
    csp_room_deinit(&room);
}

#endif
