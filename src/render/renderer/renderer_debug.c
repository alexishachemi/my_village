
#include "texture.h"
#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" // Full of unused parameters :(
#pragma GCC diagnostic pop

#include "chunk.h"
#include "raylib.h"
#include "registry.h"
#include "render.h"
#include "world.h"
#include <stdio.h>

static void draw_grid(renderer_t *renderer, world_t *world)
{
    unsigned int i = 0;
    unsigned int tile_size_px = 0;
    unsigned int chunk_size_px = 0;
    unsigned int world_size_px = 0;

    if (!renderer->debug.grid)
        return;
    tile_size_px = renderer->settings.tile_size_px;
    chunk_size_px = world->chunk_size * tile_size_px;
    world_size_px = world->size * tile_size_px;
    for (; i < world_size_px; i += tile_size_px) {
        DrawLineEx((Vector2){0, i}, (Vector2){world_size_px, i},
            DEBUG_TILE_LINE_THICKNESS, DEBUG_TILE_LINE_COLOR);
        DrawLineEx((Vector2){i, 0}, (Vector2){i, world_size_px},
            DEBUG_TILE_LINE_THICKNESS, DEBUG_TILE_LINE_COLOR);
        if (i % chunk_size_px == 0) {
            DrawLineEx((Vector2){0, i}, (Vector2){world_size_px, i},
                DEBUG_CHUNK_LINE_THICKNESS, DEBUG_CHUNK_LINE_COLOR);
            DrawLineEx((Vector2){i, 0}, (Vector2){i, world_size_px},
                DEBUG_CHUNK_LINE_THICKNESS, DEBUG_CHUNK_LINE_COLOR);
        }
    }
    DrawLineEx((Vector2){0, world_size_px}, (Vector2){world_size_px, world_size_px},
        DEBUG_CHUNK_LINE_THICKNESS, DEBUG_CHUNK_LINE_COLOR);
    DrawLineEx((Vector2){world_size_px, 0}, (Vector2){world_size_px, world_size_px},
        DEBUG_CHUNK_LINE_THICKNESS, DEBUG_CHUNK_LINE_COLOR);
}

static void draw_top_bar(renderer_t *renderer, world_t *world)
{
    size_t tiles = 0;
    char buf[1024];
    Rectangle top_bar = {0, 0, GetScreenWidth(), 32};

    for (unsigned int i = 0; i < REG_SIZE(world->chunks); i++) {
        tiles += REG_AT(chunk_t, &world->chunks, i)->tiles.size;
    }
    DrawRectangleRec(top_bar, WHITE);
    snprintf(
        buf, 1024,
        "Tiles: %ld\t\tChunks: %ld",
        tiles, REG_SIZE(world->chunks)
    );
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiLabel(top_bar, buf);
    GuiToggle((Rectangle){top_bar.width - 54, top_bar.y + 3, 50, top_bar.height - 6},
        "Grid", &renderer->debug.grid);
    if (GuiButton((Rectangle){top_bar.width - 138, top_bar.y + 3, 80, top_bar.height - 6}, "Center"))
        renderer_center_camera(renderer, world);
}

void renderer_draw_overlay(renderer_t *renderer, world_t *world)
{
    if (!renderer || !world)
        return;
    draw_grid(renderer, world);
    EndMode2D();
    draw_top_bar(renderer, world);
    BeginMode2D(renderer->camera);
}

void renderer_print(renderer_t *renderer)
{
    texture_t *texture = NULL;

    if (!renderer)
        return;
    printf("-- Renderer --\n    - screen_size: %dx%d\n    - tile_size_px: %ld\n",
        renderer->settings.screen_width, renderer->settings.screen_height,
        renderer->settings.tile_size_px);
    printf("\n-- Textures --\n");
    for (unsigned int i = 0; i < REG_SIZE(renderer->textures); i++) {
        texture = REG_AT(texture_t, &renderer->textures, i);
        printf("    - \"%s\" -> %s\n", texture->name, texture->path);
    }
}
