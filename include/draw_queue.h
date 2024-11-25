#pragma once

#include "linked.h"
#include "prop.h"
#include "raylib.h"
#include "texture.h"

typedef struct {
    texture_t *texture;
    Rectangle src_rect;
    Rectangle dest_rect;
    z_index_t z_index;
    bool fit;
} draw_action_t;

typedef list_t draw_queue_t;

void draw_queue_init(draw_queue_t *dq);
void draw_queue_deinit(draw_queue_t *dq);
bool draw_queue_add(
    draw_queue_t *dq,
    texture_t *texture,
    Rectangle src_rect,
    Rectangle dest_rect,
    z_index_t z_index,
    bool fit
);