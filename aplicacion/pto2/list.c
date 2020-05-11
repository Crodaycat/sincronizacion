#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

void init_list(list_t *l)
{
    l->base = NULL;
    pthread_mutex_init(&l->lock, NULL);
}

void init_book_catalog(list_t *l)
{
    printf("Initializing list of available books\n");

    for (int i = 0; i < TOTAL_BOOKS; i++)
    {
        char name[10];
        book *b = malloc(sizeof(book));

        sprintf(name, "book_%d", i);

        b->id = i;
        b->name = name;
        b->num_pages = 100;
        b->pub_year = 2020;

        list_insert(l, i, b);
    }
}

void list_insert(list_t *l, int key, book *b)
{
    node_t *new = malloc(sizeof(node_t));

    if (new == NULL)
    {
        perror("malloc");
        return;
    }

    new->key = key;
    new->book = b;
    new->next = l->base;
    new->available = 1;

    l->base = new;
}

int loan_book(list_t *l, int key)
{
    int success = -1;
    node_t *curr = get_node_by_key(l, key);

    pthread_mutex_lock(&l->lock);

    if (curr)
    {
        if (curr->available == 1)
        {
            curr->available = 0;
            success = 1;
        }
        else
        {
            success = 0;
        }
    }

    pthread_mutex_unlock(&l->lock);

    return success;
}

node_t *get_node_by_key(list_t *l, int key)
{
    node_t *curr = l->base;

    while (curr)
    {
        if (curr->key == key)
        {
            break;
        }
        curr = curr->next;
    }

    return curr;
}