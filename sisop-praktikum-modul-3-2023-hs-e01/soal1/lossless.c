#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_TREE_HT 100
// int bit[23];

struct treedecode
{
    char data;
    struct treedecode *left, *right;
};

struct treedecode *init()
{
    struct treedecode *new = (struct treedecode *)malloc(sizeof(struct treedecode));
    new->data = '$';
    new->left = NULL;
    new->right = NULL;

    return new;
}
struct MinHeapNode
{

    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

struct MinHeap
{
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

struct MinHeapNode *newNode(char data, unsigned freq)
{
    struct MinHeapNode *temp = (struct MinHeapNode *)malloc(
        sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

struct MinHeap *createMinHeap(unsigned capacity)

{

    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array = (struct MinHeapNode **)malloc(
        minHeap->capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a,
                     struct MinHeapNode **b)

{

    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx)

{

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap *minHeap)
{

    return (minHeap->size == 1);
}

struct MinHeapNode *extractMin(struct MinHeap *minHeap)

{

    struct MinHeapNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap *minHeap,
                   struct MinHeapNode *minHeapNode)

{

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
    {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}
void buildMinHeap(struct MinHeap *minHeap)

{

    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
void printArr(int arr[], int n, int x, char hufmen[101][101])
{

    int i;
    // int count = 0;
    for (i = 0; i < n; ++i)
    {
        // printf("%d", arr[i]);
        // count++;
        if (x == 82)
        {
            hufmen[16][i] = arr[i] + '0';
        }
        else if (x == 83)
        {
            hufmen[17][i] = arr[i] + '0';
        }
        else if (x == 84)
        {
            hufmen[18][i] = arr[i] + '0';
        }
        else if (x == 85)
        {
            hufmen[19][i] = arr[i] + '0';
        }
        else if (x == 87)
        {
            hufmen[20][i] = arr[i] + '0';
        }
        else if (x == 89)
        {
            hufmen[21][i] = arr[i] + '0';
        }
        else if (x == 90)
        {
            hufmen[22][i] = arr[i] + '0';
        }
        else
        {
            hufmen[x - 65][i] = arr[i] + '0';
        }
    }
    // if (x == 82)
    // {
    //     bit[16] = count;
    // }
    // else if (x == 83)
    // {
    //     bit[17] = count;
    // }
    // else if (x == 84)
    // {
    //     bit[18] = count;
    // }
    // else if (x == 85)
    // {
    //     bit[19] = count;
    // }
    // else if (x == 87)
    // {
    //     bit[20] = count;
    // }
    // else if (x == 89)
    // {
    //     bit[21] = count;
    // }
    // else if (x == 90)
    // {
    //     bit[22] = count;
    // }
    // else
    // {
    //     bit[x - 65] = count;
    // }

    // printf("\n");
}

int isLeaf(struct MinHeapNode *root)

{

    return !(root->left) && !(root->right);
}

struct MinHeap *createAndBuildMinHeap(char data[],
                                      int freq[], int size)

{

    struct MinHeap *minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

struct MinHeapNode *buildHuffmanTree(char data[],
                                     int freq[], int size)

{
    struct MinHeapNode *left, *right, *top;

    struct MinHeap *minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap))
    {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void printCodes(struct MinHeapNode *root, int arr[],
                int top, char hufmen[101][101])

{

    if (root->left)
    {

        arr[top] = 0;
        printCodes(root->left, arr, top + 1, hufmen);
    }

    if (root->right)
    {

        arr[top] = 1;
        printCodes(root->right, arr, top + 1, hufmen);
    }

    if (isLeaf(root))
    {

        // printf("%c: ", root->data);
        int coba = root->data;
        printArr(arr, top, coba, hufmen);
    }
}

void HuffmanCodes(char data[], int freq[], int size, char hufmen[101][101])

{
    struct MinHeapNode *root = buildHuffmanTree(data, freq, size);

    int arr[MAX_TREE_HT], top = 0;

    printCodes(root, arr, top, hufmen);
}

void encode(char asli[], char enc_asli[], char hufmen[101][101])
{
    FILE *in = fopen(asli, "r");
    FILE *out = fopen(enc_asli, "w");
    char ch;
    do
    {
        ch = fgetc(in);
        if (ch == EOF)
            break;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            if (ch == 'A' || ch == 'a')
            {
                fprintf(out, "%s", hufmen[0]);
            }
            else if (ch == 'B' || ch == 'b')
            {
                fprintf(out, "%s", hufmen[1]);
            }
            else if (ch == 'C' || ch == 'c')
            {
                fprintf(out, "%s", hufmen[2]);
            }
            else if (ch == 'D' || ch == 'd')
            {
                fprintf(out, "%s", hufmen[3]);
            }
            else if (ch == 'E' || ch == 'e')
            {
                fprintf(out, "%s", hufmen[4]);
            }
            else if (ch == 'F' || ch == 'f')
            {
                fprintf(out, "%s", hufmen[5]);
            }
            else if (ch == 'G' || ch == 'g')
            {
                fprintf(out, "%s", hufmen[6]);
            }
            else if (ch == 'H' || ch == 'h')
            {
                fprintf(out, "%s", hufmen[7]);
            }
            else if (ch == 'I' || ch == 'i')
            {
                fprintf(out, "%s", hufmen[8]);
            }
            else if (ch == 'J' || ch == 'j')
            {
                fprintf(out, "%s", hufmen[9]);
            }
            else if (ch == 'K' || ch == 'k')
            {
                fprintf(out, "%s", hufmen[10]);
            }
            else if (ch == 'L' || ch == 'l')
            {
                fprintf(out, "%s", hufmen[11]);
            }
            else if (ch == 'M' || ch == 'm')
            {
                fprintf(out, "%s", hufmen[12]);
            }
            else if (ch == 'N' || ch == 'n')
            {
                fprintf(out, "%s", hufmen[13]);
            }
            else if (ch == 'O' || ch == 'o')
            {
                fprintf(out, "%s", hufmen[14]);
            }
            else if (ch == 'P' || ch == 'p')
            {
                fprintf(out, "%s", hufmen[15]);
            }
            else if (ch == 'R' || ch == 'r')
            {
                fprintf(out, "%s", hufmen[16]);
            }
            else if (ch == 'S' || ch == 's')
            {
                fprintf(out, "%s", hufmen[17]);
            }
            else if (ch == 'T' || ch == 't')
            {
                fprintf(out, "%s", hufmen[18]);
            }
            else if (ch == 'U' || ch == 'u')
            {
                fprintf(out, "%s", hufmen[19]);
            }
            else if (ch == 'W' || ch == 'w')
            {
                fprintf(out, "%s", hufmen[20]);
            }
            else if (ch == 'Y' || ch == 'y')
            {
                fprintf(out, "%s", hufmen[21]);
            }
            else if (ch == 'Z' || ch == 'z')
            {
                fprintf(out, "%s", hufmen[22]);
            }
        }
        else
        {
            fprintf(out, "%c", ch);
        }
    } while (ch != EOF);
    fclose(in);
    fclose(out);
}

void decode(char enc_asli[], char dcd_asli[], char hufmen[101][101], char arr1[23], int pjg)
{
    struct treedecode *root = init();
    struct treedecode *curr;
    for (int i = 0; i < pjg; i++)
    {
        curr = root;
        for (int j = 0; j < strlen(hufmen[i]); j++)
        {
            if (hufmen[i][j] == '0')
            {
                if (curr->left == NULL)
                {
                    struct treedecode *new = init();
                    curr->left = new;
                }
                curr = curr->left;
            }
            else
            {
                if (curr->right == NULL)
                {
                    struct treedecode *new = init();
                    curr->right = new;
                }
                curr = curr->right;
            }
        }
        curr->data = arr1[i];
    }

    FILE *in = fopen(enc_asli, "r");
    FILE *out = fopen(dcd_asli, "w");
    struct treedecode *solve = root;
    char ch;
    do
    {
        ch = fgetc(in);
        if (ch == EOF)
            break;

        if (ch == '0')
            solve = solve->left;
        else if (ch == '1')
            solve = solve->right;
        else
        {
            fprintf(out, "%c", ch);
            solve = root;
            continue;
        }

        if (solve->left == NULL && solve->right == NULL)
        {
            fprintf(out, "%c", solve->data);
            solve = root;
        }
    } while (ch != EOF);
}

int main()
{
    char asli[] = "file.txt";
    char enc_asli[] = "encode.txt";
    char dcd_asli[] = "decode.txt";

    int fd1[2]; // Used to store two ends of first pipe
    int fd2[2]; // Used to store two ends of second pipe
    pid_t p;

    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Pipe1 Failed");
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe2 Failed");
        return 1;
    }

    p = fork();

    if (p < 0)
    {
        fprintf(stderr, "fork Failed");
        return 1;
    }

    else if (p > 0)
    {
        close(fd1[0]);

        printf("PARENT-HITUNG FREQ\n");
        char arr1[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                       'O', 'P', 'R', 'S', 'T', 'U', 'W', 'Y', 'Z'};
        int pjg = strlen(arr1);
        int hitung[pjg];
        for (int i = 0; i < pjg; i++)
        {
            hitung[i] = 0;
        }

        FILE *ptr = fopen(asli, "r");
        char ch;
        int index;
        do
        {
            ch = fgetc(ptr);
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            {
                if (ch == 'A' || ch == 'a')
                {
                    hitung[0]++;
                }
                else if (ch == 'B' || ch == 'b')
                {
                    hitung[1]++;
                }
                else if (ch == 'C' || ch == 'c')
                {
                    hitung[2]++;
                }
                else if (ch == 'D' || ch == 'd')
                {
                    hitung[3]++;
                }
                else if (ch == 'E' || ch == 'e')
                {
                    hitung[4]++;
                }
                else if (ch == 'F' || ch == 'f')
                {
                    hitung[5]++;
                }
                else if (ch == 'G' || ch == 'g')
                {
                    hitung[6]++;
                }
                else if (ch == 'H' || ch == 'h')
                {
                    hitung[7]++;
                }
                else if (ch == 'I' || ch == 'i')
                {
                    hitung[8]++;
                }
                else if (ch == 'J' || ch == 'j')
                {
                    hitung[9]++;
                }
                else if (ch == 'K' || ch == 'k')
                {
                    hitung[10]++;
                }
                else if (ch == 'L' || ch == 'l')
                {
                    hitung[11]++;
                }
                else if (ch == 'M' || ch == 'm')
                {
                    hitung[12]++;
                }
                else if (ch == 'N' || ch == 'n')
                {
                    hitung[13]++;
                }
                else if (ch == 'O' || ch == 'o')
                {
                    hitung[14]++;
                }
                else if (ch == 'P' || ch == 'p')
                {
                    hitung[15]++;
                }
                else if (ch == 'R' || ch == 'r')
                {
                    hitung[16]++;
                }
                else if (ch == 'S' || ch == 's')
                {
                    hitung[17]++;
                }
                else if (ch == 'T' || ch == 't')
                {
                    hitung[18]++;
                }
                else if (ch == 'U' || ch == 'u')
                {
                    hitung[19]++;
                }
                else if (ch == 'W' || ch == 'w')
                {
                    hitung[20]++;
                }
                else if (ch == 'Y' || ch == 'y')
                {
                    hitung[21]++;
                }
                else if (ch == 'Z' || ch == 'z')
                {
                    hitung[22]++;
                }
            }
        } while (ch != EOF);
        fclose(ptr);
        sleep(3);
        printf("PARENT-KIRIM FREQ KE CHILD\n");
        write(fd1[1], &pjg, sizeof(int));
        write(fd1[1], arr1, sizeof(char) * pjg + 1);
        write(fd1[1], hitung, sizeof(int) * pjg);
        sleep(3);

        close(fd1[1]);
        wait(NULL);
        close(fd2[1]);
        char hufmen[101][101];
        printf("PARENT-MENDAPAT CODE HUFMAN DARI CHILD\n");
        read(fd2[0], hufmen, sizeof(char) * 101 * 101);
        sleep(3);

        printf("PARENT-DCD\n");
        decode(enc_asli, dcd_asli, hufmen, arr1, pjg);
        sleep(3);

        printf("PARENT-MEMBANDINGKAN BITS\n");
        sleep(3);
        int awal = 0, akhir = 0;
        for (int i = 0; i < pjg; i++)
        {
            awal = awal + hitung[i] * 8;
        }
        for (int i = 0; i < pjg; i++)
        {
            akhir = akhir + hitung[i] * strlen(hufmen[i]);
        }
        printf("ASCII = %d\n", awal);
        printf("HUFFMAN = %d\n", akhir);

        printf("PARENT-HASIL HUFMAN\n");
        sleep(3);
        for (int i = 0; i < 23; i++)
        {
            printf("%c : ", arr1[i]);
            for (int j = 0; j < strlen(hufmen[i]); j++)
            {
                printf("%c", hufmen[i][j]);
            }
            printf("\n");
        }

        close(fd2[0]);
    }
    else
    {
        sleep(4);
        close(fd1[1]);
        int pjg;
        printf("CHILD-MENERIMA FREQ\n");
        read(fd1[0], &pjg, sizeof(int));
        char arr1[pjg];
        int hitung[pjg];
        read(fd1[0], arr1, sizeof(char) * pjg + 1);
        read(fd1[0], hitung, sizeof(int) * pjg);
        sleep(3);

        printf("CHILD-ENC\n");
        char hufmen[101][101];
        HuffmanCodes(arr1, hitung, pjg, hufmen);
        encode(asli, enc_asli, hufmen);
        sleep(3);

        close(fd1[0]);
        close(fd2[0]);
        printf("CHILD-MENGIRIM CODE HUFMAN\n");
        write(fd2[1], hufmen, sizeof(char) * 101 * 101);
        sleep(3);
        close(fd2[1]);
        exit(0);
    }

    return 0;
}

// referensi code hufman :
// https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
// https://www.geeksforgeeks.org/huffman-decoding/