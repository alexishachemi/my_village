#include <string.h>
#include "str.h"

inline char *namecpy(char *dest, const char *src)
{
    return strncpy(dest, src, NAME_SIZE);
}

inline char *pathcpy(char *dest, const char *src)
{
    return strncpy(dest, src, PATH_SIZE);
}

char *path_find_filename(const char *path)
{
    int i = strlen(path) - 1;

    while (i >= 0 && path[i] != '/') {
        i--;
    }
    return (char *)(path + i + 1);
}