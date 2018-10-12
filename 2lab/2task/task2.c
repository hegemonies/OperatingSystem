#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>

extern char **environ;

int main(void)
{
    system("export PATH=$PATH:$PWD/prettyBash");
    char *user = calloc(1, sizeof(char));
    int i;
    for (i = 0; environ[13][i + 5] != '\0' && environ[13][i + 5] != '\n'; i++) {
        user = realloc(user, sizeof(char) * (i + 2));
        user[i] = environ[13][i + 5];
    }
    user[i] = '\0';
    char *pwd = calloc(1, sizeof(char));
    for (i = 0; environ[26][i + 4] != '\0' && environ[26][i + 4] != '\n'; i++) {
        pwd = realloc(pwd, sizeof(char) * (i + 2));
        pwd[i] = environ[26][i + 4];
    }
    pwd[i] = '\0';
    // system("ps -eo pid,pgrp,sess");
    printf("%s:%s$: ", user, pwd);
    char *req = calloc(100, sizeof(char));
    fgets(req, 100, stdin);
    
    // system(req);

    char **argv = malloc(sizeof(char*) * 10);
    int count = 0;

    for (int i = 0; req[i] != '\n' && req[i] != '\0'; count++, i++) {
        argv = realloc(argv, sizeof(char*) * (count + 1));
        for (int j = 0; !isspace(req[i]) && req[i] != '\0' && req[i] != '\n'; j++, i++) {
            argv[count] = realloc(argv[count], sizeof(char) * (j + 1));
            argv[count][j] = req[i];
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%s.\n", argv[i]);
    }

    pid_t pid;

    if ((pid = fork()) < 0) {
        printf("Error fork: %d\n", errno);
        exit(1);
    }

    printf("pid = %d", pid);

    execv("/bin/bash", argv);

    printf("hey\n");

    int status;
    waitpid(pid, &status, WNOHANG);
    // if (wait(&status) == pid) {
    // if (waitpid(pid, &status, WNOHANG) == pid) {
    //     printf("[%d]\tProcess %d exited with status %d.\n", (int)getpid(), pid, WEXITSTATUS(status));
    // } else {
    //     exit(1);
    // }

    for (int i = 0; i < count; i++) {
        free(argv[i]);
    }
    free(argv);

    printf("Finish\n");

    return 0;
}