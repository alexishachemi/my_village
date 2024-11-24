#include <stdlib.h>
#include <stdbool.h>
#include "draw_queue.h"
#include "linked.h"
#include "prop.h"

void draw_queue_init(draw_queue_t *dq)
{
    list_init(dq);
}

void draw_queue_deinit(draw_queue_t *dq)
{
    list_clear_free(dq);
}

static void *alloc_action(void)
{
    return malloc(sizeof(draw_action_t));
}

bool draw_queue_add(
    draw_queue_t *dq,
    texture_t *texture,
    Rectangle src_rect,
    Rectangle dest_rect,
    z_index_t z_index,
    bool fit
)
{
    draw_action_t *action = NULL;
    unsigned int insert_idx = 0;

    if (!dq || !texture)
        return false;
    for (node_t *curr = dq->head; curr; curr = curr->next, insert_idx++) {
        if (((draw_action_t *)curr->data)->z_index > z_index)
            break;
    }
    action = list_insert(dq, alloc_action, insert_idx);
    if (!action)
        return false;
    action->texture = texture;
    action->src_rect = src_rect;
    action->dest_rect = dest_rect;
    action->z_index = z_index;
    action->fit = fit;
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(draw_queue, init)
{
    draw_queue_t dq = {0};

    draw_queue_init(&dq);
    cr_assert_eq(dq.size, 0);
    draw_queue_deinit(&dq);
}

Test(draw_queue, add)
{
    draw_queue_t dq = {0};
    texture_t texture = {0};
    Rectangle rect = {0};
    z_index_t z_index = 0;

    draw_queue_init(&dq);
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, z_index, false));
    cr_assert_eq(((draw_action_t *)dq.head->data)->texture, &texture);
    cr_assert_eq(((draw_action_t *)dq.head->data)->src_rect.x, 0);
    cr_assert_eq(((draw_action_t *)dq.head->data)->z_index, 0);
    draw_queue_deinit(&dq);
}

Test(draw_queue, add_multiple)
{
    draw_queue_t dq = {0};
    texture_t texture = {0};
    Rectangle rect = {0};
    z_index_t z_index = 0;

    draw_queue_init(&dq);
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, z_index, false));
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, z_index, false));
    cr_assert_eq(((draw_action_t *)dq.head->data)->texture, &texture);
    cr_assert_eq(((draw_action_t *)dq.head->next->data)->texture, &texture);
    draw_queue_deinit(&dq);
}

Test(draw_queue, add_multiple_sorted)
{
    draw_queue_t dq = {0};
    texture_t texture = {0};
    Rectangle rect = {0};
    z_index_t z_index = 0;
    draw_action_t *action = NULL;

    draw_queue_init(&dq);
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, 1, false));
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, 0, false));
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, 5, false));
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, 2, false));
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, 4, false));
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, 3, false));
    cr_assert(draw_queue_add(&dq, &texture, rect, rect, 19, true)); 
    for (node_t *curr = dq.head; curr; curr = curr->next) {
        action = curr->data;
        cr_assert(action->z_index >= z_index);
        z_index = action->z_index;
    }
    draw_queue_deinit(&dq); 
}

#endif