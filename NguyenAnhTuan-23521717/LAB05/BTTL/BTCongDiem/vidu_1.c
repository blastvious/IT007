/*######################################
# University of Information Technology #
# IT007 Operating System
#
# <Anh Tuan>, <23521717> #
# File: vidu_1.c
#
######################################*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#define NUM_THREADS 2
#define MAX_BUFFER 5
#define NUM_ITERATIONS 10
sem_t semempty; // Định nghĩa biến sem
sem_t semfull;
sem_t mutex;

int products_count = 0;
int productsTotals = 0;
int sellTotals =0;
void *Product(void *args)
{
    while (1)
    {
        sem_wait(&semempty);
        sem_wait(&mutex);
        products_count++;
        productsTotals++;
        
        sem_post(&mutex);
        sem_post(&semfull);
        usleep(100000);
    }
    
    return NULL;
}
void *Sells(void *args)
{
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        sem_wait(&semfull);
        sem_wait(&mutex);
        if (products_count > 0)
        {
            products_count--;
            sellTotals++;
        }
        sem_post(&mutex);
        sem_post(&semempty);
    }
    printf("San xuat %d", sellTotals);
}
int main()
{
    sem_init(&mutex, 0, 1);
    sem_init(&semempty, 0, MAX_BUFFER); 
    sem_init(&semfull, 0, 0);
    pthread_t product_threads, consumer_thread;

    
    pthread_create(&product_threads, NULL, Product, NULL);
    pthread_create(&consumer_thread, NULL, Sells, NULL);

    pthread_join(product_threads, NULL);
    pthread_join(consumer_thread, NULL);
    printf("\n============== KẾT QUẢ CUỐI CÙNG ==============\n");
    printf("Tổng sản phẩm được tạo: %d\n", productsTotals);
    printf("Tổng sản phẩm được bán: %d\n", sellTotals);
    printf("Sản phẩm còn lại trong kho: %d\n", products_count);
    printf("==============================================\n");
    sem_destroy(&mutex);
    sem_destroy(&semempty);
    sem_destroy(&semfull);

    return 0;
}