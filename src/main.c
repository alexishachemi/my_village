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

static void setup_debug_map(world_t *world)
{
    size_t terrain_asset = world_new_asset(world, "terrain", "tests/assets/test_terrain.png");
    size_t prop_asset = world_new_asset(world, "prop", "tests/assets/test_prop.png");
    size_t terrain_id = world_new_terrain(world, "terrain", terrain_asset);
    size_t prop_id = world_new_prop(world, "prop");
    prop_t *prop = world_get_prop_ptr(world, prop_id);

    srand(time(NULL));
    prop_set_mono_asset(prop, prop_asset);
    for (size_t y = 0; y < world->size; y++) {
        for (size_t x = 0; x < world->size; x++) {
            tile_t *tile = world_get_tile(world, x, y);
            if (!tile)
                return;
            tile->terrain_id = terrain_id;
            if (rand() % 10 == 0)
                tile->prop_id = prop_id;
        }
    }
}

int MAIN(void)
{
    world_t world = {0};

    world_init(&world, 10, 10);
    setup_debug_map(&world);
    render(&world, &(display_settings_t){
        .screen_width = 800,
        .screen_height = 600,
        .tile_size_px = 32
    });
    // reg_map(&world.chunk_reg, (reg_callback_t)chunk_print);
    world_deinit(&world);
    return 0;
}
