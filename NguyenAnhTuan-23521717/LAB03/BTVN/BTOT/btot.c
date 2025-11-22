/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Anh Tuan, 23521717
# File: btot.c
#
######################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#define MAX_BUFFER 1024

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0)
    {
        printf("Error: Input must be a positive integer.\n");
        return 1;
    }
    const char *shm_name = "shared";
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        return 1;
    }
    ftruncate(fd, MAX_BUFFER); // Do rong giai vat ly
    char *buffer = mmap(NULL, MAX_BUFFER, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buffer == MAP_FAILED)
    {
        return 1;
    }
    memset(buffer, 0, MAX_BUFFER);
    pid_t pid = fork();

    if (pid == 0)
    {
        int num = n;
        char temp[32];
        sprintf(buffer, "%d", num);
        while (num != 1)
        {
            if (num % 2 == 0)
            {
                num /= 2;
            }
            else
            {
                num = 3 * num + 1;
            }
            sprintf(temp, ", %d", num);
            strncat(buffer, temp, MAX_BUFFER - strlen(buffer) - 1);
        }
        munmap(buffer, MAX_BUFFER);
        close(fd);
        exit(0);
    }
    if(pid > 0){
         wait(NULL);

        printf("Collatz sequence starting from %d:\n%s\n", n, buffer);

        // Cleanup
        munmap(buffer, MAX_BUFFER);
        close(fd);
        shm_unlink(shm_name);
    }
    return 0;
}