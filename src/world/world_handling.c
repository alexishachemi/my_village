#include "world.h"

bool world_pos_is_valid(world_t *world, v2_t pos)
{
    if (!world)
        return false;
    return pos.x >= 0 && (size_t)pos.x < world->size
        && pos.y >= 0 && (size_t)pos.y < world->size;
}
