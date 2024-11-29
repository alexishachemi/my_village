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

    asset_t *grass_asset = world_new_asset(world, "grass", grass_texture, (Rectangle){0, 0, 32, 32});
    asset_t *tree_asset = world_new_asset(world, "tree", tree_texture, (Rectangle){0, 0, 32, 48});
    asset_t *big_tree_asset = world_new_asset(world, "big_tree", tree_texture, (Rectangle){32, 0, 64, 96});

    terrain_t *grass_terrain = world_new_terrain(world, "grass", grass_asset);
    prop_t *tree_prop = world_new_prop(world, "tree");
    prop_t *big_tree_prop = world_new_prop(world, "big_tree");

    srand(time(NULL));
    prop_set_mono_asset(tree_prop, tree_asset);
    prop_set_mono_asset(big_tree_prop, big_tree_asset);
    for (size_t y = 0; y < world->size; y++) {
        for (size_t x = 0; x < world->size; x++) {
            tile_t *tile = world_get_tile(world, x, y);
            if (!tile)
                return;
            tile->terrain = grass_terrain;
            if (rand() % 10 == 0)
                tile->prop = big_tree_prop;
        }
    }
}

int MAIN(void)
{
    world_t world = {0};
    renderer_t renderer = {0};

    world_init(&world, 30, 3);
    renderer_init(&renderer, &(display_settings_t){800, 600, 32});
    setup_debug_map(&renderer, &world);
    render_and_display(&renderer, &world);
    renderer_deinit(&renderer);
    world_deinit(&world);
    return 0;
}
