#include "chunk.h"
#include "prop.h"
#include "render.h"
#include "raylib.h"
#include "terrain.h"
#include "world.h"

static Rectangle get_texture_draw_rect(const Texture2D *texture,
    Rectangle tile_rect)
{
    size_t offw = texture->width - (size_t)tile_rect.width;
    size_t offh = texture->height - (size_t)tile_rect.height;

    return (Rectangle){
        tile_rect.x - (float)offw / 2,
        tile_rect.y - offh,
        texture->width,
        texture->height
    };
}

static void draw_texture(renderer_t *renderer, size_t id, Rectangle tile_rect,
    bool fit)
{
    Texture2D *texture = REG_AT(Texture2D, &renderer->textures, id);
    Rectangle draw_rect = tile_rect;

    if (!texture)
        return;
    if (!fit)
        draw_rect = get_texture_draw_rect(texture, tile_rect);
    DrawTexturePro(
        *texture,
        (Rectangle){0, 0, texture->width, texture->height},
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
    terrain_t *terrain = world_get_terrain_ptr(renderer->world, tile->terrain_id);
    prop_t *prop = world_get_prop_ptr(renderer->world, tile->prop_id);

    if (terrain) {
        draw_texture(renderer, terrain->asset_id, tile_rect, true);
    }
    if (prop) {
        draw_texture(renderer,
            prop_get_asset_id(prop, tile->prop_orient), tile_rect, false);
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

static void draw(renderer_t *renderer)
{
    BeginDrawing();
    // BeginMode2D(renderer->camera);
    ClearBackground(BG_COLOR);
    for (size_t i = 0; i < renderer->world->chunk_reg.last_free_index; i++) {
        draw_chunk(renderer, REG_AT(chunk_t, &renderer->world->chunk_reg, i));
    }
    // EndMode2D();
    EndDrawing();
}

static void mainloop(renderer_t *renderer)
{
    while (!WindowShouldClose()) {
        draw(renderer);
    }
}

bool renderer_display(renderer_t *renderer)
{
    if (!renderer)
        return false;
    mainloop(renderer);
    return true;
}