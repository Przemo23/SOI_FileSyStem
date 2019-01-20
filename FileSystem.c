#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "FileSystem.h"
#include <string.h>
#define BLOCK_SIZE 1024


void menu()
{
    while(QUIT == 0)
    {
        puts("\n\nWelcome to the file system");
        puts("Choose one of following options:");
        puts("1 - create VirtualDisk");
        puts("2 - delete VirtualDisk");
        puts("3 - send file from VirtualDisk to Linux");
        puts("4 - send file from Linux to VirtualDisk");
        puts("5 - remove file from VirtualDisk");
        puts("6 - show map");
        puts("7 - defragmentate");
        puts("d - show descriptors");
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
                while(getchar() != '\n');
                break;

            case '2':
                while(getchar() != '\n');
                delete_disk();
                break;

            case '3':
                while(getchar() != '\n');
                puts("What's the name of the file you want to download (max size 10)?");
                char TempName[10] = {'\000'};
                scanf("%s",TempName);
                download_file(TempName);
                break;

            case '4':
                while(getchar() != '\n');
                puts("What's the name of the file you want to upload (max size 10)?");
                char TempName1[10];
                scanf("%s",TempName1);
                upload_file(TempName1);
                break;

            case '5':
                while(getchar() != '\n');
                if(super->file_number <= 0)
                    puts("The VirtualDisk is empty");
                else
                {
                    int choice;
                    puts("Choose one of the following files: ");
                    for (int iter = 0; iter < super->file_number; iter++)
                        printf("File %d: %s\n", iter + 1, descriptors[iter].fname);
                    scanf("%d", &choice);
                    while(getchar() != '\n');
                    remove_file(choice);
                }
                break;

            case '6':
                while(getchar() != '\n');
                show_map();
                break;
            case '7':
                while(getchar() != '\n');
                defragment();
                break;
            case 'd':
                while(getchar() != '\n');
                show_descriptors();
                break;

            case 'q':
                while(getchar() != '\n');
                QUIT = 1;
                puts("Bye!");
                break;

            default:
                while(getchar() != '\n');
                puts("Wrong option chosen");
                break;
        }
    }
}
void load_disk()
{
    FILE *pt;
    super = malloc(sizeof(super_block));
    pt = fopen("VirtualDisk", "rb");
    if(!pt)
        return ;
    fseek(pt, 0L, SEEK_SET);
    fread(super, sizeof(super_block), 1, pt);
    size_t descriptors_number = super->file_number;
    descriptors = malloc(sizeof(descriptor)*descriptors_number);
    fread(descriptors, sizeof(descriptor)*descriptors_number,1, pt);
    puts("VirtualDisk parameters: \n\n");
    printf("Disk size: %d\n", super->disk_size);
    printf("Blocks allocated: %d\n", super->all_blocks);
    printf("Free blocks: %d\n", super->free_blocks);
    printf("Number of files: %d\n", super->file_number);
    printf("First file address: %d\n\n\n", super->first_file);
    bitmap = malloc(sizeof(bool)*super->all_blocks);
    int iter, iter2;
    for(iter = 0; iter<super->all_blocks;iter++)
        bitmap[iter] = false;
    for(iter = 0; iter<super->file_number; iter++)
        for(iter2 = (descriptors[iter].address-super->first_file)/BLOCK_SIZE; iter2 < (descriptors[iter].address-super->first_file)/BLOCK_SIZE + descriptors[super->file_number-1].fsize; iter2++)
            bitmap[iter2] = true;
}

void create_disk(int B)
{
    bitmap = malloc(sizeof(bool)*B);
    int iter;
    for(iter = 0; iter<B; iter++)
        bitmap[iter] = false;
    super = malloc(sizeof(super_block));
    descriptors = malloc(sizeof(descriptor)*B);
    super->disk_size = B*BLOCK_SIZE + sizeof(descriptor)*B + sizeof(super_block);
    super->file_number = 0;
    super->free_blocks = B;
    super->first_file = super->disk_size - B*BLOCK_SIZE;
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
    {
        free(descriptors);
        free(super);
        free(bitmap);
        puts("VirtualDisk deleted successfully");
    }
    else
        puts("Couldn't delete VirtualDisk");
}
void upload_file(char* FileName)
{
    while(getchar() != '\n');
    char line;

    FILE *pt = fopen(FileName,"r+b");
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
    descriptors[super->file_number].fsize = FileSize/BLOCK_SIZE+1;
    if(super->file_number == 0)
        descriptors[super->file_number].address = sizeof(super_block) + sizeof(descriptor)*super->all_blocks;
    else
        descriptors[super->file_number].address = descriptors[super->file_number-1].address + descriptors[super->file_number-1].fsize*BLOCK_SIZE;

    fseek ( disk , descriptors[super->file_number].address,SEEK_SET);
    do /* read a line */
    {
        line = fgetc(pt);
        fputc(line,disk);

    }while(line != EOF);
    super->file_number++;
    super->free_blocks -= FileSize/BLOCK_SIZE + 1; /*not the best solution*/

    fseek(disk,0L,SEEK_SET);
    fwrite(super, sizeof(super_block), 1,disk);
    fseek(disk,sizeof(super_block),SEEK_SET);
    fwrite(descriptors,sizeof(descriptor)*super->file_number,1,disk);
    puts("Download succesful");
    int it;
    for(it = descriptors[super->file_number -1].address/BLOCK_SIZE; it <descriptors[super->file_number -1].address/BLOCK_SIZE + descriptors[super->file_number-1].fsize; it++ )
        bitmap[it] = true;
    fclose (disk);
    fclose (pt);
}

