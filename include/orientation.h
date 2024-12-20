#pragma once

typedef enum {
    ORIENT_DOWN = 0,
    ORIENT_UP = 1,
    ORIENT_LEFT = 2,
    ORIENT_RIGHT = 3
} orient_t;

static const orient_t directions[] = { ORIENT_DOWN, ORIENT_UP, ORIENT_LEFT, ORIENT_RIGHT };