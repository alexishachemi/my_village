#include "csp.h"
#include "registry.h"
#include "world.h"

csp_room_t *world_new_room(world_t *world, const char *name)
{
    csp_room_t *room = NULL;

    if (!world || !name || world_get_room(world, name))
        return false;
    room = reg_new_elem(&world->rooms);
    if (!room || !csp_room_init(room, name))
        return NULL;
    return room;
}

csp_room_t *world_get_room(world_t *world, const char *name)
{
    csp_room_t *room = NULL;

    if (!world || !name)
        return NULL;
    for (size_t i = 0; i < REG_SIZE(world->rooms); i++) {
        room = REG_AT(csp_room_t, &world->rooms, i);
        if (room && STR_EQ(room->name, name))
            return room;
    }
    return NULL;
}

bool world_generate_room(world_t *world, const char *name, Rectangle bounds)
{
    csp_map_t map = {0};
    csp_room_t *room = world_get_room(world, name);
    v2_t pos = {bounds.x, bounds.y};
    v2_t size = {bounds.height, bounds.width};

    if (!room || !csp_map_init(&map, room, size))
        return false;
    if (!csp_map_generate(&map) || !csp_map_apply(&map, world, pos)) {
        csp_map_deinit(&map);
        return false;
    }
    csp_map_deinit(&map);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world_room, new_room)
{
    world_t world = {0};
    csp_room_t *room = NULL;

    cr_assert(world_init(&world, 10, 10));
    room = world_new_room(&world, "name");
    cr_assert_not_null(room);
    cr_assert_str_eq(room->name, "name");
    cr_assert_null(room->terrain);
    world_deinit(&world);
}

Test(world_room, duplicate_room)
{
    world_t world = {0};
    csp_room_t *room = NULL;

    cr_assert(world_init(&world, 10, 10));
    room = world_new_room(&world, "name");
    cr_assert_not_null(room);
    cr_assert_null(world_new_room(&world, "name"));
    world_deinit(&world);
}

Test(world_room, get_room)
{
    world_t world = {0};
    csp_room_t *room = NULL;

    cr_assert(world_init(&world, 10, 10));
    room = world_new_room(&world, "name");
    cr_assert_not_null(room);
    cr_assert_eq(world_get_room(&world, "name"), room);
    world_deinit(&world);
}

Test(world_room, get_room_bad)
{
    world_t world = {0};
    csp_room_t *room = NULL;

    cr_assert(world_init(&world, 10, 10));
    room = world_new_room(&world, "name");
    cr_assert_not_null(room);
    cr_assert_eq(world_get_room(&world, "name"), room);
    cr_assert_null(world_get_room(&world, "other_name"));
    world_deinit(&world);
}

#endif
