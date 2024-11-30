#pragma once

#include "world.h"
#include "raylib.h"
#include "registry.h"
#include "draw_queue.h"

#define TEXTURE_REGISTRY_BASE_SIZE 128
#define BG_COLOR BLACK
#define DEBUG_LINE_BASE_THICKNESS 0.5
#define DEBUG_CHUNK_LINE_COLOR BLUE
#define DEBUG_CHUNK_LINE_THICKNESS (DEBUG_LINE_BASE_THICKNESS + 2.0)
#define DEBUG_TILE_LINE_COLOR PINK
#define DEBUG_TILE_LINE_THICKNESS (DEBUG_LINE_BASE_THICKNESS + 1.0)
#define CAMERA_SCROLL_SPEED 300.0

typedef struct {
    unsigned int screen_width;
    unsigned int screen_height;
    size_t tile_size_px;
} display_settings_t;

typedef struct {
    bool grid;
} display_debug_setting_t;

typedef struct {
    reg_t textures; // texture_t
    Camera2D camera;
    draw_queue_t draw_queue;
    display_settings_t settings;
    display_debug_setting_t debug;
} renderer_t;

bool renderer_init(renderer_t *renderer, const display_settings_t *settings);
void renderer_deinit(renderer_t *renderer);
bool renderer_load(renderer_t *renderer);
void renderer_unload(renderer_t *renderer);

texture_t *renderer_new_texture(renderer_t *renderer,
    const char *name, const char *path);

bool renderer_display(renderer_t *renderer, world_t *world);
bool render_and_display(renderer_t *renderer, world_t *world);

void renderer_draw_overlay(renderer_t *renderer, world_t *world);