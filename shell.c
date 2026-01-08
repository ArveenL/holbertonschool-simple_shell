#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    char *line = NULL;
    size_t len = 0;

    while (1) {
        printf("$ ");                    // prompt
        if (getline(&line, &len, stdin) == -1) break; // EOF

        line[strcspn(line, "\n")] = 0;   // remove newline

        char *args[64];
        char *token = strtok(line, " ");
        int i = 0;
        while (token) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;                  // end with NULL

        if (args[0] == NULL) continue;   // empty input

        pid_t pid = fork();
        if (pid == 0) {                  // child
            execvp(args[0], args);       // run command using PATH
            perror("exec");              // if exec fails
            exit(1);
        } else if (pid > 0) {            // parent
            waitpid(pid, NULL, 0);       // wait for child
        } else {
            perror("fork");              // fork failed
        }
    }

    free(line);
    return 0;
}

