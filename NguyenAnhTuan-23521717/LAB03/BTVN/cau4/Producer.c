/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Anh Tuan, 23521717
# File: Producer.c
#
######################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

#define SIZE 10
#define SHM_NAME "OS"

int main() {
    int fd;
    char *buffer;

    srand(time(NULL));

    // Tạo shared memory
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    buffer = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Khởi tạo buffer = 0
    for (int i = 0; i < SIZE; i++) buffer[i] = 0;

    while (1) {
        if (buffer[0] == -1) break; 

        int num = rand() % 10 + 11; 
        int written = 0;

  
        for (int i = 0; i < SIZE; i++) {
            if (buffer[i] == 0) {
                buffer[i] = (char)num;
                printf("Producer wrote %d at index %d\n", num, i);
                written = 1;
                break;
            }
        }

        if (!written) {
            printf("Buffer full, waiting consumer...\n");
        }

        usleep(200000000); 
    }

    munmap(buffer, SIZE);
    close(fd);
    shm_unlink(SHM_NAME);
    return 0;
}
