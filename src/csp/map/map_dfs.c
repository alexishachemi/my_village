#include "csp.h"
#include "registry.h"

static bool is_discovered(reg_t *discovered, v2_t pos)
{
    v2_t pos_buf = {0};

    for (int i = discovered->last_free_index - 1; i >= 0; i--) {
        pos_buf = *REG_AT(v2_t, discovered, i);
        if (V2_EQ(pos_buf, pos))
            return true;
    }
    return false;
}

static void dfs_search(csp_map_t *map, reg_t *discovered, v2_t pos, unsigned int layer)
{
    const v2_t directions[] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
    csp_cell_t *cell = csp_map_get_cell(map, pos, layer);

    if (!cell || cell->occupied || is_discovered(discovered, pos))
        return;
    reg_push_back(discovered, &pos);
    for (unsigned int i = 0; i < 4; i++) {
        dfs_search(map, discovered, V2_ADD(pos, directions[i]), layer);
    }
}

static bool check_position(csp_map_t *map, reg_t *discovered, bool *searched, v2_t pos, unsigned int layer)
{
    csp_cell_t *cell = csp_map_get_cell(map, pos, layer);

    if (!cell || cell->occupied)
        return true;
    if (*searched) {
        if (!is_discovered(discovered, pos))
            return false;
        return true;
    }
    *searched = true;
    dfs_search(map, discovered, pos, layer);
    return true;
}

bool csp_map_dfs_cells(csp_map_t *map, unsigned int layer)
{
    bool searched = false;
    reg_t discovered = {0};
    bool status = true;

    if (!reg_init(&discovered, sizeof(v2_t), map->area))
        return false;
    for (int y = 0; status && y < map->size.y; y++) { 
        for (int x = 0; status && x < map->size.x; x++) {
            status = check_position(map, &discovered, &searched, (v2_t){x, y}, layer);
        }
    }
    reg_deinit(&discovered);
    return status;
}
