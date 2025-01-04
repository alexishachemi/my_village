#include <stdio.h>
#include <stdarg.h>
#include "prop.h"
#include "world.h"

static void print_asset(const asset_t *asset)
{
    if (!asset) {
        printf("(null)\n");
        return;
    }
    printf(
        "[%s], texture: %s, [%d, %d, %d, %d]\n",
        asset->name, asset->texture->name, (int)asset->draw_rect.x,
        (int)asset->draw_rect.y, (int)asset->draw_rect.width, (int)asset->draw_rect.height
    );
}

static int iprintf(unsigned int i, const char *format, ...)
{
    va_list args = {0};
    int ret = 0;

    va_start(args, format);
    while (i--) {
        printf("\t");
    }
    ret = vprintf(format, args);
    va_end(args);
    return ret;
}

static void print_assets(reg_t *reg)
{
    asset_t *curr = NULL;

    printf("\n-- Assets --\n");
    for (unsigned int i = 0; i < reg->last_free_index; i++) {
        curr = REG_AT(asset_t, reg, i);
        iprintf(1, "- ");
        print_asset(curr);
    }
}

static void print_asset_map(prop_asset_map_t *map, unsigned int i)
{
    iprintf(i + 1, "- Asset Mode: ");
    switch (map->asset_mode) {
        case AMODE_NONE: printf("None\n"); break;
        case AMODE_MONO: {
            printf("Mono\n");
            iprintf(i + 1, "- Asset: ");
            print_asset(map->asset);
        } break;
        case AMODE_MULTI: {
            printf("Multi\n");
            iprintf(i + 1, "- Up Asset: ");
            print_asset(map->asset_up);
            iprintf(i + 1, "- Down Asset: ");
            print_asset(map->asset_down);
            iprintf(i + 1, "- Left Asset: ");
            print_asset(map->asset_left);
            iprintf(i + 1, "- right Asset: ");
            print_asset(map->asset_right);
        } break;
    }
}

static void print_prop(prop_t *prop, unsigned int i);

static void print_prop_children(prop_t *prop, unsigned int i)
{
    prop_t *child = NULL;
    
    if (!prop->has_child || prop->type != PTYPE_PARENT)
        return;
    iprintf(i, "- Children:\n");
    for (unsigned int idx = 0; idx < REG_SIZE(prop->childs); idx++) {
        child = REG_AT(prop_t, &prop->childs, idx);
        iprintf(i + 1, "- Offset: [%d, %d]\n", child->offset.x, child->offset.y);
        print_prop(child, i + 1);
    }
}

static void print_prop(prop_t *prop, unsigned int i)
{
    iprintf(i, "- [%s]\n", prop->name);
    print_asset_map(&prop->asset_map, i);
    print_prop_children(prop, i + 1);
}

static void print_props(reg_t *reg)
{
    printf("\n-- Props --\n");
    for (unsigned int i = 0; i < reg->last_free_index; i++) {
        print_prop(REG_AT(prop_t, reg, i), 1);
    }
}

void world_print(world_t *world)
{
    if (!world)
        return;
    printf("-- World --\n\t- size: %ld\n\t- chunk_size: %ld", world->size, world->chunk_size);
    print_assets(&world->asset_reg);
    print_props(&world->prop_reg);
}
