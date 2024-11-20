#include "registry.h"
#include "cvec.h"

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

size_t reg_size(reg_t *reg)
{
    return reg ? reg->last_free_index : 0;
}
