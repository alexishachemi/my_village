#include "registry.h"
#include "cvec.h"
#include <string.h>

bool reg_init(reg_t *reg, size_t elem_size, size_t base_size)
{
    if (!reg)
        return false;
    reg->vec = vec_create(elem_size);
    vec_fast_resize(&reg->vec, base_size);
    reg->last_free_index = 0;
    return true;
}

void reg_deinit(reg_t *reg)
{
    if (!reg)
        return;
    vec_free_data(&reg->vec);
}

void *reg_new_elem(reg_t *reg)
{
    if (!reg)
        return NULL;
    if (reg->last_free_index >= reg->vec.size
        && !vec_fast_resize(&reg->vec, reg->vec.size * 2))
        return NULL;
    return vec_fast_at(&reg->vec, reg->last_free_index++);
}

void *reg_push_back(reg_t *reg, void *elem, size_t size)
{
    void *new = NULL;

    if (!reg || !elem)
        return NULL;
    new = reg_new_elem(reg);
    if (!reg)
        return NULL;
    return memcpy(new, elem, size);
}

bool reg_map(reg_t *reg, reg_callback_t func)
{
    if (!reg || !func)
        return false;
    for (size_t i = 0; i < REG_SIZE(*reg); i++) {
        func(vec_fast_at(&reg->vec, i));
    }
    return true;
}

size_t reg_last_idx(reg_t *reg)
{
    if (!reg)
        return 0;
    return reg->last_free_index - 1;
}

void reg_pop_back(reg_t *reg, void *buf)
{
    void *last = NULL;

    if (!reg)
        return;
    last = vec_fast_at(&reg->vec, reg->last_free_index - 1);
    if (buf)
        memcpy(buf, last, reg->vec.elem_size);
    memset(last, 0, reg->vec.elem_size);
    reg->last_free_index -= 1;
}
