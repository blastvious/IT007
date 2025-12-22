/*######################################
# University of Information Technology #
# IT007 Operating System
#
# <Anh Tuan>, <23521717> #
# File: cau1.c #
######################################*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<semaphore.h>


int sells = 0, products = 0;
sem_t empty , full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *Producer(void *mess){
    while (1)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        products++;
        printf("Product %d\n", products);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    
}


void *Consumer(void *mess){
    while (1)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        sells++;
        printf("Sells %d\n", sells);
        printf("Sells + 1717 %d\n", sells + 1717);
        pthread_mutex_unlock(&mutex);  
        sem_post(&empty);
    }
    
}
int main()
{
    sem_init(&full, 0,0);
    sem_init(&empty,0, sells + 1717);

    pthread_t pA,pB;
    pthread_create(&pA,NULL,&Consumer,NULL);
    pthread_create(&pB,NULL,&Producer,NULL);
    
    while (1)
    {
        
    }
    return 0;
}