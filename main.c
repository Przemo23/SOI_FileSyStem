#include <stdio.h>
#include "header.h"
int main()
{
    QUIT = 0;
    load_disk();
    while(!QUIT) menu();
    return 0;
}


