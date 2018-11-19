#include "manifest.h"

int main(void) {
    int fd = open(FIFO, O_WRONLY);
    if (fd < 0) {
        printf("Error create FIFO\n");
        exit(1);
    }

    printf("Enter file name: ");
    char fileName[50];
    scanf("%s", fileName);

    int n = strlen(fileName) + strlen("FILENAME~");
    char *sendFileName = calloc(n, sizeof(char));

    printf("n = %d\n", n);

    strcat(sendFileName, "FILENAME~");
    strcat(sendFileName, fileName);

    char end = 26;
    strcat(sendFileName, &end);

    printf("%s\n", sendFileName);

    write(fd, sendFileName, strlen(sendFileName));

    char anyStr[] = "Hello wobydobydicoo world!";
    write(fd, anyStr, strlen(anyStr));
    write(fd, &end, 1);

    close(fd);

    return 0;
}