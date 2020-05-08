#ifndef COUNTER_H
#define COUNTER_H
#include <stdio.h>
#include <pthread.h>

#define NUMCPUS 8

typedef struct __counter_t
{
    int global;                     // global count
    pthread_mutex_t glock;          // global lock
    int local[NUMCPUS];             // per-CPU count
    pthread_mutex_t llock[NUMCPUS]; // ... and locks
    int threshold;                  // update frequency
} counter_t;

void init(counter_t *c, int threshold);
void update(counter_t *c, int threadID, int amt);
int get(counter_t *c);

#endif