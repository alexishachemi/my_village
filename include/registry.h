#pragma once

#include "cvec.h"
#include "linked.h"
#include <unistd.h>

#define REG_AT(type, regp, i) ((type*)vec_fast_at(&(regp)->vec, i))
#define REG_SIZE(reg) ((reg).last_free_index)

typedef struct {
    vec_t vec;
    size_t last_free_index;
} reg_t;

typedef void (*reg_callback_t)(void *);

bool reg_init(reg_t *reg, size_t elem_size, size_t base_size);
void reg_deinit(reg_t *reg);
void *reg_new_elem(reg_t *reg);
void *reg_push_back(reg_t *reg, void *elem);
bool reg_map(reg_t *reg, reg_callback_t func);
size_t reg_last_idx(reg_t *reg);
void *reg_get_if(reg_t *reg, comparator_t compare, void *right);
void reg_pop_back(reg_t *reg, void *buf);