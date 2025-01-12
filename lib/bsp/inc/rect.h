#pragma once

#include <stdbool.h>
#include "vec2.h"

typedef enum bsp_orient_e bsp_orient_t;

typedef struct {
    bsp_v2_t pos;
    bsp_v2_t size;
} bsp_rect_t;

bsp_rect_t rect_new_i(int x, int y, int w, int h);
void rect_print(bsp_rect_t r);

bsp_rect_t rect_new_e(bsp_v2_t start, bsp_v2_t end);
bsp_v2_t rect_end(bsp_rect_t r);

bsp_v2_t rect_center(bsp_rect_t r);
bsp_rect_t rect_merge(bsp_rect_t r1, bsp_rect_t r2);
bsp_rect_t rect_grow(bsp_rect_t r, int n);
int rect_area(bsp_rect_t r);
int bsp_rect_touching_overlap(bsp_rect_t a, bsp_rect_t b);
bool rect_get_overlap_centers(bsp_rect_t r1, bsp_rect_t r2, bsp_v2_t *c1, bsp_v2_t *c2);
bsp_orient_t rect_get_overlap_orientation(bsp_rect_t r1, bsp_rect_t r2);
