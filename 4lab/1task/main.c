#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

int main(void) {
    int filedes[2];

    if (pipe(filedes) < 0) { // create program channel
        printf("Error pipe\n");
        exit(1);
    }

    pid_t pid;

    if ((pid = fork()) < 0) { // create a child process
        printf("Error fork\n");
        exit(1);
    }

    if (pid == 0) {
        char buffer;
        int fd = open("passwds", O_CREAT | O_WRONLY);

        if (fd < 0) {
            printf("Error open passwds (%d)", errno);
            exit(1);
        }

        long int n;
        while ((n = read(filedes[0], &buffer, 1)) > 0) {
            if (buffer == 26) {
                break;
            } else if (buffer == 'b') {
                buffer = 'n';
            } else if (buffer == 'i') {
                buffer = 'a';
            } else if (buffer == 'n') {
                buffer = 'b';
            }
            write(fd, &buffer, 1);
        }
        // printf("[%d] n = %ld\n", pid, n);

        close(fd);

        exit(0);
    } else {
        int fd = open("/etc/passwd", O_RDONLY);

        if (fd < 0) {
            printf("Error open file /etc/passwd\n");
            exit(1);
        }

        char buffer[2500];

        long int n = read(fd, buffer, 2500);
        printf("[%d] n = %ld\n", pid, n);

        write(filedes[1], buffer, n);

        char end = 26;

        write(filedes[1], &end, 1);

        close(fd);

        int status;
        wait(&status);
    }

    return 0;
}