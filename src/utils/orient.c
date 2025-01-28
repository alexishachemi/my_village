#include <stdbool.h>
#include "orientation.h"

bool orient_eq(orient_t *a, orient_t *b)
{
    return a && b && *a == *b;
}