void show_map()
{
     int iter;
     printf("| S_B |");
     for(iter = 0; iter<super->file_number; iter++)
         printf(" D%d |", iter+1);
     for(iter = 0; iter < super->free_blocks; iter++)
         printf(" D_free |");
     for(iter = 0; iter < super->all_blocks; iter++)
     {
         if(bitmap[iter] == false)
             printf(" FREE |");
         else
             printf(" FILLED |");
     }
     puts("\n");
}
void show_descriptors()
{
    int iter;
    for(iter = 0; iter < super->file_number; iter++)
    {
        printf("DESCRIPTOR %d\n",  iter + 1);
        printf("%s\n", descriptors[iter].fname);
        printf("%d\n", descriptors[iter].address);
        printf("%d\n", descriptors[iter].fsize);
        puts("\n\n");
    }
}

void remove_file(int choice)
{
    int it;
    for(it = descriptors[choice-1].address/BLOCK_SIZE; it <descriptors[choice -1].address/BLOCK_SIZE + descriptors[choice-1].fsize; it++ )
        bitmap[it] = false;
    super->free_blocks+=descriptors[choice-1].fsize;
    FILE* Disk = fopen("VirtualDisk","r+b");
    if(Disk == NULL)
    {
        puts("Opening the disk failed.");
        return;
    }
    /*Cleaning the file*/

    fseek(Disk,descriptors[choice-1].address,SEEK_SET);
    for(it = 0; it<descriptors[choice-1].fsize*BLOCK_SIZE;it++)
        fputc('\000',Disk);
    /*Clearing and moving descriptors */

    int CharsLeft,CharsToCopy;
    CharsToCopy = (super->file_number - choice)*sizeof(descriptor);
    char c;
    for(CharsLeft = CharsToCopy; CharsLeft>0;CharsLeft--)
    {
        fseek(Disk,(choice+1) * sizeof(descriptor) +(CharsToCopy - CharsLeft),SEEK_SET);
        c = fgetc(Disk);
        fseek(Disk, choice * sizeof(descriptor) + (CharsToCopy - CharsLeft),SEEK_SET);
        fputc(c,Disk);
    }
    fseek(Disk,super->file_number*sizeof(descriptor),SEEK_SET);
    for(it=0;it<sizeof(descriptor);it++)
        fputc('\000',Disk);
    /* Clearing descriptors array*/
    int iter;
    for(iter = choice-1; iter<super->file_number; iter++)
    {
        descriptors[iter].fsize = descriptors[iter+1].fsize;
        descriptors[iter].address = descriptors[iter+1].address;
        strcpy(descriptors[iter].fname, descriptors[iter+1].fname);
    }
    super->file_number--;
    fseek(Disk,0L,SEEK_SET);
    fwrite(super, sizeof(super_block), 1,Disk);
    fclose(Disk);
}
void download_file(char* FileName)
{
    while(getchar() != '\n');
    bool QUIT;
    int Iter,FileAdress,FileSize;
    FileSize=-1;
    for(Iter = 0 ,QUIT = 0;Iter<super->file_number && QUIT == 0;Iter++)
    {
        if(strcmp(FileName,descriptors[Iter].fname)==0)
        {
            FileSize = descriptors[Iter].fsize;
            FileAdress = descriptors[Iter].address;
            QUIT = 1;
        }
    }
    if(FileSize == -1)
    {
        puts("There is no such file to download");
        return;
    }
    puts("Found file.");
    strcat(FileName,".dl");
    char linec;
    FILE* Downloaded = fopen(FileName,"wb+");
    FILE* Disk = fopen("VirtualDisk","rb+");
    fseek(Disk,FileAdress,SEEK_SET);
    linec = fgetc(Disk);
    while(linec>0 && linec<127)
    {

        fputc(linec,Downloaded);
        linec = fgetc(Disk);
    }
    fclose(Downloaded);
    fclose(Disk);
    puts("Download succesful");
    return;
}
void defragment()
{
    FILE* Disk = fopen("VirtualDisk","r+b");
    if(Disk == NULL)
    {
        puts("Opening the disk failed.");
        return;
    }
    if(super->file_number == 0)
    {
        puts("The disk is empty");
        return;
    }
    int FirstFile,CharsLeft,FirstHole,NextHole,TempIter,TempIter2,DIter;
    char c;
    FirstHole = -1;
    for(FirstFile = 0;FirstFile<super->all_blocks;FirstFile++)
    {
        if(bitmap[FirstFile]==false)
            FirstHole = FirstFile;
        if(bitmap[FirstFile]==true && FirstHole > -1)
        {

            for(NextHole = FirstFile;bitmap[NextHole]==true && NextHole < super->all_blocks;NextHole++)
                bitmap[NextHole] = false;
            int CharsToCopy =(NextHole-FirstFile)*BLOCK_SIZE;
            for(CharsLeft = CharsToCopy; CharsLeft>0;CharsLeft--)
            {
                fseek(Disk,super->first_file + FirstFile*BLOCK_SIZE +(CharsToCopy - CharsLeft),SEEK_SET);
                c = fgetc(Disk);
                fseek(Disk,super->first_file + FirstHole*BLOCK_SIZE + (CharsToCopy - CharsLeft),SEEK_SET);
                fputc(c,Disk);
            }
            for(TempIter = FirstHole,TempIter2=0;TempIter2<NextHole-FirstFile;TempIter++,TempIter2++)
                bitmap[TempIter]=true;
            FirstFile = NextHole-1;
            FirstHole = -1;
        }
    }
    /* Updating  descriptors*/

    DIter = 0;
    descriptors[DIter].address = super->first_file;
    while(DIter<super->file_number-1)
    {
      descriptors[1+DIter].address = descriptors[DIter].address + descriptors[DIter].fsize*BLOCK_SIZE;
        DIter++;
    }
    fclose(Disk);



}


