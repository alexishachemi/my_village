#include "texture.h"
#include "str.h"

bool texture_init(texture_t *texture, const char *name, const char *path)
{
    if (!texture || !path)
        return false;
    namecpy(texture->name, name ? name : path_find_filename(path));
    pathcpy(texture->path, path);
    return true;
}

bool texture_load(texture_t *texture)
{
    if (!texture)
        return false;
    texture->rtexture = LoadTexture(texture->path);
    return texture->rtexture.id != 0;
}

void texture_unload(texture_t *texture)
{
    if (!texture)
        return;
    UnloadTexture(texture->rtexture);
}