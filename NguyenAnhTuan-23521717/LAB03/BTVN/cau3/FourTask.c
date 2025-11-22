/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Anh Tuan, 23521717
# File: FourTask.c
#
######################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int loop_forever = 1;

void on_sigint()
{
    printf("\ncount.sh has stoppped!\n");
    loop_forever = 0;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, on_sigint);
    printf("Welcome to IT007, I am 23521717!\n");

    __pid_t pid;
    pid = fork();
    if (pid > 0)
    {
        
        wait(NULL);
    }
    if (pid == 0)
    {
        execl("./count.sh", "./count.sh", "120", NULL);
    }
    

    exit(0);
}