#include "orientation.h"
#include "prop.h"
#include "v2.h"
#ifndef TEST
#define MAIN main
#else
#define MAIN _main
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "world.h"
#include "render.h"

static void setup_debug_map(renderer_t *r, world_t *world)
{
    texture_t *grass_texture = renderer_new_texture(r, "grass", "tests/assets/test_terrain.png");
    texture_t *tree_texture = renderer_new_texture(r, "tree", "tests/assets/test_prop.png");
    texture_t *sofa_texture = renderer_new_texture(r, "sofa", "tests/assets/test_sofa.png");

    asset_t *grass_asset = world_new_asset(world, "grass", grass_texture, (Rectangle){0, 0, 32, 32});
    asset_t *tree_asset = world_new_asset(world, "tree", tree_texture, (Rectangle){0, 0, 32, 48});
    asset_t *big_tree_asset = world_new_asset(world, "big_tree", tree_texture, (Rectangle){32, 0, 64, 96});
    asset_t *left_sofa_asset = world_new_asset(world, "lsofa", sofa_texture, (Rectangle){0, 0, 32, 42});
    asset_t *right_sofa_asset = world_new_asset(world, "rsofa", sofa_texture, (Rectangle){32, 0, 32, 42});

    terrain_t *grass_terrain = world_new_terrain(world, "grass", grass_asset);
    prop_t *tree_prop = world_new_prop(world, "tree");
    prop_t *big_tree_prop = world_new_prop(world, "big_tree");
    prop_t *sofa_prop = world_new_prop(world, "sofa");
    prop_t *sofa_prop_r = prop_add_child(sofa_prop, (v2_t){0, 1});

    prop_set_mono_asset(tree_prop, tree_asset);
    prop_set_mono_asset(big_tree_prop, big_tree_asset);
    prop_set_mono_asset(sofa_prop, left_sofa_asset);
    prop_set_mono_asset(sofa_prop_r, right_sofa_asset);
    prop_set_z_index(big_tree_prop, 1);
    prop_set_z_index(tree_prop, 0);

    srand(time(NULL));
    for (size_t y = 0; y < world->size; y++) {
        for (size_t x = 0; x < world->size; x++) {
            world_place_terrain(world, (v2_t){x, y}, grass_terrain);
            if (rand() % 5 == 0)
                world_place_prop(world, sofa_prop, (v2_t){x, y}, ORIENT_RIGHT, false);
            else if (rand() % 5 == 0)
                world_place_prop(world, big_tree_prop, (v2_t){x, y}, ORIENT_RIGHT, false);
            else if (rand() % 7 == 0)
                world_place_prop(world, tree_prop, (v2_t){x, y}, ORIENT_RIGHT, false);
        
        }
    }
}

int MAIN(void)
{
    world_t world = {0};
    renderer_t renderer = {0};

    world_init(&world, 100, 10);
    renderer_init(&renderer, &(display_settings_t){800, 600, 32});
    setup_debug_map(&renderer, &world);
    render(&renderer, &world);
    renderer_deinit(&renderer);
    world_deinit(&world);
    return 0;
}
