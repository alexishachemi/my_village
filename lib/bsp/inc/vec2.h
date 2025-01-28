#pragma once

typedef struct {
    int x;
    int y;
} bsp_v2_t;

float vec2_dist(bsp_v2_t a, bsp_v2_t b);
bsp_v2_t vec2_move_towards(bsp_v2_t from, bsp_v2_t to, int delta);
void vec2_print(bsp_v2_t vec);

bsp_v2_t vec2_add(bsp_v2_t a, bsp_v2_t b);
bsp_v2_t vec2_sub(bsp_v2_t a, bsp_v2_t b);
bsp_v2_t vec2_mul(bsp_v2_t a, bsp_v2_t b);
bsp_v2_t vec2_div(bsp_v2_t a, bsp_v2_t b);

bsp_v2_t vec2_add_i(bsp_v2_t a, int n);
bsp_v2_t vec2_sub_i(bsp_v2_t a, int n);
bsp_v2_t vec2_mul_i(bsp_v2_t a, int n);
bsp_v2_t vec2_div_i(bsp_v2_t a, int n);
