#include <stdio.h>   /* printf, perror */
#include <stdlib.h>  /* exit, malloc, free */
#include <unistd.h>  /* fork, execve, access */
#include <string.h>  /* strlen */

extern char **environ;  /* needed for execve */

int main(void)
{
    char *line;
    size_t len;
    ssize_t nread;
    pid_t pid;
    int status;

    line = NULL;
    len = 0;

    while (1) /* infinite loop → shell stays alive */
    {
        printf("$ "); /* show prompt */
        nread = getline(&line, &len, stdin); /* read input */

        if (nread == -1) /* EOF (Ctrl+D) */
        {
            printf("\n");
            break; /* exit shell */
        }

        /* remove newline at the end */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0) /* child process */
        {
            char *argv[2];
            argv[0] = line;  /* command typed */
            argv[1] = NULL;

            if (access(line, X_OK) == 0) /* executable exists */
                execve(line, argv, environ);
            else
                printf("Error: No such file or directory\n");

            exit(EXIT_FAILURE); /* if execve fails */
        }
        else /* parent process */
        {
            wait(&status);
        }
    }

    free(line);
    return 0;
}

