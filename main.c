#include <stdio.h>
#include "FileSystem.h"
int main()
{
    if(load_disk()==1)
        puts("Succesfully loaded disc.");

    menu();
    return 0;
}
