#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    char * argv1[] = {"cat", "Makefile", 0};
    //char * argv2[] = {"head", "-4", 0};
    char * argv2[] = {"wc", "-l", 0};

    setbuf(stdout, NULL);

    int fd[2];

    if (pipe(fd) == -1) {
            printf("Error");
            exit(1);
    }

    pid_t c1 = fork();

    if (c1 < 0) {
            printf("Error");
            exit(1);
    }

    if (c1 == 0) {
        printf("In CHILD-1 (PID=%d): executing command %s ...\n", getpid(), argv1[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(argv1[0], argv1);

        printf("Exec failed");
        exit(1);

    }

    pid_t c2 = fork();

    if (c2 < 0) {
            printf("Error");
            exit(1);
    }

    if (c2 == 0) {
        printf("In CHILD-2 (PID=%d): executing command %s ...\n", getpid(), argv2[0]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        execvp(argv2[0], argv2);

        printf("Exec failed");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);
    
    int status;
    pid_t pid1, pid2;

    pid1 = wait(&status);
    pid2 = wait(&status);

    printf("In PARENT (PID=%d): successfully reaped child (PID=%d)\n", getpid(), pid1);
    printf("In PARENT (PID=%d): successfully reaped child (PID=%d)\n", getpid(), pid2);

    return 0;
}
