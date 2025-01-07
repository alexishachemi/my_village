#pragma once

#include "cvec.h"

#define REG_AT(type, regp, i) ((type*)vec_fast_at(&(regp)->vec, i))
#define REG_SIZE(reg) ((reg).last_free_index)
#define REG_ADD(regp, elem, s) (reg_push_back((regp), (elem), (s)))

typedef struct {
    vec_t vec;
    size_t last_free_index;
} reg_t;

typedef void (*reg_callback_t)(void *);

bool reg_init(reg_t *reg, size_t elem_size, size_t base_size);
void reg_deinit(reg_t *reg);
void *reg_new_elem(reg_t *reg);
void *reg_push_back(reg_t *reg, void *elem, size_t size);
bool reg_map(reg_t *reg, reg_callback_t func);
size_t reg_last_idx(reg_t *reg);
void reg_pop_back(reg_t *reg, void *buf);