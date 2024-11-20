#ifndef TEST
#define MAIN main
#else
#define MAIN _main
#endif

#include <stdio.h>

int MAIN(void)
{
    printf("Hello, World!\n");
    return 0;
}
