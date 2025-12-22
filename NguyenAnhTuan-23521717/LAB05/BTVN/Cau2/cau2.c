/*######################################
# University of Information Technology #
# IT007 Operating System
#
# <Anh Tuan>, <23521717> #
# File: cau2.c #
######################################*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#define MAX 100
int a[MAX];
int i = 0, n;

sem_t empty, full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *processA(void *arg)
{
    while (1)
    {
        sem_wait(&empty);         
        pthread_mutex_lock(&mutex);
        int temp = rand() % 100;
        a[i++] = temp;
        printf("Add %d | Size = %d\n", temp, i);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);           
    }
}

void *processB(void *arg)
{
    while (1)
    {
        sem_wait(&full);          
        pthread_mutex_lock(&mutex);
        if (i <= 0)
        {
            printf("Nothing in array a\n");
        }
        else
        {
            int removed = a[--i];  
            printf("Remove %d | Size = %d\n", removed, i);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);        
     
    }
}

int main()
{
    printf("Nhap so phan tu n: ");
    scanf("%d", &n);
    sem_init(&empty, 0, n);
    sem_init(&full, 0, 0);
    pthread_t pA, pB;
    pthread_create(&pA, NULL, processA, NULL);
    pthread_create(&pB, NULL, processB, NULL);
    while (1)
    {
        /* code */
    }
    return 0;
}
