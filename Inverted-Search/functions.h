#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

int read_and_validate(int argc, char *argv[]);

FileNode *create_file_node(char *filename);

int check_file_present(MainNode *head[], char *filename);

void insert_file_at_last(char *filename, FileNode **file_head);

void create_database(int argc, char *argv[], MainNode *head[]);

void store_word(char *word, char *filename, MainNode **head);

void display_database(MainNode *head[]);

//void display_file_list(FileNode *file_head);

void search_database(MainNode *head[]);

void save_database(MainNode *head[]);

void delete_file_from_list(char *filename, FileNode **file_head);

void update_database(int argc, char *argv[], MainNode *head[], FileNode **file_head);

void update_new_files(FileNode *file_head, MainNode *head[]);

#endif