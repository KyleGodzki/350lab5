#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    char * argv1[] = {"cat", "Makefile", 0};
    char * argv2[] = {"head", "-4", 0};
    char * argv3[] = {"wc", "-l", 0};

    setbuf(stdout, NULL);

    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
            printf("Error");
            exit(1);
    }

    pid_t c1 = fork();
    if (c1 == 0) {
            printf("In CHILD-1 (PID=%d): executing command %s ...\n", getpid(), argv1[0]);
            dup2(pipe1[1], STDOUT_FILENO);

            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            execvp(argv1[0], argv1);
            printf("Exec failed");
            exit(1);
    }

    pid_t c2 = fork();
    if (c2 == 0) {
            printf("In CHILD-2 (PID=%d): executing command %s ...\n", getpid(), argv2[0]);

            dup2(pipe1[0], STDIN_FILENO);
            dup2(pipe2[1], STDOUT_FILENO);

            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

                                                                                                                                                                                               45,0-1        Top    if (c1 == 0) {
            printf("In CHILD-1 (PID=%d): executing command %s ...\n", getpid(), argv1[0]);
            dup2(pipe1[1], STDOUT_FILENO);

            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            execvp(argv1[0], argv1);
            printf("Exec failed");
            exit(1);
    }

    pid_t c2 = fork();
    if (c2 == 0) {
            printf("In CHILD-2 (PID=%d): executing command %s ...\n", getpid(), argv2[0]);

            dup2(pipe1[0], STDIN_FILENO);
            dup2(pipe2[1], STDOUT_FILENO);

            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            execvp(argv2[0], argv2);
            printf("Exec failed");
            exit(1);
    }

    pid_t c3 = fork();
    if(c3 == 0) {
            printf("In CHILD-3 (PID=%d): executing command %s ...\n", getpid(), argv3[0]);

            dup2(pipe2[0], STDIN_FILENO);

            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            execvp(argv3[0], argv3);
            printf("Exec failed");
            exit(1);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    int status;

    waitpid(c1, &status, 0);
    printf("In PARENT (PID=%d): successfully reaped child (PID=%d)\n", getpid(), c1);

    waitpid(c2, &status, 0);
    printf("In PARENT (PID=%d): successfully reaped child (PID=%d)\n", getpid(), c2);

    waitpid(c3, &status, 0);
    printf("In PARENT (PID=%d): successfully reaped child (PID=%d)\n", getpid(), c3);

    return 0;
}


