#include <string.h>
#include "str.h"

char *namecpy(char *dest, const char *src)
{
    return strncpy(dest, src, NAME_SIZE);
}

char *pathcpy(char *dest, const char *src)
{
    return strncpy(dest, src, PATH_SIZE);
}