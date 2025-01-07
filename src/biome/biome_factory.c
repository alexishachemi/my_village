#include <sys/types.h>
#include "biome.h"
#include "registry.h"
#include "str.h"
#include "terrain.h"
#include "prop.h"

bool biome_init(biome_t *biome, const char *name)
{
    if (!biome || !name)
        return false;
    namecpy(biome->name, name);
    reg_init(&biome->parents, sizeof(biome_t*), BIOME_HERITAGE_BASE_SIZE);
    reg_init(&biome->terrains, sizeof(terrain_t *), BIOME_TERRAIN_BASE_SIZE);
    reg_init(&biome->props, sizeof(prop_t*), BIOME_PROPS_BASE_SIZE);
    return true;
}

void biome_deinit(biome_t *biome)
{
    if (!biome)
        return;
    reg_deinit(&biome->parents);
    reg_deinit(&biome->terrains);
    reg_deinit(&biome->props);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(biome, init)
{
    biome_t biome = {0};

    cr_assert(biome_init(&biome, "test_biome"));
    biome_deinit(&biome);
}

#endif
