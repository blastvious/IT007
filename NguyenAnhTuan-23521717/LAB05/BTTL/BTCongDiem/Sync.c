/*######################################
# University of Information Technology #
# IT007 Operating System
#
# <Anh Tuan >, <23521717> #
# File: Sync.c #
######################################*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int v_ready = 0;
int w_ready = 0;
int y_ready = 0;
int z_ready = 0;

/* A: w */
void *ProcessA(void *arg){
    sleep(2);   // tính toán
    pthread_mutex_lock(&mutex);
    w_ready = 1;
    printf("ProcessA done !!!\n");
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/* B: v */
void *ProcessB(void *arg){
    sleep(2);
    pthread_mutex_lock(&mutex);
    v_ready = 1;
    printf("ProcessB done !!!\n");
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/* C: sau v */
void *ProcessC(void *arg){
    pthread_mutex_lock(&mutex);
    while (!v_ready)
        pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    sleep(2);
    printf("ProcessC done !!!\n");

    y_ready = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/* D: sau v */
void *ProcessD(void *arg){
    pthread_mutex_lock(&mutex);
    while (!v_ready)
        pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    sleep(2);
    printf("ProcessD done !!!\n");

    pthread_mutex_lock(&mutex);
    z_ready = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/* E: sau w và y */
void *ProcessE(void *arg){
    pthread_mutex_lock(&mutex);
    while (!w_ready || !y_ready)
        pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    sleep(2);
    printf("ProcessE done !!!\n");
    return NULL;
}

/* F: sau w và z */
void *ProcessF(void *arg){
    pthread_mutex_lock(&mutex);
    while (!w_ready || !z_ready)
        pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    sleep(2);
    printf("ProcessF done !!!\n");
    return NULL;
}

/* G: sau y và z */
void *ProcessG(void *arg){
    pthread_mutex_lock(&mutex);
    while (!y_ready || !z_ready)
        pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    sleep(2);
    printf("ProcessG done !!!\n");
    return NULL;
}

int main()
{
    pthread_t ta, tb, tc, td, te, tf, tg;

    pthread_create(&ta, NULL, ProcessA, NULL);
    pthread_create(&tb, NULL, ProcessB, NULL);
    pthread_create(&tc, NULL, ProcessC, NULL);
    pthread_create(&td, NULL, ProcessD, NULL);
    pthread_create(&te, NULL, ProcessE, NULL);
    pthread_create(&tf, NULL, ProcessF, NULL);
    pthread_create(&tg, NULL, ProcessG, NULL);

    pthread_join(ta, NULL);
    pthread_join(tb, NULL);
    pthread_join(tc, NULL);
    pthread_join(td, NULL);
    pthread_join(te, NULL);
    pthread_join(tf, NULL);
    pthread_join(tg, NULL);
    return 0;
}