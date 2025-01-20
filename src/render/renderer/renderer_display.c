#include "asset.h"
#include "chunk.h"
#include "draw_queue.h"
#include "prop.h"
#include "registry.h"
#include "render.h"
#include "raylib.h"
#include "terrain.h"
#include "tile.h"
#include "world.h"
#include <stdio.h>

static Rectangle get_texture_draw_rect(Rectangle texture_rect, 
    Rectangle tile_rect)
{
    float offw = texture_rect.width - tile_rect.width;
    float offh = texture_rect.height - tile_rect.height;

    return (Rectangle){
        tile_rect.x - offw / 2,
        tile_rect.y - offh,
        texture_rect.width,
        texture_rect.height
    };
}

static void draw_action(draw_action_t *action)
{
    Rectangle draw_rect = action->dest_rect;

    if (!action->fit)
        draw_rect = get_texture_draw_rect(action->src_rect, action->dest_rect);
    DrawTexturePro(
        action->texture->rtexture,
        action->src_rect,
        draw_rect,
        (Vector2){0, 0},
        0,
        WHITE
    );
}

static bool queue_tile(renderer_t *renderer, tile_t *tile, size_t x, size_t y)
{
    size_t tile_size = renderer->settings.tile_size_px;
    Rectangle tile_rect = {x * tile_size, y * tile_size, tile_size, tile_size};
    terrain_t *terrain = tile->terrain;
    placed_prop_t *placed = NULL;
    asset_t *asset = NULL;

    if (terrain && !draw_queue_add(&renderer->draw_queue, terrain->asset->texture,
        terrain->asset->draw_rect, tile_rect, TERRAIN_Z_INDEX, true)) {
        return false;
    }
    for (node_t *itt = tile->props.head; itt; itt = itt->next) {
        placed = itt->data;
        asset = prop_get_asset(placed->prop, placed->orient);
        if (!asset)
            return false;
        if (!draw_queue_add(&renderer->draw_queue, asset->texture, asset->draw_rect, tile_rect,
            placed->prop->type == PTYPE_PARENT ? placed->prop->z_index : placed->prop->parent->z_index, false))
            return false;
    }
    return true;
}

static bool queue_chunk(renderer_t *renderer, chunk_t *chunk)
{
    size_t offx = chunk->bounds.from_x;
    size_t offy = chunk->bounds.from_y;
    bool success = true;

    for (size_t i = 0; success && i < chunk->tiles.size; i++) {
        success = queue_tile(
            renderer,
            vec_fast_at(&chunk->tiles, i),
            offx + i % (chunk->bounds.to_x - chunk->bounds.from_x + 1),
            offy + i / (chunk->bounds.to_x - chunk->bounds.from_x + 1)
        );
    }
    return success;
}

static bool queue(renderer_t *renderer, world_t *world)
{
    for (size_t i = 0; i < REG_SIZE(world->chunk_reg); i++) {
        if (!queue_chunk(renderer, REG_AT(chunk_t, &world->chunk_reg, i)))
            return false;
    }
    return true;
}

static void draw(renderer_t *renderer, world_t *world)
{
    BeginDrawing();
    BeginMode2D(renderer->camera);
    ClearBackground(BG_COLOR);
    list_map(&renderer->draw_queue, (callback_t)draw_action);
    renderer_draw_overlay(renderer, world);
    EndMode2D();
    EndDrawing();
}

static void update(renderer_t *renderer, world_t *world)
{
    renderer_update_camera(&renderer->camera);
    if (IsKeyPressed(KEY_SPACE))
        renderer_center_camera(renderer, world);
}

bool renderer_display(renderer_t *renderer, world_t *world)
{

    if (!renderer)
        return false;
    renderer_center_camera(renderer, world);
    while (!WindowShouldClose()) {
        update(renderer, world);
        draw(renderer, world);
    }
    return true;
}

bool render_and_display(renderer_t *renderer, world_t *world)
{
    bool status = false;

    if (!renderer || !world)
        return false;
    SetTraceLogLevel(LOG_NONE);
    InitWindow(
        renderer->settings.screen_width,
        renderer->settings.screen_height,
        "MyVillage"
    );
    if (!renderer_load(renderer)) {
        dprintf(2, "Failed to load renderer\n");
        CloseWindow();
        return false;
    }
    if (!queue(renderer, world)) {
        dprintf(2, "Failed to queue render actions\n");
        renderer_unload(renderer);
        CloseWindow();
        return false;
    }
    status = renderer_display(renderer, world);
    renderer_unload(renderer);
    CloseWindow();
    return status;
}