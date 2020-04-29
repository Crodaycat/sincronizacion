#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include "counter.h"

/* structs */
// code here (if you required it)...

/* start_routine header */
void *incrementCounter(void *);

/* Global variables */
// code here (if you required it)...
counter_t counter;
int MAXCNT;
int NUMTHREADS;

int main(int argc, char *argv[])
{

    /* get the command's line parameters */
    if (argc < 3)
    {
        printf("Debe llamar al programa con el maximo del contador y el número de hilos.\n");
        return 1;
    }

    MAXCNT = atoi(argv[1]);
    NUMTHREADS = atoi(argv[2]);
    int maxCntPerThread[NUMTHREADS];

    /* Declaration of struct timeval variables */
    struct timeval start_tv, end_tv;

    /* Initializing conter */
    init(&counter);

    /* Threads handlers */
    pthread_t threads[NUMTHREADS];

    /* Thread creation */
    int res = MAXCNT % NUMTHREADS;
    int threadsMax = MAXCNT / NUMTHREADS;

    for (int i = 0; i < NUMTHREADS; i++)
    {
        maxCntPerThread[i] = threadsMax;

        if (res > 0 && i < res)
        {
            maxCntPerThread[i] += 1;
        }
    }

    /* Time starts counting */
    gettimeofday(&start_tv, NULL);

    /* Creating a Threads */
    for (int i = 0; i < NUMTHREADS; i++)
    {
        pthread_create(&threads[i], NULL, incrementCounter, ((void *)&maxCntPerThread[i]));
    }

    /* Threads joins */
    for (int i = 0; i < NUMTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    /* get the end time */
    gettimeofday(&end_tv, NULL);

    /* get the elapset time (end_time - start_time) */
    double elapsed_time = (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec) / 1000000.0;

    /* print the results (number threads employed, counter value, elasep time) */
    printf("La cantidad de hilos es: %d, el valor del contador es: %d, tiempo transcurrido: %lf \n", NUMTHREADS, counter.value, elapsed_time);

    return 0;
}

/* start_routine definition */
void *incrementCounter(void *param)
{
    int max = *(int *)param;

    for (int i = 0; i < max; i++)
    {
        increment(&counter);
    }

    return NULL;
}
