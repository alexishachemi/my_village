#include <string.h>
#include "draw_queue.h"
#include "raylib.h"
#include "registry.h"
#include "render.h"
#include "texture.h"

bool renderer_init(renderer_t *renderer, const display_settings_t *settings)
{
    if (!renderer || !settings)
        return false;
    renderer->camera = (Camera2D){{0, 0}, {0 , 0}, 0, 1.0};
    memcpy(&renderer->settings, settings, sizeof(display_settings_t));
    memset(&renderer->debug, 0, sizeof(display_debug_setting_t));
    draw_queue_init(&renderer->draw_queue);
    return reg_init(&renderer->textures, sizeof(texture_t), TEXTURE_REGISTRY_BASE_SIZE);
}

void renderer_deinit(renderer_t *renderer)
{
    if (!renderer)
        return;
    draw_queue_deinit(&renderer->draw_queue);
    reg_deinit(&renderer->textures);
}

texture_t *renderer_new_texture(renderer_t *renderer,
    const char *name, const char *path)
{
    texture_t *texture = NULL;

    if (!renderer || !path)
        return false;
    texture = reg_new_elem(&renderer->textures);
    if (!texture || !texture_init(texture, name, path))
        return NULL;
    return texture;
}

bool renderer_load(renderer_t *renderer)
{
    if (!renderer)
        return false;
    for (size_t i = 0; i < REG_SIZE(renderer->textures); i++) {
        if (!texture_load(REG_AT(texture_t, &renderer->textures, i)))
            return false;
    }
    return true;
}

void renderer_unload(renderer_t *renderer)
{
    if (!renderer)
        return;
    reg_map(&renderer->textures, (reg_callback_t)texture_unload);
}