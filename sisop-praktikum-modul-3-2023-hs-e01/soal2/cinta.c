#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef struct
{
    int start;
    int end;
    unsigned long long result;
} FactorialArgs;

void *factorial(void *arg)
{
    FactorialArgs *args = (FactorialArgs *)arg;
    args->result = 1;

    for (int i = args->start; i <= args->end; i++)
    {
        args->result *= i;
        // printf("%d",i);
    }

    return NULL;
}

unsigned long long calculateFactorial(int n, int numThreads)
{
    pthread_t threads[numThreads];
    FactorialArgs threadArgs[numThreads];
    int numbersPerThread = n / numThreads;
    int remainder = n % numThreads;
    unsigned long long totalFactorial = 1;

    for (int i = 0; i < numThreads; i++)
    {
        // pembagian kerja
        int start = i * numbersPerThread + 1;
        int end = (i + 1) * numbersPerThread;

        if (i == numThreads - 1)
        {
            end += remainder;
        }

        threadArgs[i].start = start;
        threadArgs[i].end = end;

        pthread_create(&threads[i], NULL, factorial, &threadArgs[i]);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
        totalFactorial *= threadArgs[i].result;
    }

    return totalFactorial;
}

int main()
{
    clock_t start, end;
    // todo  sudah thread
    start = clock();
    int rows = 4;
    int cols = 5;

    int key = ftok(".", 1012003);
    int *arr;

    int shmid = shmget(key, sizeof(int) * 1000, 0666 | IPC_CREAT);

    arr = (int *)shmat(shmid, NULL, 0);
    printf("Shared array in Process 2:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = i * cols + j;
            printf("%d ", arr[index]);
        }
        printf("\n");
    }

    // for (int i = 0; i < rows; i++)
    // {
    //     for (int j = 0; j < cols; j++)
    //     {
    //         int index = i * cols + j;
    //         unsigned long long result = calculateFactorial(arr[index], 2);
    //         printf("%llu ", result);
    //     }
    //     printf("\n");
    // }
    printf("hasil faktorial 2:\n");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = i * cols + j;
            unsigned long long result = calculateFactorial(arr[index], 2);
            printf("%llu ", result);
        }
         printf("\n");
    }

    shmdt((void *)arr);

    end = clock();
    double duration = ((double)end - start) / CLOCKS_PER_SEC;

    printf("Time taken to execute in seconds : %f", duration);
    return 0;
}
