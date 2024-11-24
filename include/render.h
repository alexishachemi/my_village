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
    reg_t textures; // texture_t
    Camera2D camera;
    display_settings_t settings;
} renderer_t;

bool renderer_init(renderer_t *renderer, const display_settings_t *settings);
void renderer_deinit(renderer_t *renderer);
bool renderer_load(renderer_t *renderer);
void renderer_unload(renderer_t *renderer);

texture_t *renderer_new_texture(renderer_t *renderer,
    const char *name, const char *path);

bool renderer_display(renderer_t *renderer, world_t *world);
bool render(renderer_t *renderer, world_t *world);