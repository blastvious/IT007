/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Anh Tuan, 23521717
# File: time.c
#
######################################*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    // Tạo shared memory
    const char *shm_name = "/time_shm";
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(struct timeval));
    struct timeval *start = mmap(NULL, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid_t pid = fork();


    if (pid == 0) {
        // Con: lưu thời điểm bắt đầu
        gettimeofday(start, NULL);
        execl("/bin/sh", "sh", "-c", argv[1], NULL);
        perror("execl");
        exit(1);
    } else {
        wait(NULL); // chờ con xong
        struct timeval end;
        gettimeofday(&end, NULL);

        double elapsed = (end.tv_sec - start->tv_sec) + (end.tv_usec - start->tv_usec)/1000000.0;
        printf("Execute time: %f s\n", elapsed);

        // Cleanup
        munmap(start, sizeof(struct timeval));
        close(fd);
        shm_unlink(shm_name);
    }

    return 0;
}
