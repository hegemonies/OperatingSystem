#include "manifest.h"

int main(void) {
    if (mknod(FIFO, S_IFIFO | 0666, 0) < 0) {
        printf("Error create FIFO\n");
        exit(1);
    }

    int fd = open(FIFO, O_RDONLY);
    if (fd < 0) {
        printf("Error open FIFO\n");
        exit(1);
    }

    char buffer[MAXBUF];

    read(fd, buffer, MAXBUF);

    int sizeFileName = 0;
    for (int i = strlen("FILENAME~"); buffer[i] != 26; i++) {
        sizeFileName++;
    }

    // printf("size File Name = %d\n", sizeFileName);

    char fileName[sizeFileName + 1];
    strcpy(fileName, buffer + strlen("FILENAME~"));
    fileName[strlen(fileName) - 2] = 0;

    // printf("fileName = %s.\n", fileName);

    int new_fd = open(fileName, O_CREAT | O_WRONLY);
    if (new_fd < 0) {
        printf("Error create file %s\n", fileName);
        exit(1);
    }

    do {
        read(fd, &buffer[0], 1);
        if (buffer[0] == 26) {
            break;
        }
        write(new_fd, &buffer[0], 1);
    } while (1);

    close(new_fd);
    close(fd);

    if (unlink(FIFO) < 0) {
        printf("Error delete FIFO\n");
        exit(1);
    }

    return 0;
}