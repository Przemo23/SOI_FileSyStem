//
// Created by mateusz on 15.01.19.
//
#include <stdbool.h>
#ifndef FS_HEADER_H
#define FS_HEADER_H
#define BLOCK_SIZE 1024
#define MAX_FILENAME_LENGTH 10
int QUIT;
typedef struct super_block
{
    int disk_size;
    int file_number;
    int free_blocks;
    int all_blocks;
    int first_file;
}super_block;

typedef struct descriptor
{
    char fname[MAX_FILENAME_LENGTH];
    int fsize;
    int address;
}descriptor;

void menu();
void create_disk(int B);
void delete_disk();
void upload_file(char *FileName);
void download_file(char *filename);
void remove_file(int choice);
void show_map();
void defragment();
void load_disk();
void makebinfile();
void show_descriptors();

bool *bitmap;
super_block *super;
descriptor *descriptors;

#endif //FS_HEADER_H
