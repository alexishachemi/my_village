#pragma once

#define UNUSED __attribute__((unused))

#include "linked.h"
#include "orientation.h"

bool list_shuffle(list_t *list);
bool orient_eq(orient_t *a, orient_t *b);