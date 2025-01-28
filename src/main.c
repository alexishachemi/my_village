#ifndef TEST
#define MAIN main
#else
#define MAIN _main
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "csp.h"
#include "raylib.h"
#include "world.h"
#include "render.h"
#include "bsp.h"
#include "linked.h"
#include "registry.h"
#include "parser.h"

static bool place_rooms(world_t *world, bsp_t *bsp)
{
    list_t leaves = {0};
    unsigned int idx = 0;
    unsigned int wrapped_idx = 0;
    Rectangle bounds = {0};
    bsp_t *leaf = NULL;

    list_init(&leaves);
    if (!bsp_get_leaves(bsp, &leaves))
        return false;
    for (node_t *n = leaves.head; n; n = n->next, idx++) {
        leaf = n->data;
        bounds = (Rectangle){
            leaf->room.pos.x,
            leaf->room.pos.y,
            leaf->room.size.x,
            leaf->room.size.y
        };
        wrapped_idx = idx % world->rooms.last_free_index;
        if (!world_generate_room(world, REG_AT(csp_room_t, &world->rooms, wrapped_idx)->name, bounds))
            return false;
    }
    list_clear(&leaves, NULL);
    return true;
}

int MAIN(void)
{
    world_t world = {0};
    renderer_t renderer = {0};
    unsigned int seed = time(NULL);
    bsp_t bsp = {0};

    srand(seed);
    SetRandomSeed(seed);
    if (!parse_config(&world, &renderer, "examples/config/interiors.json")) {
        dprintf(2, "Failed to parse config\n");
        return EXIT_FAILURE;
    }
    if (!bsp_init(&bsp, (bsp_rect_t){{0,0}, {world.size, world.size}})) {
        dprintf(2, "Failed to initialize bsp\n");
        return EXIT_FAILURE;
    }
    bsp.s_settings = (bsp_split_settings_t) {
        .splits = world.rooms.last_free_index - 1,
        .max_ratio = 2.7,
        .same_split_percent = 0
    };
    bsp.s_settings.splits = bsp.s_settings.splits <= 0 ? 1 : bsp.s_settings.splits;
    bsp.r_settings = (bsp_room_settings_t){
        .max_ratio = 1.0,
        .min_coverage_percent = 10,
        .max_coverage_percent = 20,
        .spacing_scale = 0.5
    };
    bsp.c_settings = (bsp_corridor_settings_t){
        .room_link_min_touch = 10,
        .cycling_rate = 0.1
    };
    if (!bsp_generate(&bsp)) {
        dprintf(2, "Failed to generate BSP\n");
        return EXIT_FAILURE;
    }
    if (!place_rooms(&world, &bsp)) {
        dprintf(2, "Failed to place rooms\n");
        return EXIT_FAILURE;
    }
    render_and_display(&renderer, &world);
    world_deinit(&world);
    renderer_deinit(&renderer);
    return EXIT_SUCCESS;
}
