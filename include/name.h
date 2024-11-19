#pragma once

#define NAME_SIZE 32
#define PATH_SIZE 128

typedef char name_t[NAME_SIZE];
typedef char path_t[PATH_SIZE];

char *namecpy(char *dest, const char *src);
char *pathcpy(char *dest, const char *src);