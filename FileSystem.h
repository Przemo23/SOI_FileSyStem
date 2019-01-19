#ifndef FILESYSTEM_INCLUDED
#define FILESYSTEM_INCLUDED
#define BLOCK_SIZE 1024
#define MAX_FILE_NUMBER 16
#include <stdbool.h>

typedef struct super_block
{
    int disk_size;
    int file_number;
    int free_blocks;
    int first_file;
    int all_blocks;
}super_block;

typedef struct descriptor
{
    char fname[10];
    int fsize;
    int address;
}descriptor;

void menu();
void create_disk(int B);
void delete_disk();
void upload_file(char *);
void download_file(char *);
void remove_file();
void show_map();
void defragment();
bool load_disk();
void show_descriptors();

super_block* super;
descriptor* descriptors;
bool *bitmap;


#endif
