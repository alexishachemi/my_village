#include "asset.h"
#include "chunk.h"
#include "prop.h"
#include "render.h"
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

static void draw_texture(Texture2D *texture, Rectangle texture_rect,
    Rectangle tile_rect, bool fit)
{
    Rectangle draw_rect = tile_rect;

    if (!texture)
        return;
    if (!fit)
        draw_rect = get_texture_draw_rect(texture_rect, tile_rect);
    DrawTexturePro(
        *texture,
        texture_rect,
        draw_rect,
        (Vector2){0, 0},
        0,
        WHITE
    );
}

static void draw_tile(renderer_t *renderer, tile_t *tile, size_t x, size_t y)
{
    size_t tile_size = renderer->settings.tile_size_px;
    Rectangle tile_rect = {x * tile_size, y * tile_size, tile_size, tile_size};
    terrain_t *terrain = tile->terrain;
    prop_t *prop = tile->prop;
    asset_t *asset = NULL;

    if (terrain) {
        draw_texture(
            &terrain->asset->texture->rtexture,
            terrain->asset->draw_rect,
            tile_rect,
            true
        );
    }
    if (prop) {
        asset = prop_get_asset(prop, tile->prop_orient);
        if (!asset)
            return;
        draw_texture(
            &asset->texture->rtexture,
            asset->draw_rect,
            tile_rect,
            false
        );
    }
}

static void draw_chunk(renderer_t *renderer, chunk_t *chunk)
{
    size_t offx = chunk->bounds.from_x;
    size_t offy = chunk->bounds.from_y;

    for (size_t i = 0; i < chunk->tiles.size; i++) {
        draw_tile(
            renderer,
            vec_fast_at(&chunk->tiles, i),
            offx + i % (chunk->bounds.to_x - chunk->bounds.from_x + 1),
            offy + i / (chunk->bounds.to_x - chunk->bounds.from_x + 1)
        );
    }
}

static void draw(renderer_t *renderer, world_t *world)
{
    BeginDrawing();
    // BeginMode2D(renderer->camera);
    ClearBackground(BG_COLOR);
    for (size_t i = 0; i < world->chunk_reg.last_free_index; i++) {
        draw_chunk(renderer, REG_AT(chunk_t, &world->chunk_reg, i));
    }
    // EndMode2D();
    EndDrawing();
}

static void mainloop(renderer_t *renderer, world_t *world)
{
    while (!WindowShouldClose()) {
        draw(renderer, world);
    }
}

bool renderer_display(renderer_t *renderer, world_t *world)
{
    if (!renderer || !world)
        return false;
    mainloop(renderer, world);
    return true;
}

bool render(renderer_t *renderer, world_t *world)
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
    status = renderer_display(renderer, world);
    renderer_unload(renderer);
    CloseWindow();
    return status;
}