#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

extern char **environ;
static char **commands;
static int count_commands;

int getSize(char *str)
{
    int i;
    for (i = 0; str[i] != '\0' && str[i] != '\n'; i++) {}

    return i;
}

// 1 - true; 0 - false
int isCommand(char *str)
{
    if (!str) {
        return 0;
    }

    // if (isalpha(str[0])) {
    //     return 1;
    // }
    for (int i = 0; i < count_commands; i++) {
        if (strcmp(commands[i], str) == 0) {
            return 1;
        }
    }

    return 0;
}

// 1 - true; 0 - false
int isAnd(char *str)
{
    if (!str) {
        return 0;
    }
    
    if (str[0] == '&' && str[1] == '&') {
        return 1;
    }

    return 0;
}

// 1 - true; 0 - false
int isOr(char *str)
{
    if (!str) {
        return 0;
    }
    
    if (str[0] == '|' && str[1] == '|') {
        return 1;
    }

    return 0;
}

// 1 - true; 0 - false
int isOption(char *str)
{
    if (!str) {
        return 0;
    }

    // if (str[0] == '-') {
    //     return 1;
    // }
    if (!isCommand(str)) {
        return 1;
    }

    return 0;
}

char *concatenationBin(char *str)
{
    int size = getSize(str) + 6;

    str = realloc(str, sizeof(char) * size);

    char *cstr = calloc(size, sizeof(char));

    cstr = memcpy(cstr, str, size);

    str = "/bin/";

    for (int i = 5, j = 0; i < size; i++, j++) {
        str[i] = cstr[j];
    }

    return str;
}

void get_all_commands(char *path)
{
    FILE *file = fopen(path, "r");

    commands = calloc(1, sizeof(char*));

    for (int i = 0; !feof(file); i++) {
        commands = realloc(commands, sizeof(char*) * (i + 1));
        char *tmp = calloc(30, sizeof(char));
        fscanf(file, "%s", tmp);
        int size = getSize(tmp);
        commands[i] = calloc(size, sizeof(char));
        commands[i] = memcpy(commands[i], tmp, size);
        free(tmp);
        count_commands++;
    }

    fclose(file);
}

int main(void)
{
    system("ls `echo $PATH | sed \"s/:/ /g\"`| grep -v \"^/\" > commands.txt");
    get_all_commands("commands.txt");
    printf("pid = %d\ngid = %d\nsid = %d\n", (int)getpid(), (int)getgid(), (int)getsid(0));
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

    // for (int i = 0; i < count; i++) {
    //     int size = 0;
    //     for (int j = 0; argv[i][j] != '\0' && argv[i][j] != '\n'; j++) {
    //         size++;
    //     }
    //     printf("%s. %d\n", argv[i], size);
    // }

    pid_t pid;

    if ((pid = fork()) < 0) {
        printf("Error fork: %d\n", errno);
        exit(0);
    }

    
    if (pid == 0) {
        int now_count_command = 0;
        for (int i = 0; i < count; i++) {
            char *command;
            if (isCommand(argv[i])) {
                if (now_count_command > 0) {
                    execl(argv[i - 1], command, NULL);
                    now_count_command = 0;
                }
                command = calloc(getSize(argv[i]) + 1, sizeof(char));
                command = memcpy(command, argv[i], getSize(argv[i]) + 1);
                argv[i] = concatenationBin(argv[i]);
                now_count_command++;
                printf("%s\n", argv[i]);
            } else {
                char **options = calloc(1, sizeof(char*));
                options[0] = calloc(getSize(command) + 1, sizeof(char));
                options[0] = memcpy(options[0], command, getSize(command) + 1);
                int j;
                int k;
                for (j = i, k = 1; !isCommand(argv[j]) && j < count; j++, k++) {
                    if (isAnd(argv[j]) || isOr(argv[j])) {
                        j++;
                        break;
                    } else if (isOption(argv[j])) {
                        options = realloc(options, sizeof(char*) * (k + 1));
                        options[k] = calloc(getSize(argv[j]) + 1, sizeof(char));
                        options[k] = memcpy(options[k], argv[j], getSize(argv[i]) + 1);
                    }
                }
                execv(argv[i - 1], options);
                i = j;
            }
        }

        printf("my pid = %d\n", (int)getpid());
        printf("pid = %d\n", pid);
        execv("/bin/bash", argv);
        int s;
        wait(&s);
        printf("status = %d\n", s);
        exit(0);
    } else {
        printf("!my pid = %d\n", (int)getpid());
        printf("!pid = %d\n", pid);
        int status;
        waitpid(pid, &status, WNOHANG);
        // wait(&status);
        printf("status = %d\n", status);
    }


    for (int i = 0; i < count; i++) {
        free(argv[i]);
    }
    free(argv);

    return 0;
}