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
    texture_t *terrain_texture = renderer_new_texture(r, "terrain", "tests/assets/test_terrain.png");
    texture_t *prop_texture = renderer_new_texture(r, "prop", "tests/assets/test_prop.png");
    asset_t *terrain_asset = world_new_asset(world, "terrain", terrain_texture, (Rectangle){0, 0, 32, 32});
    asset_t *prop_asset = world_new_asset(world, "prop", prop_texture, (Rectangle){0, 0, 48, 64});
    terrain_t *terrain = world_new_terrain(world, "terrain", terrain_asset);
    prop_t *prop = world_new_prop(world, "prop");

    srand(time(NULL));
    prop_set_mono_asset(prop, prop_asset);
    for (size_t y = 0; y < world->size; y++) {
        for (size_t x = 0; x < world->size; x++) {
            tile_t *tile = world_get_tile(world, x, y);
            if (!tile)
                return;
            tile->terrain = terrain;
            if (rand() % 10 == 0)
                tile->prop = prop;
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
