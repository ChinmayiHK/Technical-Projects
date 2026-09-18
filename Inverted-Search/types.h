#ifndef TYPES_H
#define TYPES_H

typedef struct sub_node
{
    char file_name[20];
    int word_count;

    struct sub_node *link;

} SubNode;


typedef struct main_node
{
    char word[20];
    int file_count;

    SubNode *sub_link;

    struct main_node *main_link;

} MainNode;


typedef struct file_node
{
    char file_name[20];

    struct file_node *link;

} FileNode;

#endif