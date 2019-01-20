#include <stdio.h>
#include "FileSystem.h"
int main()
{
    QUIT = 0;
    load_disk();
    while(!QUIT) menu();
    return 0;
}
