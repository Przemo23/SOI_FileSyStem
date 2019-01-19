//
// Created by mateusz on 15.01.19.
//
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
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
            FILE *temp = fopen("VirtualDisk", "rb");
            if(temp)
            {
                puts("Disk already exists");
                break;
            }
            puts("Enter size of the disk [kB]: ");
            int size;
            scanf("%d", &size);
            printf("You chose %d [kB]. Additionally, %d [B] are going to be allocated\n", size, sizeof(super_block) +
                    sizeof(descriptor)*size);
            create_disk(size);
            break;


        case '2':
            while(getchar() != '\n');
            delete_disk();
            break;
        case '3':
            break;
        case '4':
            while(getchar() != '\n');
            char filename[MAX_FILENAME_LENGTH];
            puts("Name of file to upload: ");
            scanf("%s", &filename);
            upload_file(filename);
            break;
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
    super->first_file = super->disk_size - BLOCK_SIZE*B;
    super->all_blocks = B;
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

void upload_file(char *FileName)
{
    char line[256];
    char linec[256];
    FILE *pt = fopen("plik2","r+b");
    FILE *disk = fopen("VirtualDisk","w+b");
    int FileSize;
    if(pt == NULL)
    {
        puts("There is no such file to upload.");
        return;
    }
    if(disk == NULL)
    {
        puts("Not able to open the disk.");
        return;
    }
    fseek(pt, 0L, SEEK_END);
    FileSize = ftell(pt);
    if(FileSize > super->free_blocks*BLOCK_SIZE)
    {
        puts("There isn't enough space for the file.");
        return;

    }
    fseek(pt, 0L,SEEK_SET);
    strcpy(descriptors[super->file_number].fname,FileName);
    descriptors[super->file_number].fsize = FileSize/BLOCK_SIZE + 1;
    descriptors[super->file_number].address = super->disk_size-super->free_blocks*BLOCK_SIZE;
    fseek ( disk , -(super->disk_size-super->free_blocks*BLOCK_SIZE) , SEEK_SET );
    while ( fgets ( line, sizeof line,pt ) != NULL ) /* read a line */
    {
        fputs (line, stdout); /* write the line */
        strcpy(linec, line);
        fprintf (disk , linec);
    }
    super->file_number++;
    super->free_blocks -= FileSize/BLOCK_SIZE + 1; /*not the best solution*/

    fseek(disk,0L,SEEK_SET);
    fwrite(super, sizeof(super_block), 1,disk);
    fseek(disk,sizeof(super_block)+(super->file_number-1)*sizeof(descriptor),SEEK_SET);
    fwrite(&descriptors[super->file_number-1],sizeof(descriptor),1,disk);


    fclose (disk);
    fclose (pt);

}

void load_disk()
{
    FILE *pt;
    super = malloc(sizeof(super_block));
    pt = fopen("VirtualDisk", "rb");
    if(!pt)
        return;
    fseek(pt, 0L, SEEK_SET);
    fread(super, sizeof(super_block), 1, pt);
    size_t descriptors_number = super->file_number;
    descriptors = malloc(sizeof(descriptor)*descriptors_number);
    fread(descriptors, sizeof(descriptor), descriptors_number, pt);
    puts("VirtualDisk parameters: \n\n");
    printf("Disk size: %d\n", super->disk_size);
    printf("Blocks allocated: %d\n", super->all_blocks);
    printf("Free blocks: %d\n", super->free_blocks);
    printf("Number of files: %d\n", super->file_number);
    printf("First file address: %d\n\n\n", super->first_file);
}
