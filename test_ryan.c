#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char environ;

int main(void) {
    pid_t pid;
    char args[] = {"/bin/echno", "C is fun", NULL};

    pid = fork();
    if (pid == 0)
    {
        execve("/bin/echo", args, environ);
    }
    wait(NULL);
    return (0);
}
