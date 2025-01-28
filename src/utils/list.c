#include "linked.h"
#include "utils.h"
#include <stdlib.h>

static bool merge(list_t *list, list_t *list1, list_t *list2)
{
    while (list1->head && list2->head) {
        if (rand() % 2) {
            if (!list_move(list1, rand() % list1->size, list))
                return false;
        } else if (!list_move(list2, rand() % list2->size, list))
            return false;
    }
    while (list1->head) {
        if (!list_move(list1, list1->size - 1, list))
            return false;
    }
    while (list2->head) {
        if (!list_move(list2, list2->size - 1, list))
            return false;
    }
    return true;
}

bool list_shuffle(list_t *list)
{
    list_t list1, list2;
    bool move_switch = true;

    if (!list)
        return false;
    if (list->size <= 1)
        return true;
    list_init(&list1);
    list_init(&list2);
    while (list->head) {
        if (!list_move(list, 0, (move_switch ? &list1 : &list2)))
            return false;
        move_switch = !move_switch;
    }
    return merge(list, &list1, &list2);
}