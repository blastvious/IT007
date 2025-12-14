/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Anh Tuan, 23521717
# File: finalLab.c
#
######################################*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_HISTORY_ITEMS 10
#define HISTORY_FILE ".it007sh_history"

pid_t pid = -1;
int sigint_count = 0;
char history[MAX_HISTORY_ITEMS][MAX_LINE];
int history_count = 0;

void add_to_history(const char *cmd)
{
    if (history_count < MAX_HISTORY_ITEMS)
    {
        strcpy(history[history_count++], cmd);
    }
    else
    {
        for (int i = 1; i < MAX_HISTORY_ITEMS; i++)
        {
            strcpy(history[i - 1], history[i]);
        }
        strcpy(history[MAX_HISTORY_ITEMS - 1], cmd);
    }
}

void print_history()
{
    printf("=== History command ===\n");
    for (int i = history_count - 1; i >= 0; i--)
    {
        printf("%s\n", history[i]);
    }
}
void on_sigint(int sigint)
{
    if(pid > 0){
        kill(pid, SIGINT);
        printf("Ctrl was pressed !!! ╰(*°▽°*)╯\n");
    }
    else{
        sigint_count++;
        if(sigint_count >= 2){
            printf("^C (❁´◡`❁)\n");
            exit(0);
        }
        else{
            printf("\nPress Ctrl+C again to exit it007sh\n");
            fflush(stdout);
        }
    }
    
}

void save_history()
{
    FILE *f = fopen(HISTORY_FILE, "w");
    if (f == NULL)
    {
        printf("Can not save history from file\n");
        return;
    }
    for (int i = 0; i < history_count; i++)
    {
        fprintf(f, "%s\n", history[i]);
    }
    fclose(f);
}
void load_history()
{
    FILE *f = fopen(HISTORY_FILE, "r");
    if (f == NULL)
    {
        return;
    }
    char line[MAX_LINE];
    history_count = 0;
    while (fgets(line, MAX_LINE, f) != NULL)
    {
        line[strcspn(line, "\n")] = 0;

        if (history_count < MAX_HISTORY_ITEMS)
        {
            strcpy(history[history_count++], line);
        }
        else
        {
            for (int i = 1; i < MAX_HISTORY_ITEMS; i++)
            {
                strcpy(history[i - 1], history[i]);
            }
            strcpy(history[MAX_HISTORY_ITEMS - 1], line);
        }
    }
    fclose(f);
}
int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    char input[MAX_LINE];
    int should_run = 1; /* flag to determine when to exit program */
    int redirect_in = 0, redirect_out = 0;
    char *input_file = NULL;
    char *out_file = NULL;
    signal(SIGINT, on_sigint);

    load_history();
    while (should_run)
    {
        printf("it007sh>");
        fflush(stdout);

        if (fgets(input, MAX_LINE, stdin) == NULL)
        {
            continue;
        }

        input[strcspn(input, "\n")] = 0;
        // Reset status
        redirect_in = 0;
        redirect_out = 0;
        input_file = NULL;
        out_file = NULL;

        if (strlen(input) == 0)
        {
            continue;
        }
        /*Execute recent command*/
        int is_HF = 0;
        if (strcmp(input, "HF") == 0)
        {
            is_HF = 1;
            if (history_count == 0)
            {
                printf("There are no command in history !!!\n");
                continue;
            }
            char recent_command[MAX_LINE];
            strcpy(recent_command, history[history_count - 1]);
            printf("Re - execute: %s\n", recent_command);
            strcpy(input, recent_command);
        }

        /*History command*/
        if (strcmp(input, "history") == 0)
        {
            print_history();
            continue;
        }

        if (!is_HF)
        {
            add_to_history(input);
        }

        if (strcmp(input, "exit") == 0)
        {
            should_run = 0;
            continue;
            // break;
        }
        /*Separate parameters to args arr*/
        char *token = strtok(input, " ");
        int index = 0;
        while (token != NULL)
        {
            args[index++] = token;
            token = strtok(NULL, " ");
        }
        args[index] = NULL;

        char *exec_args[MAX_LINE / 2 + 1];

        //Pipe-line

        int pipe_index = -1;
        for (int i = 0; args[i] != NULL; i++)
        {
            if (strcmp(args[i], "|") == 0)
            {
                pipe_index = i;
                break;
            }
        }
        
        if (pipe_index != -1)
        {
            char *left_args[MAX_LINE / 2 + 1];
            char *right_args[MAX_LINE / 2 + 1];

            int i;
            for (i = 0; i < pipe_index; i++)
                left_args[i] = args[i];
            left_args[i] = NULL;

            int j = 0;
            for (i = pipe_index + 1; args[i] != NULL; i++)
                right_args[j++] = args[i];
            right_args[j] = NULL;

            int fd[2];
            pipe(fd);

            pid_t pid1 = fork();
            if (pid1 == 0)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
                execvp(left_args[0], left_args);
                exit(1);
            }

            pid_t pid2 = fork();
            if (pid2 == 0)
            {
                dup2(fd[0], STDIN_FILENO);
                close(fd[1]);
                close(fd[0]);
                execvp(right_args[0], right_args);
                exit(1);
            }

            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            wait(NULL);
            continue;
        }

        // Out - In
        int exec_index = 0;
        for (int i = 0; args[i] != NULL; i++)
        {
            if (strcmp(args[i], "<") == 0)
            {
                redirect_in = 1;
                input_file = args[i + 1];
                i++;
            }
            else if (strcmp(args[i], ">") == 0)
            {
                redirect_out = 1;
                out_file = args[i + 1];
                i++;
            }
            else
            {
                exec_args[exec_index++] = args[i];
            }
        }

        exec_args[exec_index] = NULL;

        pid = fork();
        if (pid == 0)
        {
            if (redirect_in)
            {
                int fd = open(input_file, O_RDONLY);
                if (fd < 0)
                {
                    printf("faile to open file\n");
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            if (redirect_out)
            {
                int fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    printf("faile to open file\n");
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            execvp(exec_args[0], exec_args);
            exit(1);
        }
        else if (pid > 0)
        {
            wait(NULL);
            pid = -1;
            sigint_count = 0;
        }
    }
    save_history();
    return 0;
}