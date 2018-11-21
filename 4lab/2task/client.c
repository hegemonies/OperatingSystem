#include "manifest.h"
#include <signal.h>
#include <pthread.h>

static int fd;
static int needStop;

void sighandler(int signo)
{
	char end = 26;
	write(fd, &end, 1);
	needStop = 1;
}

void stop()
{
	//pause();	
  	signal(SIGTSTP, sighandler);
}0

int main(void) {
	needStop = 0;
    fd = open(FIFO, O_WRONLY);
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
    
    pthread_t thread;
    pthread_create(&thread, NULL, (void *)stop, NULL);

	char answer[100];
    while (!needStop) {
    	printf("$ ");
    	scanf("%s", answer);
    	write(fd, answer, strlen(answer));
    }
    // write(fd, anyStr, strlen(anyStr));
    // write(fd, &end, 1);

    close(fd);
    
    pthread_join(thread, NULL);

    return 0;
}
