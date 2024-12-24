#ifndef TEST
#define MAIN main
#else
#define MAIN _main
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "csp.h"
#include "chunk.h"
#include "registry.h"
#include "orientation.h"
#include "prop.h"
#include "v2.h"
#include "world.h"
#include "render.h"

static void setup_debug_map(renderer_t *r, world_t *world)
{
    texture_t *grass_texture = renderer_new_texture(r, "grass", "tests/assets/test_terrain.png");
    texture_t *tree_texture = renderer_new_texture(r, "tree", "tests/assets/test_prop.png");
    texture_t *sofa_texture = renderer_new_texture(r, "sofa", "tests/assets/test_sofa.png");
    texture_t *bed_texture = renderer_new_texture(r, "bed", "tests/assets/bed.png");
    texture_t *painting_texture = renderer_new_texture(r, "painting", "tests/assets/painting.png");
    texture_t *floor_texture = renderer_new_texture(r, "floor", "tests/assets/floor.png");

    asset_t *grass_asset = world_new_asset(world, "grass", grass_texture, (Rectangle){0, 0, 32, 32});
    asset_t *tree_asset = world_new_asset(world, "tree", tree_texture, (Rectangle){0, 0, 32, 48});
    asset_t *big_tree_asset = world_new_asset(world, "big_tree", tree_texture, (Rectangle){32, 0, 64, 96});
    
    asset_t *left_sofa_down_asset = world_new_asset(world, "l_sofa_d", sofa_texture, (Rectangle){0, 0, 32, 42});
    asset_t *right_sofa_down_asset = world_new_asset(world, "r_sofa_d", sofa_texture, (Rectangle){32, 0, 32, 42});

    asset_t *left_sofa_right_asset = world_new_asset(world, "l_sofa_r", sofa_texture, (Rectangle){0, 42, 32, 44});
    asset_t *right_sofa_right_asset = world_new_asset(world, "r_sofa_r", sofa_texture, (Rectangle){0, 86, 32, 32});

    asset_t *left_sofa_left_asset = world_new_asset(world, "l_sofa_l", sofa_texture, (Rectangle){32, 86, 32, 32});
    asset_t *right_sofa_left_asset = world_new_asset(world, "r_sofa_l", sofa_texture, (Rectangle){32, 42, 32, 44});
    

    asset_t *tl_bed_asset = world_new_asset(world, "bed", bed_texture, (Rectangle){0, 0, 32, 32});
    asset_t *tr_bed_asset = world_new_asset(world, "tr_bed", bed_texture, (Rectangle){32, 0, 32, 32});
    asset_t *bl_bed_asset = world_new_asset(world, "bl_bed", bed_texture, (Rectangle){0, 32, 32, 32});
    asset_t *br_bed_asset = world_new_asset(world, "br_bed", bed_texture, (Rectangle){32, 32, 32, 32});
    asset_t *painting_asset = world_new_asset(world, "painting", painting_texture, (Rectangle){0, 0, 32, 32});
    asset_t *floor_asset = world_new_asset(world, "floor", floor_texture, (Rectangle){0, 0, 32, 32});

    terrain_t *grass_terrain = world_new_terrain(world, "grass", grass_asset);
    prop_t *tree_prop = world_new_prop(world, "tree");
    prop_t *big_tree_prop = world_new_prop(world, "big_tree");
    prop_t *sofa_prop = world_new_prop(world, "sofa");
    prop_t *sofa_prop_r = prop_add_child(sofa_prop, (v2_t){1, 0});
    prop_t *tl_bed_prop = world_new_prop(world, "bed");
    prop_t *tr_bed_prop = prop_add_child(tl_bed_prop, (v2_t){1, 0});
    prop_t *bl_bed_prop = prop_add_child(tl_bed_prop, (v2_t){0, 1});
    prop_t *br_bed_prop = prop_add_child(tl_bed_prop, (v2_t){1, 1});
    prop_t *painting_prop = world_new_prop(world, "painting");
    world_new_terrain(world, "floor", floor_asset);

    prop_set_mono_asset(tree_prop, tree_asset);
    prop_set_mono_asset(big_tree_prop, big_tree_asset);

    prop_set_multi_asset(
        sofa_prop,
        left_sofa_down_asset,
        left_sofa_down_asset,
        left_sofa_left_asset,
        left_sofa_right_asset
    );

    prop_set_multi_asset(
        sofa_prop_r,
        right_sofa_down_asset,
        right_sofa_down_asset,
        right_sofa_left_asset,
        right_sofa_right_asset
    );

    prop_set_z_index(big_tree_prop, 1);
    prop_set_z_index(tree_prop, 0);
    prop_set_mono_asset(tl_bed_prop, tl_bed_asset);
    prop_set_mono_asset(tr_bed_prop, tr_bed_asset);
    prop_set_mono_asset(bl_bed_prop, bl_bed_asset);
    prop_set_mono_asset(br_bed_prop, br_bed_asset);
    prop_set_z_index(tl_bed_prop, 0);
    prop_set_mono_asset(painting_prop, painting_asset);
    prop_set_z_index(painting_prop, 1);

    // srand(time(NULL));
    for (size_t y = 0; y < world->size; y++) {
        for (size_t x = 0; x < world->size; x++) {
            world_place_terrain(world, (v2_t){x, y}, grass_terrain);
            // if (rand() % 5 == 0)
            //     world_place_prop(world, sofa_prop, (v2_t){x, y}, ORIENT_DOWN, false);
            // if (rand() % 5 == 0)
                // world_place_prop(world, big_tree_prop, (v2_t){x, y}, ORIENT_DOWN, false);
            // else if (rand() % 7 == 0)
                // world_place_prop(world, tree_prop, (v2_t){x, y}, ORIENT_DOWN, false);
        }
    }
}

static void setup_room(world_t *world)
{
    csp_map_t map = {0};
    v2_t map_size = {5, 5};
    unsigned int map_layers = 3;
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

    obj = csp_map_add_obj(&map, world_get_prop(world, "sofa"));
    csp_set_on_ground(obj);
    csp_set_reserved_space(obj, (v2_t){1, 0});
    csp_set_adjacent_to_wall(obj);
    csp_set_amount(obj, 3);
    csp_set_has_orient(obj, ORIENT_RIGHT);
    csp_set_has_orient(obj, ORIENT_LEFT);
    csp_set_has_orient(obj, ORIENT_DOWN);

    if (!csp_map_generate(&map))
        printf("-- Room generation failed --\n");
    // csp_place_obj(&map, obj, (v2_t){0, 0}, 0, ORIENT_DOWN);
    // csp_map_print(&map);
    csp_map_apply(&map, world, (v2_t){25, 25});
    csp_map_deinit(&map);
}

int MAIN(void)
{
    world_t world = {0};
    renderer_t renderer = {0};

    srand(time(NULL));
    if (!world_init(&world, 50, 0)) {
        dprintf(2, "ERROR: Failed to initialize world\n");
        return EXIT_FAILURE;
    }
    renderer_init(&renderer, &(display_settings_t){
        .screen_width=800,
        .screen_height=600,
        .tile_size_px=32
    });
    setup_debug_map(&renderer, &world);
    setup_room(&world);
    render_and_display(&renderer, &world);
    renderer_deinit(&renderer);
    world_deinit(&world);
    return EXIT_SUCCESS;
}
