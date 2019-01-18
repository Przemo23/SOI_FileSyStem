#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "FileSystem.h"

void menu()
{
    puts("Welcome to the file system");
    puts("Choose one of following options:");
    puts("1 - create VirtualDisk");
    puts("2 - remove VirtualDisk");
    puts("3 - send file from VirtualDisk to Linux");
    puts("4 - send file from Linux to VirtualDisk");
    puts("5 - remove file from VirtualDisk");
    puts("6 - show map");
    puts("q - exit");

    switch(getchar())
    {
        case '1':
            puts("Enter size of the disk (in bytes): ");
            int size;
            scanf("%d", &size);
            printf("You chose %d bytes. Additionally, x bytes are going to be allocated\n", size);
            create_disk(size);
        case '2': break;
        case '3': break;
        case '4': break;
        case '5': break;
        case '6': break;
        case 'q': break;
        default: puts("Wrong option chosen");
    }
}

void create_disk(int B)
{
    descriptors = malloc( B * sizeof(descriptor));
    super = malloc(sizeof(super_block));
    if(super == NULL)
    {
        printf("Failed memory allocation.\n");
        return;
    }
    super->disk_size = (int)sizeof(descriptor)+(int)sizeof(super_block)+B*BLOCK_SIZE;
    super->free_blocks = B;
    super->file_number = 0;

    FILE *pt = fopen("VirtualDisk", "w+");


    if (fwrite(super, sizeof(super_block), 1,pt) != 1)
    {
         printf("Error. Could not write superblock data\n");
        fclose(pt);
    }
    else
    {
        printf("Successfully created file system\n");
        fclose(pt);
    }
}
