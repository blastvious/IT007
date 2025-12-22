/*######################################
# University of Information Technology #
# IT007 Operating System
#
# <Anh Tuan>, <23521717> #
# File: cau3.c #
######################################*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int x = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
void *ProcessA(void *mess)
{
    while (1)
    {

        pthread_mutex_lock(&m);
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("%d\n", x);
        pthread_mutex_unlock(&m);
    }
}

void *ProcessB(void *mess)
{
    while (1)
    {
        pthread_mutex_lock(&m);
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("%d\n", x);
        pthread_mutex_unlock(&m);
    }
}
int main()
{
    pthread_t pA, pB;
    pthread_create(&pA, NULL, &ProcessA, NULL);
    pthread_create(&pB, NULL, &ProcessB, NULL);
    pthread_join(pA, NULL);
    pthread_join(pB, NULL);
    return 0;
}