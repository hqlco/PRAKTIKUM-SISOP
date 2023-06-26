#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{

    int rows = 4;
    int cols = 5;

    int key = ftok(".", 1012003);
    int *arr;

    int shmid = shmget(key, sizeof(int) * 1000, 0666 | IPC_CREAT);

    arr = (int *)shmat(shmid, NULL, 0);

    int arr1[4][2];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            arr1[i][j] = 2;
        }
    }
    int arr2[2][5];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            arr2[i][j] = 1;
        }
    }
    int rslt[4][5];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            rslt[i][j] = 0;

            for (int k = 0; k < 2; k++)
            {
                rslt[i][j] += arr1[i][k] * arr2[k][j];
            }

            printf("%d\t", rslt[i][j]);
        }
        printf("\n");
    }
    // for (int i = 0; i < 4; i++)
    // {
    //     for (int j = 0; j < 2; j++)
    //     {
    //         // arr1[i][j] = rand() % 10 + 1;
    //         printf("%d ", arr1[i][j]);
    //     }
    //     printf("\n");
    // }
    // for (int i = 0; i < 2; i++)
    // {
    //     for (int j = 0; j < 5; j++)
    //     {
    //         // arr2[i][j] = rand() % 10 + 1;
    //         printf("%d ", arr2[i][j]);
    //     }
    //     printf("\n");
    // }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {

            int index = i * cols + j;
            arr[index] = rslt[i][j];
        }
    }
    shmdt((void *)arr);
    return 0;
}
