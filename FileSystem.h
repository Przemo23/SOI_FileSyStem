#ifndef FILESYSTEM_INCLUDED
#define FILESYSTEM_INCLUDED
#define BLOCK_SIZE 1024
#define MAX_FILE_NUMBER 16

typedef struct super_block
{
    int disk_size;
    int file_number;
    int free_blocks;
    int first_file;
}super_block;

typedef struct descriptor
{
    char fname;
    int fsize;
    int address;
}descriptor;

void menu();
void create_disk(int B);
int remove_disk();
void upload_file(char *);
void download_file(char *);
void remove_file();
void show_map();
void defragment();

super_block* super;
descriptor *descriptors;


#endif // FILESYSTEM_INCLUDED
