#pragma once

#include "cvec.h"

typedef struct {
    vec_t vec;
    size_t last_free_index;
} reg_t;

bool reg_init(reg_t *reg, size_t elem_size, size_t base_size);
void reg_deinit(reg_t *reg);
void *reg_new_elem(reg_t *reg);
size_t reg_size(reg_t *reg);