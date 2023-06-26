#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
long long factorial(int n)
{
    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}
int main()
{

    clock_t start, end;
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
    printf("hasil faktorial 2:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = i * cols + j;
            long long hasil = factorial(arr[index]);
            printf("%lld ", hasil);
        }
        printf("\n");
    }

    shmdt((void *)arr);

    end = clock();
    double duration = ((double)end - start) / CLOCKS_PER_SEC;

    printf("Time taken to execute in seconds : %f", duration);
    return 0;
}
