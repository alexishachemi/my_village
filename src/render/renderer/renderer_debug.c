#include "raylib.h"
#include "render.h"

void renderer_draw_overlay(renderer_t *renderer, world_t *world)
{
    unsigned int i = 0;
    unsigned int tile_size_px = 0;
    unsigned int chunk_size_px = 0;
    unsigned int world_size_px = 0;

    if (!renderer || !world || !renderer->debug_mode)
        return;
    tile_size_px = renderer->settings.tile_size_px;
    chunk_size_px = world->chunk_size * tile_size_px;
    world_size_px = world->size * tile_size_px;
    for (; i < world_size_px; i += tile_size_px) {
        DrawLineEx((Vector2){0, i}, (Vector2){world_size_px, i}, DEBUG_TILE_LINE_THICKNESS, DEBUG_TILE_LINE_COLOR);
        DrawLineEx((Vector2){i, 0}, (Vector2){i, world_size_px}, DEBUG_TILE_LINE_THICKNESS, DEBUG_TILE_LINE_COLOR);
        if (i % chunk_size_px == 0) {
            DrawLineEx((Vector2){0, i}, (Vector2){world_size_px, i}, DEBUG_CHUNK_LINE_THICKNESS, DEBUG_CHUNK_LINE_COLOR);
            DrawLineEx((Vector2){i, 0}, (Vector2){i, world_size_px}, DEBUG_CHUNK_LINE_THICKNESS, DEBUG_CHUNK_LINE_COLOR);
        }
    }
    if (i % chunk_size_px != 0) {
        DrawLineEx((Vector2){0, i}, (Vector2){world_size_px, i}, DEBUG_CHUNK_LINE_THICKNESS, DEBUG_CHUNK_LINE_COLOR);
        DrawLineEx((Vector2){i, 0}, (Vector2){i, world_size_px}, DEBUG_CHUNK_LINE_THICKNESS, DEBUG_CHUNK_LINE_COLOR);
    }
}