#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0)
        printf("Inside the child process\n");  // runs in child
    else if (pid > 0)
        printf("Inside the parent, child PID = %d\n", pid);  // runs in parent
    else
        printf("fork failed\n");
}

