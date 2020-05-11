#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include <sys/time.h>

#define OFFICES_NUM 4
#define LOANS_BY_OFFICE 1000

list_t books_catalog;

typedef struct __thread_args
{
    list_t *list;
    int office_number;
    int successfull_count;
    int failed_count;
} thread_args;

void *make_loans(void *args);

int main(int argc, char *argv[])
{

    init_list(&books_catalog);
    init_book_catalog(&books_catalog);

    pthread_t offices[OFFICES_NUM];
    thread_args t_args[OFFICES_NUM];

    /* Create threads */
    for (int i = 0; i < 4; i++)
    {
        thread_args p = {&books_catalog, i, 0, 0};
        t_args[i] = p;

        pthread_create(&offices[i], NULL, make_loans, &t_args[i]);
    }

    int successfull_count = 0;
    int failed_count = 0;

    for (int j = 0; j < 4; j++)
    {
        pthread_join(offices[j], NULL);

        successfull_count += t_args[j].successfull_count;
        failed_count += t_args[j].failed_count;
    }

    int total_loaned_books = (TOTAL_BOOKS - successfull_count);

    printf("Total successful loans %d, total failed loans %d \n", successfull_count, failed_count);
    printf("Number of remaining available books %d \n", total_loaned_books);
}

void *make_loans(void *args)
{
    thread_args *arguments = (thread_args *)args;
    int successfuled = 0;
    int failoans = 0;

    int seed = (int)rand();
    srand((unsigned)seed);

    printf("Office  %d with seed %d \n", arguments->office_number, seed);

    for (int i = 0; i < LOANS_BY_OFFICE; i++)
    {
        int book_to_loan = rand() % (TOTAL_BOOKS);
        int loaned = loan_book(arguments->list, book_to_loan);

        if (loaned == 1)
        {
            successfuled++;
        }
        else
        {
            failoans++;
        }
    }

    arguments->failed_count = failoans;
    arguments->successfull_count = successfuled;

    printf(
        "Office  %d has finished its workload, successful loans %d, failed loans %d \n",
        arguments->office_number, successfuled,
        failoans);

    return NULL;
}
