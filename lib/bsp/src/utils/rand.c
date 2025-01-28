#include <stdlib.h>
#include "utils.h"

int rand_range(int from, int to)
{
    int max_range = to - from;
    if (max_range <= 0)
        return from > 0 ? from : 1;
    int result = from + (rand() % max_range);
    return result > 0 ? result : 1;
}