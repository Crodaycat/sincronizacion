
#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <pthread.h>
#define TOTAL_BOOKS 10000

typedef struct __book
{
    int id;
    char *name;
    int num_pages;
    int pub_year;
} book;

typedef struct __node_t
{
    int key;
    int available;
    struct __book *book;
    struct __node_t *next;
} node_t;

typedef struct __list_t
{
    node_t *base;
    pthread_mutex_t lock;
} list_t;

void init_list(list_t *l);
void init_book_catalog(list_t *l);
void list_insert(list_t *l, int key, book *b);
node_t *get_node_by_key(list_t *l, int key);
int loan_book(list_t *l, int key);
#endif