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