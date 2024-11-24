#pragma once

#include <string.h>

#define NAME_SIZE 32
#define PATH_SIZE 128
#define STR_EQ(s1, s2) (strcmp(s1, s2) == 0)

typedef char name_t[NAME_SIZE];
typedef char path_t[PATH_SIZE];

char *namecpy(char *dest, const char *src);
char *pathcpy(char *dest, const char *src);
char *path_find_filename(const char *path);