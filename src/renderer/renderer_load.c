#include "raylib.h"
#include "registry.h"
#include "render.h"

bool renderer_add_texture(renderer_t *renderer, const char *path)
{
    Texture2D *texture = NULL;

    if (!renderer || !path)
        return false;
    texture = reg_new_elem(&renderer->textures);
    if (!texture)
        return false;
    *texture = LoadTexture(path);
    return texture->id != 0;
}