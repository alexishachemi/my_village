#include "asset.h"
#include "chunk.h"
#include "draw_queue.h"
#include "prop.h"
#include "registry.h"
#include "render.h"
#include "raymath.h"
#include "raylib.h"
#include "terrain.h"
#include "world.h"

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
    prop_t *prop = tile->prop;
    asset_t *asset = NULL;

    if (terrain && !draw_queue_add(&renderer->draw_queue, terrain->asset->texture,
        terrain->asset->draw_rect, tile_rect, TERRAIN_Z_INDEX, true)) {
        return false;
    }
    if (!prop)
        return true;
    asset = prop_get_asset(prop, tile->prop_orient);
    if (!asset)
        return false;
    return draw_queue_add(
        &renderer->draw_queue,
        asset->texture,
        asset->draw_rect,
        tile_rect,
        prop->type == PTYPE_PARENT ? prop->z_index : prop->parent->z_index,
        false
    );
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

static Vector2 get_input_axis()
{
    Vector2 input = {0};

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_Z))
        input.y -= 1;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        input.y += 1;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        input.x -= 1;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        input.x += 1;
    return input;
}

static void update(renderer_t *renderer)
{
    Vector2 velocity = Vector2Scale(get_input_axis(), CAMERA_SCROLL_SPEED);
    velocity = Vector2Scale(velocity, GetFrameTime());
    renderer->camera.target = Vector2Add(renderer->camera.target, velocity);
}

bool renderer_display(renderer_t *renderer, world_t *world)
{
    if (!renderer)
        return false;
    while (!WindowShouldClose()) {
        update(renderer);
        draw(renderer, world);
    }
    return true;
}

bool render_and_display(renderer_t *renderer, world_t *world)
{
    bool status = false;

    if (!renderer || !world)
        return false;
    InitWindow(
        renderer->settings.screen_width,
        renderer->settings.screen_height,
        "MyVillage"
    );
    if (!renderer_load(renderer)) {
        CloseWindow();
        return false;
    }
    if (!queue(renderer, world)) {
        renderer_unload(renderer);
        CloseWindow();
        return false;
    }
    status = renderer_display(renderer, world);
    renderer_unload(renderer);
    CloseWindow();
    return status;
}