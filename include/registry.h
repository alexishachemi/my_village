#pragma once

#include "cvec.h"

#define REG_AT(type, reg_ptr, i) ((type*)vec_fast_at(&(reg_ptr)->vec, i))

typedef struct {
    vec_t vec;
    size_t last_free_index;
} reg_t;

typedef void (*reg_callback_t)(void *);

bool reg_init(reg_t *reg, size_t elem_size, size_t base_size);
void reg_deinit(reg_t *reg);
void *reg_new_elem(reg_t *reg);
size_t reg_size(reg_t *reg);
bool reg_map(reg_t *reg, reg_callback_t func);
size_t reg_last_idx(reg_t *reg);