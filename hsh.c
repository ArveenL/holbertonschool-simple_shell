#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_PATH 1024

extern char **environ;

/* Get PATH environment variable manually */
int get_path_env(char *buf, size_t size)
{
    int i = 0;

    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            strncpy(buf, environ[i] + 5, size - 1);
            buf[size - 1] = '\0';
            return 1;
        }
        i++;
    }
    return 0;
}

/* Check if file exists and is executable */
int is_executable(const char *path)
{
    return access(path, X_OK) == 0;
}

/* Find full path for command */
int find_command(char *cmd, char *full_path, size_t size)
{
    char path_env[MAX_PATH];
    char *dir;
    char tmp[MAX_PATH];

    /* If command has /, use as-is */
    if (strchr(cmd, '/'))
    {
        if (is_executable(cmd))
        {
            strncpy(full_path, cmd, size - 1);
            full_path[size - 1] = '\0';
            return 1;
        }
        return 0;
    }

    if (!get_path_env(path_env, sizeof(path_env)))
        return 0;

    dir = strtok(path_env, ":");
    while (dir)
    {
        snprintf(tmp, sizeof(tmp), "%s/%s", dir, cmd);
        if (is_executable(tmp))
        {
            strncpy(full_path, tmp, size - 1);
            full_path[size - 1] = '\0';
            return 1;
        }
        dir = strtok(NULL, ":");
    }

    return 0;
}

/* Parse input line into arguments */
int parse_line(char *line, char **argv)
{
    int argc = 0;
    char *token = strtok(line, " \t\n");

    while (token && argc < MAX_ARGS - 1)
    {
        argv[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;
    return argc;
}

int main(void)
{
    char line[MAX_LINE];
    char *argv[MAX_ARGS];
    char full_path[MAX_PATH];
    pid_t pid;
    int status;

    while (1)
    {
        printf(":) ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        if (parse_line(line, argv) == 0)
            continue;

        /* Find full path, skip fork if command not found */
        if (!find_command(argv[0], full_path, sizeof(full_path)))
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            continue;
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            execve(full_path, argv, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

