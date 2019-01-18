//
// Created by mateusz on 15.01.19.
//
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void menu()
{
    puts("Welcome to the file system");
    puts("Choose one of following options:");
    puts("1 - create VirtualDisk");
    puts("2 - delete VirtualDisk");
    puts("3 - send file from VirtualDisk to Linux");
    puts("4 - send file from Linux to VirtualDisk");
    puts("5 - remove file from VirtualDisk");
    puts("6 - show map");
    puts("q - exit");

    switch(getchar())
    {
        case '1':
            while(getchar() != '\n');
            puts("Enter size of the disk [kB]: ");
            int size;
            scanf("%d", &size);
            printf("You chose %d bytes. Additionally, x bytes are going to be allocated\n", size);
            create_disk(size);
            break;
        case '2':
            while(getchar() != '\n');
            delete_disk();
            break;
        case '3': break;
        case '4': break;
        case '5': break;
        case '6': break;
        case 'q':
            while(getchar() != '\n');
            QUIT = 1;
            puts("Bye!");
            break;
        default:
            while(getchar() != '\n');
            puts("Wrong option chosen");
    }
}

void create_disk(int B)
{
    super = malloc(sizeof(super_block));
    descriptors = malloc(sizeof(descriptor)*B);
    super->disk_size = B*BLOCK_SIZE + sizeof(descriptor)*B + sizeof(super_block);
    super->file_number = 0;
    super->free_blocks = B;
    printf("%d = %d + %d + %d \n", super->disk_size, B*BLOCK_SIZE, (int)sizeof(descriptor)*B, (int) sizeof(struct super_block));
    FILE *pt = fopen("VirtualDisk", "wb+");
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

void delete_disk()
{
    int status;
    char filename[] = "VirtualDisk";
    status = remove(filename);
    if(status == 0)
        puts("VirtualDisk deleted successfully");
    else
        puts("Couldn't delete a VirtualDisk");
}