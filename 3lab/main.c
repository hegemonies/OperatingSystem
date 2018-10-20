#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include "hpctimer.h"

static int stlen;
static int printNeed;
static int threshold;
static int count_of_threads;
static int size;

pthread_mutex_t mutex;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

typedef struct {
    int *arr;
    int lo;
    int hi;
    int lvl;
} th_data;

void swap(int *a, int *b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void printArray(int *arr)
{
    if (!printNeed) {
        return;
    }

    for (int i = 0; i < stlen; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int partition(int *arr, int lo, int hi)
{
    int part_element = arr[lo];

    while (1) {
        while (lo < hi && part_element <= arr[hi]) {
            hi--;
        }

        if (lo >= hi) {
            break;
        }
        arr[lo++] = arr[hi];

        while (lo < hi && arr[lo] <= part_element) {
            lo++;
        }

        if (lo >= hi) {
            break;
        }
        arr[hi--] = arr[lo];
    }

    arr[hi] = part_element;
    return hi;
}

void quicksort(int *arr, int lo, int hi)
{
    // printf("последовательный алго\n");
    if (lo < hi) {
        // int p = partition(arr, lo, hi);
        int p = partition_(arr, lo, hi, lo + (hi - lo) / 2);
        // int p = arr[(lo + hi) / 2];
        if (printNeed) {
            // printf("p = %d\n", p);
        }

        quicksort(arr, lo, p - 1);
        if (printNeed) {
            // printf("Left: ");
        }
        // printArray(arr);

        quicksort(arr, p + 1, hi);
        if (printNeed) {
            // printf("Right: ");
        }
        // printArray(arr);
    }
}


void *quicksort_parallel(th_data *data)
{
    // printf("arr = %p\tlo = %d\thi = %d\tlvl = %d\n", data->arr, data->lo, data->hi, data->lvl);

    if (data->lo < data->hi) {
        int p = partition(data->arr, data->lo, data->hi);

        if ((data->hi - data->lo) >= threshold) {
            th_data dt = { data->arr, data->lo, p - 1, data->lvl++ };
            pthread_t thread;
            pthread_create(&thread, NULL, (void *)quicksort_parallel, &dt);
            count_of_threads++;

            th_data dt1 = { data->arr, p + 1, data->hi, data->lvl++ };
            quicksort_parallel(&dt1);
            pthread_join(thread, NULL);
        } else {
            quicksort(data->arr, data->lo, data->hi);
        }
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    count_of_threads = 0;

    printNeed = 0;
    srand(time(0));
    if (argc < 3) {
        printf("How to run:\n./main <length of the array> <size of threshold>\n");
        return 1;
    }

    threshold = atoi(argv[2]);

    int length = atoi(argv[1]);

    size = length;

    stlen = length;
    int *arr = calloc(length, sizeof(int));

    if (printNeed) {
        printf("Source array: ");
    }
    for (int i = 0; i < length; i++) {
        arr[i] = rand() % 1000;
    }
    printArray(arr);

    double t = 0;

    th_data data = { arr, 0, length - 1, 0};

    t -= wtime();
    quicksort_parallel(&data);
    t += wtime();

    if (printNeed) {
        printf("Sorted array: ");
    }
    printArray(arr);

    // validation of array
    for (int i = 0; i < length - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            printf("Count of threads = %d\n", count_of_threads);
            printf("Invalid\n");
            return 1;
        }
    }

    printf("Valid\n");
    printf("Elapsed time: %f sec\n", t);
    printf("Count of threads = %d\n", count_of_threads);

    pthread_mutex_destroy(&mutex);

    return 0;
}