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
#include "orientation.h"
#include "prop.h"
#include "v2.h"
#include "world.h"
#include "render.h"
#include "bsp.h"
#include "parser.h"
#include "rect.h"

static void setup_debug_map(world_t *world)
{
    for (size_t y = 0; y < world->size; y++) {
        for (size_t x = 0; x < world->size; x++) {
            world_place_terrain(world, (v2_t){x, y}, world_get_terrain(world, "grass"));
            if (rand() % 5 == 0)
                world_place_prop(world, world_get_prop(world, "big_tree"), (v2_t){x, y}, ORIENT_DOWN, false);
            else if (rand() % 7 == 0)
                world_place_prop(world, world_get_prop(world, "small_tree"), (v2_t){x, y}, ORIENT_LEFT, false);
        }
    }
}

static void setup_room(world_t *world, rect_t rect)
{
    csp_map_t map = {0};
    unsigned int map_layers = 3;
    v2_t map_size = {rect.w, rect.h};
    csp_object_t *obj = NULL;

    csp_map_init(&map, map_size, map_layers);
    map.floor = world_get_terrain(world, "floor");

    obj = csp_map_add_obj(&map, world_get_prop(world, "bed"));
    csp_set_on_ground(obj);
    csp_set_reserved_space(obj, (v2_t){1, 0});
    csp_set_reserved_space(obj, (v2_t){0, 1});
    csp_set_reserved_space(obj, (v2_t){1, 1});
    csp_set_adjacent_to_wall(obj);
    csp_set_has_orient(obj, ORIENT_DOWN);
    csp_set_amount(obj, 1);

    obj = csp_map_add_obj(&map, world_get_prop(world, "painting"));
    csp_set_has_orient(obj, ORIENT_DOWN);
    csp_set_on_top_of_prop(obj, world_get_prop(world, "bed"));
    csp_set_amount(obj, 1);

    obj = csp_map_add_obj(&map, world_get_prop(world, "sofa"));
    csp_set_on_ground(obj);
    csp_set_reserved_space(obj, (v2_t){1, 0});
    // csp_set_adjacent_to_wall(obj);
    csp_set_amount_range(obj, 1, 3);
    csp_set_has_orient(obj, ORIENT_RIGHT);
    csp_set_has_orient(obj, ORIENT_LEFT);
    csp_set_has_orient(obj, ORIENT_DOWN);

    if (!csp_map_generate(&map))
        printf("-- Room generation failed --\n");
    // csp_place_obj(&map, obj, (v2_t){0, 0}, 0, ORIENT_DOWN);
    // csp_map_print(&map);
    csp_map_apply(&map, world, (v2_t){rect.x, rect.y});
    csp_map_deinit(&map);
}

void setup_rooms(world_t *world, bsp_t *bsp)
{
    if (!world || !bsp)
        return;
    if (!bsp_is_leaf(bsp)) {
        setup_rooms(world, bsp->sub1);
        setup_rooms(world, bsp->sub2);
        return;
    }
    setup_room(world, (rect_t){
        bsp->room.pos.x,
        bsp->room.pos.y,
        bsp->room.size.x,
        bsp->room.size.y
    });
}

// int MAIN(void)
// {
//     world_t world = {0};
//     renderer_t renderer = {0};
//     bsp_t bsp = {0};

//     if (!world_init(&world, 50, 0)) {
//         dprintf(2, "ERROR: Failed to initialize world\n");
//         return EXIT_FAILURE;
//     }
//     renderer_init(&renderer, &(display_settings_t){
//         .screen_width=800,
//         .screen_height=600,
//         .tile_size_px=32
//     });
//     setup_debug_map(&renderer, &world);
//     setup_room(&world);
//     render_and_display(&renderer, &world);
//     renderer_deinit(&renderer);
//     world_deinit(&world);
//     return EXIT_SUCCESS;
// }

// #include <stdio.h>
// #include "parser.h"

// int MAIN(void)
// {
//     world_t world = {0};
//     renderer_t renderer = {0};
//     unsigned int seed = time(NULL);

//     if (!parse_config(&world, &renderer, "test.json"))
//         return EXIT_FAILURE;
//     renderer_print(&renderer);
//     printf("\n");
//     world_print(&world);
//     world_deinit(&world);
//     renderer_deinit(&renderer);
//     return EXIT_SUCCESS;
// }

int MAIN(void)
{
    world_t world = {0};
    renderer_t renderer = {0};
    bsp_t bsp = {0};

    srand(time(NULL));
    if (!parse_config(&world, &renderer, "test.json")) {
        dprintf(2, "Failed to parse config\n");
        return EXIT_FAILURE;
    }
    setup_debug_map(&world);
    if (!bsp_init(&bsp, (bsp_rect_t){{0,0}, {world.size, world.size}})) {
        dprintf(2, "Failed to initialize bsp\n");
        return EXIT_FAILURE;
    }
    bsp.s_settings = (bsp_split_settings_t) {
        .splits = 3,
        .max_ratio = 2.7,
        .same_split_percent = 0
    };
    bsp.r_settings = (bsp_room_settings_t){
        .max_ratio = 1.3,
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
    setup_rooms(&world, &bsp);
    render_and_display(&renderer, &world);
    world_deinit(&world);
    renderer_deinit(&renderer);
    return EXIT_SUCCESS;
}