/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Anh Tuan, 23521717
# File: Consumer.c
#
######################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SIZE 10
#define SHM_NAME "OS"

int main() {
    int fd;
    char *buffer;

    fd = shm_open(SHM_NAME, O_RDWR, 0666);
    buffer = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int sum = 0;

    while (1) {
        int found = 0;
        for (int i = 0; i < SIZE; i++) {
            if (buffer[i] > 0) {
                int val = buffer[i];
                sum += val;
                printf("Consumer read %d, sum=%d\n", val, sum);
                buffer[i] = 0; 
                found = 1;
                break; 
            }
        }

        if (!found) {
            usleep(200000); 
        }

        if (sum > 100) {
            printf("Sum > 100, \n");
            buffer[0] = -1; 
            break;
        }
    }

    munmap(buffer, SIZE);
    close(fd);
    return 0;
}
