#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include "counter.h"

/* structs */

/* start_routine header */
void *increment_counter();

/* Global variables */
counter_t counter;

int APPROXIMATION_FACTOR;
int MAXCNT;
int NUMTHREADS;

int main(int argc, char *argv[])
{

    /* get the command's line parameters */
    if (argc < 4)
    {
        printf("Debe llamar al programa con el umbral, el máximo del contador y el número de hilos.\n");
        return 1;
    }

    APPROXIMATION_FACTOR = atoi(argv[1]);
    MAXCNT = atoi(argv[2]);
    NUMTHREADS = atoi(argv[3]);

    /* Declaration of struct timeval variables */
    struct timeval start_tv, end_tv;

    /* Initializing conter */
    init(&counter, APPROXIMATION_FACTOR);

    /* Threads handlers */
    pthread_t threads[NUMTHREADS];

    /* Time starts counting */
    gettimeofday(&start_tv, NULL);

    /* Creating a Threads */
    for (int i = 0; i < NUMTHREADS; i++)
    {
        pthread_create(&threads[i], NULL, increment_counter, NULL);
    }

    /* Threads joins */
    for (int i = 0; i < NUMTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    /* Time stops counting here */
    gettimeofday(&end_tv, NULL);

    /* get the elapset time (end_time - start_time) */
    double elapsed_time = (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec) / 1000000.0;

    /* print the results (number threads employed, counter value, elasep time) */
    printf(
        "La cantidad de hilos es: %d, el factor de aproximación es: %d, el valor del contador es: %d, tiempo transcurrido: %lf \n",
        APPROXIMATION_FACTOR,
        NUMTHREADS,
        get(&counter),
        elapsed_time);

    return 0;
}

/* start_routine definition */
void *increment_counter()
{
    int t_id = pthread_self();
    int value = get(&counter);

    while (value < MAXCNT)
    {
        update(&counter, t_id, 1);
        value = get(&counter);
    }

    return NULL;
}
