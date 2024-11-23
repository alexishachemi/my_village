#pragma once

#include "world.h"
#include "raylib.h"
#include "registry.h"

#define TEXTURE_REGISTRY_BASE_SIZE 128
#define BG_COLOR BLACK

typedef struct {
    unsigned int screen_width;
    unsigned int screen_height;
    size_t tile_size_px;
} display_settings_t;

typedef struct {
    world_t *world;
    reg_t textures; // Texture2D
    Camera2D camera;
    display_settings_t settings;
} renderer_t;

bool render(world_t *world, const display_settings_t *settings);
bool renderer_add_texture(renderer_t *renderer, const char *path);
bool renderer_display(renderer_t *renderer);