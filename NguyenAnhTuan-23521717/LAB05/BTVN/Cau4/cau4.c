/*######################################
# University of Information Technology #
# IT007 Operating System
#
# <Anh Tuan>, <23521717> #
# File: cau4.c #
######################################*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int x1, x2, x3, x4, x5, x6;

int w, v, y, z, ans;

int w_ready = 0;
int v_ready = 0;
int y_ready = 0;
int z_ready = 0;

void *ProcessA(void *arg)
{
    sleep(1);
    pthread_mutex_lock(&mutex);
    w = x1 * x2;
    w_ready = 1;
    printf("(a) w = %d\n", w);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *ProcessB(void *arg)
{
    sleep(1);
    pthread_mutex_lock(&mutex);
    v = x3 * x4;
    v_ready = 1;
    printf("(b) v = %d\n", v);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *ProcessC(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (!v_ready)
        pthread_cond_wait(&cond, &mutex);
    y = v * x5;
    printf("(c) y = %d\n", y);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *ProcessD(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (!v_ready)
        pthread_cond_wait(&cond, &mutex);
    z = v * x6;
    printf("(d) z = %d\n", z);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *ProcessE(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (!w_ready || y == 0)
        pthread_cond_wait(&cond, &mutex);
    y = w * y;
    y_ready = 1;
    printf("(e) y = %d\n", y);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *ProcessF(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (!w_ready || z == 0)
        pthread_cond_wait(&cond, &mutex);
    z = w * z;
    z_ready = 1;
    printf("(f) z = %d\n", z);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}


void *ProcessG(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (!y_ready || !z_ready)
        pthread_cond_wait(&cond, &mutex);
    ans = y + z;
    printf("(g) ans = %d\n", ans);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    srand(time(NULL));

    x1 = rand() % 10 + 1;
    x2 = rand() % 10 + 1;
    x3 = rand() % 10 + 1;
    x4 = rand() % 10 + 1;
    x5 = rand() % 10 + 1;
    x6 = rand() % 10 + 1;

    printf("x1=%d x2=%d x3=%d x4=%d x5=%d x6=%d\n\n",
           x1, x2, x3, x4, x5, x6);

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
