#include "raylib.h"
#include "render.h"
#include <stdbool.h>

static bool load_assets(renderer_t *renderer, world_t *world)
{
    asset_t *asset = NULL;

    for (size_t i = 0; i < world->asset_reg.last_free_index; i++) {
        asset = world_get_asset_ptr(world, i);
        if (!asset || !renderer_add_texture(renderer, asset->path))
            return false;
    }
    return true;
}

static bool init(renderer_t *renderer, world_t *world,
    const display_settings_t *settings)
{
    if (!renderer || !world || !settings)
        return false;
    renderer->world = world;
    memcpy(&renderer->settings, settings, sizeof(display_settings_t));
    return reg_init(&renderer->textures, sizeof(Texture2D),
        TEXTURE_REGISTRY_BASE_SIZE) && load_assets(renderer, world);
}

static void deinit_texture(void *texture)
{
    UnloadTexture(*(Texture2D*)texture);
}

static void deinit(renderer_t *renderer)
{
    if (!renderer)
        return;
    reg_map(&renderer->textures, deinit_texture);
    reg_deinit(&renderer->textures);
}

bool render(world_t *world, const display_settings_t *settings)
{
    renderer_t renderer = {0};
    bool status = false;

    if (!world || !settings)
        return false;
    InitWindow(
        settings->screen_width,
        settings->screen_height,
        "MyVillage"
    );
    if (!init(&renderer, world, settings))
        return false;
    status = renderer_display(&renderer);
    deinit(&renderer);
    CloseWindow();
    return status;
}