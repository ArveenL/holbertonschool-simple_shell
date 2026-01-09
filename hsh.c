#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_PATH 1024

extern char **environ;

/* ------------------------- */
/* Get PATH manually          */
/* ------------------------- */
int get_path_env(char *buf, size_t size)
{
    int i, j;
    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            for (j = 0; environ[i][5 + j] && j < (int)(size - 1); j++)
                buf[j] = environ[i][5 + j];
            buf[j] = '\0';
            return 1;
        }
    }
    return 0;
}

/* ------------------------- */
/* Check if file is executable */
/* ------------------------- */
int is_executable(const char *path)
{
    return access(path, X_OK) == 0;
}

/* ------------------------- */
/* Build full path: dir/cmd   */
/* ------------------------- */
void build_path(char *dest, const char *dir, const char *cmd)
{
    int i = 0, j = 0;

    while (dir[i])
    {
        dest[i] = dir[i];
        i++;
    }
    dest[i++] = '/';

    while (cmd[j])
    {
        dest[i] = cmd[j];
        i++;
        j++;
    }
    dest[i] = '\0';
}

/* ------------------------- */
/* Find command in PATH       */
/* ------------------------- */
int find_command(char *cmd, char *full_path, size_t size)
{
    char path_env[MAX_PATH];
    char tmp[MAX_PATH];
    char *dir;

    /* If command contains /, treat as path */
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

    /* Get PATH manually */
    if (!get_path_env(path_env, sizeof(path_env)))
        return 0;

    /* Iterate directories in PATH */
    dir = strtok(path_env, ":");
    while (dir)
    {
        build_path(tmp, dir, cmd);
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

/* ------------------------- */
/* Parse line into argv array */
/* ------------------------- */
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

/* ------------------------- */
/* Main shell loop            */
/* ------------------------- */
int main(void)
{
    char line[MAX_LINE];
    char *argv[MAX_ARGS];
    char full_path[MAX_PATH];
    ssize_t nread;
    pid_t pid;
    int status;

    while (1)
    {
        write(1, ":) ", 3);  /* Prompt */

        nread = read(0, line, sizeof(line) - 1);
        if (nread <= 0)
            break;

        line[nread] = '\0';

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        if (parse_line(line, argv) == 0)
            continue;

        /* Find command in PATH */
        if (!find_command(argv[0], full_path, sizeof(full_path)))
        {
            write(2, "command not found\n", 18);
            continue;
        }

        /* Fork only if command exists */
        pid = fork();
        if (pid < 0)
            exit(1);

        if (pid == 0)
        {
            execve(full_path, argv, environ);
            write(2, "execve failed\n", 14);
            exit(1);
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

