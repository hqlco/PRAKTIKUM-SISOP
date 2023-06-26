# sisop-praktikum-modul-3-2023-HS-E01

| Nama                      | NRP           |Username           |
|---------------------------|---------------|--------------|
|Moh Rosy Haqqy Aminy       |5025211012     |rosyhaqqy          |
|Rule Lulu Damara           |5025211050     |rulelulu           |
|Rizky Alifiyah Rahma       |5025211209     |ikikk              |


## Soal 1
Lord Maharaja Baginda El Capitano Harry Maguire, S.Or., S.Kom yang dulunya seorang pemain bola, sekarang sudah pensiun dini karena sering blunder dan merugikan timnya. Ia memutuskan berhenti dan beralih profesi menjadi seorang programmer. Layaknya bola di permainan sepak bola yang perlu dikelola, pada dunia programming pun perlu adanya pengelolaan memori. Maguire berpikir bahwa semakin kecil file akan semakin mudah untuk dikelola dan transfer file-nya juga semakin cepat dan mudah. Dia lantas menemukan Algoritma Huffman untuk proses kompresi lossless. Dengan kepintaran yang pas-pasan dan berbekal modul Sisop, dia berpikir membuat program untuk mengkompres sebuah file. Namun, dia tidak mampu mengerjakannya sendirian, maka bantulah Maguire membuat program tersebut!
(Wajib menerapkan konsep pipes dan fork seperti yang dijelaskan di modul Sisop. Gunakan 2 pipes dengan diagram seperti di modul 3).

a. Pada parent process, baca file yang akan dikompresi dan hitung frekuensi kemunculan huruf pada file tersebut. Kirim hasil perhitungan frekuensi tiap huruf ke child process.

b. Pada child process, lakukan kompresi file dengan menggunakan algoritma Huffman berdasarkan jumlah frekuensi setiap huruf yang telah diterima.

c. Kemudian (pada child process), simpan Huffman tree pada file terkompresi. Untuk setiap huruf pada file, ubah karakter tersebut menjadi kode Huffman dan kirimkan kode tersebut ke program dekompresi menggunakan pipe.

d. Kirim hasil kompresi ke parent process. Lalu, di parent process baca Huffman tree dari file terkompresi. Baca kode Huffman dan lakukan dekompresi.

e. Di parent process, hitung jumlah bit setelah dilakukan kompresi menggunakan algoritma Huffman dan tampilkan pada layar perbandingan jumlah bit antara setelah dan sebelum dikompres dengan algoritma Huffman.

	Catatan:
    - Pada encoding ASCII, setiap karakter diwakili oleh 8 bit atau 1 byte, yang cukup untuk merepresentasikan 256 karakter yang berbeda, contoh: 
        - Huruf A	: 01000001
        - Huruf a	: 01100001
    - Untuk karakter selain huruf tidak masuk ke perhitungan jumlah bit dan tidak perlu dihitung frekuensi kemunculannya
    - Agar lebih mudah, ubah semua huruf kecil ke huruf kapital

## Jawaban

```bash
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

```
**Penjelasan:**

Langkah-langkah untuk membuat Huffman Tree

- Buat node untuk setiap karakter unik dan buat min heap untuk semua node
- Ekstrak dua node dengan frekuensi minimum dari min heap
- Buat node internal baru dengan frekuensi sama dengan jumlah frekuensi dua node. Jadikan node yang diekstraksi pertama sebagai anak kirinya dan node yang diekstraksi lainnya sebagai anak kanannya. Tambahkan node ini ke min heap
- Ulangi langkah #2 dan #3 hingga heap hanya berisi satu node. Node yang tersisa adalah root node dan tree selesai

### pengunaan pipe
```bash
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

```

### 1.a
```bash
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
```
**penjelasan**

menghitung frekuensi dari setiap huruf yang ada di dalam file.txt

### 1.b
```bash
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
```
**penjelasan**

- menerima frekuensi dari parent dan mengencode file.txt
- melakukan kompresi file dengan menggunakan algoritma Huffman

### 1.c
```bash
        close(fd1[0]);
        close(fd2[0]);
        printf("CHILD-MENGIRIM CODE HUFMAN\n");
        write(fd2[1], hufmen, sizeof(char) * 101 * 101);
        sleep(3);
        close(fd2[1]);
```
**penjelasan**

mengirimkan hasil encode ke parent

### 1.d
```bash
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

```

**penjelasan**

menerima hasil encode dari child dan melakukan decode

### 1.e
```bash
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

```

**penjelasan**

- membandingkan jumlah bit dari file.txt dengan hasil encode
- menampilkan hasil encode

**ASCII = 4840**

**HUFFMAN = 2458**




## Dokumentasi Soal 1

![Screenshot 2023-05-11 123144](https://github.com/hqlco/sampah/assets/86828535/b73f4a1a-db0d-412c-baea-768538c87d92)


## Soal 2
Fajar sedang sad karena tidak lulus dalam mata kuliah Aljabar Linear. Selain itu, dia tambah sedih lagi karena bertemu matematika di kuliah Sistem Operasi seperti kalian 🥲. Karena dia tidak bisa ngoding dan tidak bisa menghitung, jadi Fajar memohon jokian kalian. Tugas Fajar adalah sebagai berikut.

a. Membuat program C dengan nama kalian.c, yang berisi program untuk melakukan perkalian matriks. Ukuran matriks pertama adalah 4×2 dan matriks kedua 2×5. Isi dari matriks didefinisikan di dalam code. Matriks nantinya akan berisi angka random dengan rentang pada matriks pertama adalah 1-5 (inklusif), dan rentang pada matriks kedua adalah 1-4 (inklusif). Tampilkan matriks hasil perkalian tadi ke layar.

b. Buatlah program C kedua dengan nama cinta.c. Program ini akan mengambil variabel hasil perkalian matriks dari program kalian.c (program sebelumnya). Tampilkan hasil matriks tersebut ke layar.
(Catatan: wajib menerapkan konsep shared memory)

c. Setelah ditampilkan, berikutnya untuk setiap angka dari matriks tersebut, carilah nilai faktorialnya. Tampilkan hasilnya ke layar dengan format seperti matriks.
Contoh:

    array [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], ...],

    maka:
    
    1 2 6 24 120 720 ... ... …

(Catatan: Wajib menerapkan thread dan multithreading dalam penghitungan faktorial)

d. Buatlah program C ketiga dengan nama sisop.c. Pada program ini, lakukan apa yang telah kamu lakukan seperti pada cinta.c namun tanpa menggunakan thread dan multithreading. Buat dan modifikasi sedemikian rupa sehingga kamu bisa menunjukkan perbedaan atau perbandingan (hasil dan performa) antara program dengan multithread dengan yang tidak.
Dokumentasikan dan sampaikan saat demo dan laporan resmi.

## Jawaban Soal 2

### kalian.c
```bash
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
            arr1[i][j] = rand() % 2 + 1;
        }
    }
    int arr2[2][5];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            arr2[i][j] = rand() % 3 + 1;
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

```
**penjelasan**

- `int rows = 4;` dan `int cols = 5;` digunakan untuk mendeklarasikan jumlah baris dan kolom pada matriks
- `int key = ftok(".", 1012003);` digunakan untuk membuat key yang akan digunakan untuk mengakses shared memory
- `int *arr;` digunakan untuk mendeklarasikan variabel pointer yang akan digunakan untuk mengakses shared memory
- `int shmid = shmget(key, sizeof(int) * 1000, 0666 | IPC_CREAT);` digunakan untuk membuat shared memory dengan key yang telah dibuat sebelumnya
- `arr = (int *)shmat(shmid, NULL, 0);` digunakan untuk mengakses shared memory yang telah dibuat sebelumnya
- `int arr1[4][2];` digunakan untuk mendeklarasikan matriks pertama dengan ukuran 4x2
- `for (int i = 0; i < 4; i++)` digunakan untuk melakukan perulangan sebanyak 4 kali untuk mengisi matriks pertama dengan angka random dari 1-2
- `int arr2[2][5];` digunakan untuk mendeklarasikan matriks kedua dengan ukuran 2x5
- `for (int i = 0; i < 2; i++)` digunakan untuk melakukan perulangan sebanyak 2 kali untuk mengisi matriks kedua dengan angka random dari 1-3
- `int rslt[4][5];` digunakan untuk mendeklarasikan matriks hasil perkalian dengan ukuran 4x5
- `for (int i = 0; i < 4; i++)` digunakan untuk melakukan perulangan sebanyak 4 kali untuk mengisi matriks hasil perkalian dengan hasil perkalian dari matriks pertama dan kedua
- `for (int i = 0; i < rows; i++)` digunakan untuk melakukan perulangan sebanyak jumlah baris matriks
- `int index = i * cols + j;` digunakan untuk mendeklarasikan index yang akan digunakan untuk mengakses shared memory
- `arr[index] = rslt[i][j];` digunakan untuk mengisi shared memory dengan hasil perkalian matriks
- `shmdt((void *)arr);` digunakan untuk melepaskan shared memory yang telah digunakan
- `return 0;` digunakan untuk mengembalikan nilai 0


### cinta.c
```bash
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

```
**penjelasan**
- `int rows = 4;` digunakan untuk mendeklarasikan jumlah baris matriks
- `int cols = 5;` digunakan untuk mendeklarasikan jumlah kolom matriks
- `int key = ftok(".", 1012003);` digunakan untuk mendeklarasikan key untuk shared memory
- `int *arr;` digunakan untuk mendeklarasikan shared memory
- `int shmid = shmget(key, sizeof(int) * 1000, 0666 | IPC_CREAT);` digunakan untuk mendeklarasikan shared memory dengan ukuran 1000 byte
- `arr = (int *)shmat(shmid, NULL, 0);` digunakan untuk mengakses shared memory 
- Struct `FactorialArgs`:
    - Struct ini digunakan untuk menyimpan argumen yang akan digunakan dalam fungsi faktorial pada setiap thread.
    - Struct ini memiliki tiga anggota: `start` untuk menandakan bilangan awal, `end` untuk menandakan bilangan akhir, dan `result` untuk menyimpan hasil faktorial.
- Fungsi `factorial(void *arg)`:
    - Fungsi ini adalah fungsi yang akan dijalankan oleh setiap thread.
    - Mengambil argumen dalam bentuk pointer ke `FactorialArgs`.
    - Menginisialisasi `result` menjadi 1.
    - Melakukan perulangan dari `start` hingga `end` dan mengalikan setiap bilangan dengan `result`.
    - Setelah selesai, mengembalikan `NULL`.
- Fungsi `calculateFactorial(int n, int numThreads)`:
    - Fungsi ini adalah fungsi utama untuk menghitung faktorial dengan menggunakan thread.
    - Membuat array of `pthread_t` dengan ukuran sesuai dengan `numThreads` untuk menyimpan thread yang akan dibuat.
    - Membuat array of `FactorialArgs` dengan ukuran sesuai dengan `numThreads` untuk menyimpan argumen yang akan digunakan pada setiap thread.
    - Menghitung jumlah bilangan yang akan ditangani oleh setiap thread dengan membagi `n` dengan `numThreads`.
    - Menyimpan sisa pembagian dalam variabel `remainder` untuk memastikan semua bilangan diproses dengan adil oleh thread-thread yang ada.
    - Menginisialisasi variabel `totalFactorial` dengan 1, yang akan digunakan untuk menyimpan hasil faktorial keseluruhan.
    - Melakukan loop sebanyak `numThreads` untuk membuat thread-thread dan membagi tugas perhitungan faktorial.
        - Menghitung `start` dan `end` untuk setiap thread berdasarkan indeks dan jumlah bilangan yang ditangani oleh setiap thread.
        - Jika ini adalah thread terakhir, menambahkan `remainder` ke `end`.
        - Menyimpan `start` dan `end` dalam argumen threadArgs.
        - Membuat thread baru menggunakan `pthread_create` dengan fungsi faktorial sebagai argumen dan argumen threadArgs.
    - Melakukan loop sebanyak `numThreads` untuk menunggu setiap thread selesai dan mengumpulkan hasil faktorial masing-masing thread ke `totalFactorial`.
    - Mengembalikan `totalFactorial`.
- Dengan menggunakan thread, perhitungan faktorial dapat dilakukan secara paralel dengan membagi tugas perhitungan ke beberapa thread. Setiap thread akan menghitung faktorial dari bilangan-bilangan yang ditentukan dan hasilnya akan dikumpulkan dan dikalikan untuk mendapatkan hasil faktorial keseluruhan.
### sisop.c
```bash
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

```
**penjelasan**
- `int rows = 4;` digunakan untuk mendeklarasikan jumlah baris matriks
- `int cols = 5;` digunakan untuk mendeklarasikan jumlah kolom matriks
- `int key = ftok(".", 1012003);` digunakan untuk mendeklarasikan key untuk shared memory
- `int *arr;` digunakan untuk mendeklarasikan shared memory
- `int shmid = shmget(key, sizeof(int) * 1000, 0666 | IPC_CREAT);` digunakan untuk mendeklarasikan shared memory dengan ukuran 1000 byte
- `arr = (int *)shmat(shmid, NULL, 0);` digunakan untuk mengakses shared memory
- `printf("Shared array in Process 2:\n");` digunakan untuk menampilkan array yang ada di proses 2
- `printf("hasil faktorial 2:\n");` digunakan untuk menampilkan hasil faktorial dari array yang ada di proses 2
- `shmdt((void *)arr);` digunakan untuk menghapus shared memory

### perbandingan nomer 2
![Screenshot 2023-05-06 091416](https://user-images.githubusercontent.com/86828535/236592582-52195adb-1fa0-4f9b-a3e7-188f8c5e7c9c.png)

Dari gambar tersebut dapat dilihat bahwa waktu yang dibutuhkan untuk menjalankan program dengan menggunakan thread lebih lambat dibandingkan dengan program yang tidak menggunakan thread akan tetapi memiliki konsumsi CPU yang lebih sedikit(dapat dilihat dari gambar tersebut)

### dokumentasi nomer 2

![Screenshot 2023-05-06 091416](https://user-images.githubusercontent.com/86828535/236592582-52195adb-1fa0-4f9b-a3e7-188f8c5e7c9c.png)

## Soal3
#### 3.	
Elshe saat ini ingin membangun usaha sistem untuk melakukan stream lagu. Namun, Elshe tidak paham harus mulai dari mana.
#### a.	
Bantulah Elshe untuk membuat sistem stream (receiver) stream.c dengan user (multiple sender dengan identifier) user.c menggunakan message queue (wajib). Dalam hal ini, user hanya dapat mengirimkan perintah berupa STRING ke sistem dan semua aktivitas sesuai perintah akan dikerjakan oleh sistem.

## Jawaban a
###	User.c
```bash
struct message_buffer {
    long message_type;
    char message_text[MAX_MESSAGE_SIZE];
    pid_t pid;
} message;
```
**penjelasan**
-	Dibuat sebuah variabel message dari struct `message_buffer`. Variabel ini akan digunakan untuk menyimpan data pesan yang akan dikirim atau diterima.
```bash
int main()
{
    key_t key; 
    pid_t pid; 
    int msgid;
    char cmd[MAX_MESSAGE_SIZE];
    key = ftok("songfile", 'R'); 
    msgid = msgget(key, 0666 | IPC_CREAT);
    pid =  getpid();
    	
    while(1) {
    	printf("Enter command : ");
    	fgets(cmd,MAX_MESSAGE_SIZE,stdin);
    	cmd[strcspn(cmd, "\n")] = '\0'; 
    	message.pid = pid; 
    	message.message_type = 1;
    	strcpy(message.message_text, cmd); 
    	msgsnd(msgid, &message, sizeof(message), 0);
    }
    return 0;
}
```
**penjelasan**
-	`ftok()` digunakan untuk generate unik key yang akan digunakan untuk message queue
-	Menggunakan fungsi `msgget()` untuk membuat atau mendapatkan message queue dengan menggunakan key yang telah dibuat sebelumnya. Fungsi ini mengembalikan message queue ID (`msgid`).
-	Mendapatkan pid dari proses saat ini menggunakan fungsi `getpid()`. Pid digunakan untuk memberikan id pada user 
-	Dilakukan looping untuk menerima command yang diinputkan oleh user menggunakan `fgets()`
-	Menghapus karakter newline (\n) dari cmd dengan menggunakan fungsi strcspn dan menggantinya dengan karakter null (\0).
-	`message.pid = pid`, Memasukkan pid kedalam message.pid yang akan digunakan sebagai userid
-	`msgsnd ()` digunakan untuk Mengirim pesan ke message queue. Pesan yang dikirim berisi pid pengirim dan command yang dimasukkan oleh user.

### Stream.c
```bash
msgrcv(msgid, &message, sizeof(message), 0, 0);
printf("\nReceived message: %s \n", message.message_text);
msgctl(msgid, IPC_RMID, NULL);
```
**penjelasan**
-	Dalam stream.c  juga dilakukan deklarasi struct `message_buffer` dan `msgid` terlebih dahulu
-	`msgrcv()` : digunakan untuk mengambil pesan-pesan yang ada dalam sebuah message queue. 
-	`msgctl()` : digunakan untuk menghapus atau menghancurkan message queue.

#### b.	
User pertama kali akan mengirimkan perintah DECRYPT kemudian sistem stream akan melakukan decrypt/decode/konversi pada file song-playlist.json (dapat diunduh manual saja melalui link berikut) sesuai metodenya dan meng-output-kannya menjadi playlist.txt diurutkan menurut alfabet.
Proses decrypt dilakukan oleh program stream.c tanpa menggunakan koneksi socket sehingga struktur direktorinya adalah sebagai berikut:
```text
└── soal3
	├── playlist.txt
	├── song-playlist.json
	├── stream.c
	└── user.c
```
## Jawaban b
### Stream.c
```bash
void decrypt(const char* filepath, const char* outputpath) {
	int numSongs;

    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    
    char* decryptedString;
    char* fileContents = NULL;
    size_t fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);
    
    fileContents = (char*)malloc(fileSize * sizeof(char));
    fread(fileContents, sizeof(char), fileSize, file);
    fclose(file);
    
    struct json_object* json = json_tokener_parse(fileContents);
    free(fileContents);
    
    
    if (!json_object_is_type(json, json_type_array)) {
        printf("Invalid JSON format\n");
        json_object_put(json);
        return;
    }
  
    numSongs = json_object_array_length(json);
    
    FILE* output_file = fopen(outputpath, "w");
    if (output_file == NULL) {
        json_object_put(json);
        return;
    }

    for (int i = 0; i < numSongs; i++) {
        struct json_object* songObj = json_object_array_get_idx(json, i);
        struct json_object* methodObj;
        struct json_object* songStringObj;

        if (json_object_object_get_ex(songObj, "method", &methodObj) &&
            json_object_object_get_ex(songObj, "song", &songStringObj)) {
            const char* method = json_object_get_string(methodObj);
            const char* songString = json_object_get_string(songStringObj);
            if (strcmp(method, "hex") == 0) {
                int len = strlen(songString);
				decryptedString = malloc((len / 2) + 1);

				for (int i = 0; i < len; i += 2) {
				char byte[3];
				byte[0] = songString[i];
				byte[1] = songString[i + 1];
				byte[2] = '\0';

				decryptedString[i / 2] = strtol(byte, NULL, 16);
				}
				decryptedString[len / 2] = '\0';
				fprintf(output_file, "%s\n", decryptedString);
				free(decryptedString);
            
            } else if (strcmp(method, "base64") == 0) {
				const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

				int inputLen = strlen(songString);

				int outputLen = (inputLen / 4) * 3;
				if (songString[inputLen - 1] == '=') {
				outputLen--;
				if (songString[inputLen - 2] == '=')
					outputLen--;
				}

				decryptedString = malloc(outputLen + 1);
				if (decryptedString == NULL) {
				printf("Memory allocation failed\n");
				return;
				}

				int i, j, k;
				unsigned char a, b, c, d;
				for (i = 0, j = 0; i < inputLen; i += 4, j += 3) {
				a = strchr(base64chars, songString[i]) - base64chars;
				b = strchr(base64chars, songString[i + 1]) - base64chars;
				c = strchr(base64chars, songString[i + 2]) - base64chars;
				d = strchr(base64chars, songString[i + 3]) - base64chars;

				decryptedString[j] = (a << 2) | (b >> 4);
				if (songString[i + 2] != '=')
					decryptedString[j + 1] = (b << 4) | (c >> 2);
				if (songString[i + 3] != '=')
					decryptedString[j + 2] = (c << 6) | d;
				}

				decryptedString[outputLen] = '\0';
				fprintf(output_file, "%s\n", decryptedString);
				free(decryptedString);
            } else if (strcmp(method, "rot13") == 0) {
                int len = strlen(songString);
			 	decryptedString = malloc(len + 1);

				for (int i = 0; i < len; i++) {
				char c = songString[i];

				if (c >= 'a' && c <= 'z') {
					decryptedString[i] = (((c - 'a') + 13) % 26) + 'a';
				} else if (c >= 'A' && c <= 'Z') {
					decryptedString[i] = (((c - 'A') + 13) % 26) + 'A';
				} else {
					decryptedString[i] = c;
				}
				}

				decryptedString[len] = '\0';
				fprintf(output_file, "%s\n", decryptedString);
				free(decryptedString);
					
			} else {
				printf("Metode dekripsi tidak diketahui\n");
			}
        }
    }

    fclose(output_file);
    json_object_put(json);
    printf("SONG DECRYPTED\n");
}
```
**penjelasan**
-	Dibuat sebuah fungsi Bernama ```decrypt()```, file dibaca dan disimpan dalam pointer `fileContents()`
-	FileContents di-parse menggunakan fungsi `json_tokener_parse` dari library `json-c`
-	Jumlah lagu akan dihitung menggunakan `json_object_array_length` dan disimpan ke variabel `numSongs`.
-	File output kemudian dibuka menggunakan fungsi `fopen` dengan mode `"w"`.
-	Untuk setiap lagu, objek lagu diambil menggunakan `json_object_array_get_idx`. Kemudian objek method dan objek string lagu diambil menggunakan `json_object_object_get_ex`. Jika berhasil diambil, nilai method dan string lagu diambil menggunakan `json_object_get_string`
-	Kemudian, nilai dari method di-compare dengan "hex", "base64", atau "rot13". Jika method tidak dikenali, maka fungsi akan menampilkan pesan error.
-	Jika method adalah "hex", maka nilai string lagu akan diubah dari format hexadesimal ke ASCII menggunakan `strtol`. Setelah itu, nilai tersebut di-write ke file output dan memory yang digunakan oleh `decryptedString` di-release menggunakan free.
-	Jika method adalah "base64", maka nilai string lagu akan diubah dari format base64 ke ASCII menggunakan proses decoding. Setelah itu, nilai tersebut di-write ke file output dan memory yang digunakan oleh `decryptedString` di-release menggunakan free.
-	Jika method adalah "rot13", maka nilai string lagu akan diubah menggunakan teknik ROT13. Setelah itu, nilai tersebut di-write ke file output dan memory yang digunakan oleh `decryptedString` di-release menggunakan free.
-	Setelah semua lagu selesai diproses, file output ditutup menggunakan `fclose`, memory yang digunakan oleh json di-release menggunakan `json_object_put`
-	Selanjutnya akan dipanggil fungsi `sortPlaylist()` yang akan digunakan untuk mengurutkan songs didalam playlist.txt

#### c.	
Selain itu, user dapat mengirimkan perintah LIST, kemudian sistem stream akan menampilkan daftar lagu yang telah di-decrypt

## Jawaban c
```bash
void listSong(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filepath);
        return;
    }

    char line[256];
    printf("LIST DECRYPTED SONG:\n");
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  // Remove trailing newline character
        printf("%s\n", line);
    }

    fclose(file);
}
```
**penjelasan**
-	Fungsi listSong digunakan untuk membaca file `playlist.txt` menampilkannya dilayar
-	Pertama-tama  file dibuka lalu fungsi akan membaca file baris per baris dengan menggunakan fungsi `fgets()` dan menyimpannya di variabel line
-	Setiap baris yang dibaca akan dihapus karakter newline di akhir barisnya dengan menggunakan fungsi `strcspn()`.
-	Lalu setiap baris yang telah dihapus karakter newline-nya akan ditampilkan di layar menggunakan fungsi printf(). 
-	Setelah seluruh isi file selesai dibaca, file akan ditutup dengan menggunakan fungsi `fclose()`.

#### d.	
User juga dapat mengirimkan perintah PLAY <SONG> dengan ketentuan sebagai berikut.
```text
PLAY "Stereo Heart" 
    sistem akan menampilkan: 
    USER <USER_ID> PLAYING "GYM CLASS HEROES - STEREO HEART"
PLAY "BREAK"
    sistem akan menampilkan:
    THERE ARE "N" SONG CONTAINING "BREAK":
    1. THE SCRIPT - BREAKEVEN
    2. ARIANA GRANDE - BREAK FREE
dengan “N” merupakan banyaknya lagu yang sesuai dengan string query. Untuk contoh di atas berarti THERE ARE "2" SONG CONTAINING "BREAK":
PLAY "UVUWEVWEVWVE"
    THERE IS NO SONG CONTAINING "UVUVWEVWEVWE"
Untuk mempermudah dan memperpendek kodingan, query bersifat tidak case sensitive 
```
## Jawaban d 
```bash
void play_song(const char* song, long userid) {
    char uppercase_song[MAX_LENGTH];
    strncpy(uppercase_song, song, sizeof(uppercase_song));
    for (int i = 0; i < strlen(uppercase_song); i++) {
        uppercase_song[i] = toupper(uppercase_song[i]);
    }
	
    int count = 0;
    char matching_songs[MAX_LENGTH][MAX_LENGTH];

    for (int i = 0; i < playlist_size; i++) {
        char uppercase_playlist_song[MAX_LENGTH];
        strncpy(uppercase_playlist_song, playlist[i], sizeof(uppercase_playlist_song));
        for (int j = 0; j < strlen(uppercase_playlist_song); j++) {
            uppercase_playlist_song[j] = toupper(uppercase_playlist_song[j]);
        }

        if (strstr(uppercase_playlist_song, uppercase_song) != NULL) {
            strncpy(matching_songs[count], playlist[i], sizeof(matching_songs[count]));
            count++;
        }
    }

    if (count == 0) {
        printf("THERE IS NO SONG CONTAINING \"%s\"\n", song);
    } else if (count == 1) {
        char uppercase_matching_song[MAX_LENGTH];
        strncpy(uppercase_matching_song, matching_songs[0], sizeof(uppercase_matching_song));
        for (int j = 0; j < strlen(uppercase_matching_song); j++) {
            uppercase_matching_song[j] = toupper(uppercase_matching_song[j]);
        }
        printf("USER <%ld> PLAYING \"%s\"\n", userid, uppercase_matching_song);
    } else {
        printf("THERE ARE %d SONGS CONTAINING \"%s\":\n", count, song);
        for (int i = 0; i < count; i++) {
            char uppercase_matching_song[MAX_LENGTH];
            strncpy(uppercase_matching_song, matching_songs[i], 		sizeof(uppercase_matching_song));
            for (int j = 0; j < strlen(uppercase_matching_song); j++) {
                uppercase_matching_song[j] = toupper(uppercase_matching_song[j]);
            }
            printf("%d. %s\n", i + 1, uppercase_matching_song);
        }
    }
}
```
**penjelasan**
-	Fungsi `play_song` digunakan untuk mencari lagu dalam `playlist.txt `
-	Pertama, fungsi ini mengonversi string song menjadi huruf kapital dan menyimpannya dalam array `uppercase_song`
-	ungsi melakukan iterasi melalui setiap lagu dalam daftar putar dan melakukan hal yang sama untuk setiap lagu, menyimpan hasilnya dalam array `uppercase_playlist_song`
-	Jika `uppercase_song` ditemukan dalam `uppercase_playlist_song`, nama lagu yang cocok akan disalin ke dalam array matching_songs, dan count akan ditingkatkan sebesar 1.

#### e.	
User juga dapat menambahkan lagu ke dalam playlist dengan syarat sebagai berikut:
```text
1.	User mengirimkan perintah
ADD <SONG1>
ADD <SONG2>
sistem akan menampilkan:
USER <ID_USER> ADD <SONG1>
2.	User dapat mengedit playlist secara bersamaan tetapi lagu yang ditambahkan tidak boleh sama. Apabila terdapat lagu yang sama maka sistem akan meng-output-kan “SONG ALREADY ON PLAYLIST”
```
### Jawaban e
```bash
void add_song(char* song, int userid) {
	FILE* playlist_file = fopen(outputpath, "a");
    int song_found = 0;
    
    for (int i = 0; i < playlist_size; i++) {
        if (strcasecmp(playlist[i], song) == 0) {
            song_found = 1;
            break;
        }
    }
    
    if (song_found) {
        printf("SONG ALREADY ON PLAYLIST\n");
    } else {
        fprintf(playlist_file, "%s\n", song);
        fclose(playlist_file);
        printf("USER <%d> ADD %s\n", userid, song);
    }
}
```
**penjelasan**
-	Fungsi `add_song` yang digunakan untuk menambahkan lagu baru ke dalam playlist
-	Fungsi membuka file `playlist` dengan mode `"a"` (append)
-	fungsi melakukan iterasi pada array `playlist` yang berisi nama-nama lagu yang ada di `playlist` untuk memeriksa apakah nama lagu yang ingin ditambahkan sudah ada di `playlist` atau belum
-	array playlist ini dibuat didalam fungsi `load_playlist()`
-	Dimana Setiap baris yang dibaca dipotong trailing newline character dengan menggunakan fungsi `strcspn()`

#### f.	
Karena Elshe hanya memiliki resource yang kecil, untuk saat ini Elshe hanya dapat memiliki dua user. Gunakan semaphore (wajib) untuk membatasi user yang mengakses playlist.
```text
Output-kan "STREAM SYSTEM OVERLOAD" pada sistem ketika user ketiga mengirim perintah apapun.
```
#### g.	
Apabila perintahnya tidak dapat dipahami oleh sistem, sistem akan menampilkan "UNKNOWN COMMAND".
### Jawaban f dan g
```bash
int main() {

	    key_t key;
	    int msgid;
	    const char* filepath = "song-playlist.json";
	    const char* playlist = "playlist.txt";
	    key = ftok("songfile", 'R');
	    msgid = msgget(key, 0666 | IPC_CREAT);
	    
	    sem_t sem;
	    int semid = semget(key, 1, 0666 | IPC_CREAT);
	    sem_init(&sem, 0, 2);
	   
	    load_playlist();
	    
	    while(1) {
		msgrcv(msgid, &message, sizeof(message), 0, 0);
		printf("\nReceived message: %s \n", message.message_text);
		
		numUsr = 1;
		sem_wait(&sem);
		
		if(usr1 != 1 && usr2 == 2){
		    usr2 = message.pid;
		}
		if(usr1 == 1) {
		    usr1 = message.pid;
		    if(usr2 == 1) {
		        usr2++;
		    }
		}
		if(usr2 == 1 && usr1 != 0 && usr1!= message.pid){
			usr2 = message.pid;
		}
		if(usr1 == 0 && usr2 != message.pid)
			{
			usr1 = message.pid;
			if( usr2 == 0) usr2++; 
			}
		if(strcmp(message.message_text, "EXIT") == 0){
		    if(usr1 == message.pid) {
		        usr1 = 1;
		        numUsr = 0;
		    }
		    else if(usr2 == message.pid) {
		        usr2 = 2;
		        numUsr = 0;
		    }
		}
		 if(usr1 != message.pid  && usr2 != message.pid && usr1 != 1 && usr2 > 2) {
		    printf("STREAM SYSTEM OVERLOAD\n");
		    numUsr = 0;
		}
		
		//proses memanggil comman sesuai dengan yang diinputkan oleh user
		if( numUsr == 1 ) {
		if (strcmp(message.message_text, "DECRYPT") == 0 ) {
				decrypt(filepath, playlist );
				sortPlaylist(playlist);     
		}
		else if (strcmp(message.message_text, "LIST") == 0 ) {
				listSong(playlist);
		}
		else if (strncmp(message.message_text, "PLAY", 4) == 0) {
				char song[MAX_LENGTH];
				sscanf(message.message_text, "PLAY \"%[^\"]\"", song);
				play_song(song, message.pid);
		}
		else if (strncmp(message.message_text, "ADD ", 4) == 0) {
			char song[256];
			sscanf(message.message_text, "ADD %[^\"]", song);
			add_song(song, message.pid);
		} 
		else printf("UNKNOWN COMMAND\n");
		} 
			sem_post(&sem);
		}
			 
	    msgctl(msgid, IPC_RMID, NULL);
	    sem_destroy(&sem);
	    semctl(semid, 0, IPC_RMID);
	    return 0;
}
```
**penjelasan**
-	`sem_t sem;`: Mendefinisikan tipe data `sem_t` dengan nama variabel `sem`.
-	`int semid = semget(key, 1, 0666 | IPC_CREAT);`: Membuat semaphore dengan ID semid berdasarkan key dan hak akses 0666, jika semaphore belum ada maka akan dibuat baru dengan IPC_CREAT.
-	`sem_init(&sem, 0, 2);`: Inisialisasi semaphore dengan variabel sem, 0 menandakan bahwa semaphore digunakan untuk proses thread, dan 2 adalah nilai awal semaphore.
-	`Usr1, usr2, numUsr = 1;`: Menetapkan nilai variabel usr1, usr2, dan numUsr ke 1
-	`sem_wait(&sem);`: Menunggu semaphore sem hingga nilainya bernilai lebih besar dari 0.
-	Selanjutnya, terdapat serangkaian percabangan if-else yang memproses perintah yang diterima dan memperbarui status pengguna (usr1 dan usr2) serta jumlah pengguna (numUsr) yang sedang aktif di server.
-	Jika perintah yang diterima adalah "EXIT", maka server akan memperbarui status pengguna yang mengirimkan pesan dengan nilai yang tepat (usr1 atau usr2) dan mengubah nilai numUsr ke 0.
-	Jika numUsr masih sama dengan 1, maka server akan memproses perintah sesuai dengan jenis perintah yang diterima, seperti "DECRYPT", "LIST", "PLAY", dan "ADD".
-	`sem_post(&sem);`: Membebaskan semaphore sem sehingga nilainya bertambah 1.
-	Setelah selesai memproses pesan dari client, server akan melakukan pembersihan dan dealokasi memori, seperti menghapus message queue dan semaphore serta menutup program dengan kode `return`


## Dokumentasi : 
**penjelasan**
-	Untuk melakukan perintah soal diatas maka disini terdapat 1 stream yaitu rulelulu dan 3 user yaitu ruby, susi, dan nico
-	Pemeriksaan ini dilakukan dengan membandingkan nama lagu song dengan setiap elemen di array playlist menggunakan fungsi strcasecmp yang mengabaikan perbedaan huruf besar-kecil dalam perbandingan.

## 3b. 
Pada user nico diberikan command DECRYPT, sehingga stream menerima perintah DECRYPT dan memberikan informasi 
```text
Received message: DECRYPT 
SONG DECRYPTED
FILE SORTED SUCCESSFULLY
Output yang dihasilkan sudah terurut didalam file playlist.txt dan struktur direktori nya sudah benar
```
![Screenshot (1262)](https://github.com/RuleLuluDamara/Images/assets/105763198/6e180f06-775a-4464-9757-baa8e720d68e)

## 3c. 
Pada user pertama nico diberikan command LIST, sehingga stream menerima perintah LIST dan memberikan informasi seperti dokumentasi dibawah

![Screenshot (1264)](https://github.com/RuleLuluDamara/Images/assets/105763198/348dd6f8-8a49-4b21-99b9-4bc8c290d6cd)
![Screenshot (1263)](https://github.com/RuleLuluDamara/Images/assets/105763198/db6c1ad2-f53f-40e3-b150-b60499d0f7b1)
 
## 3d.
Pada user kedua yaitu ruby diberikan command PLAY Seperti dibawah, sehingga stream menerima perintah PLAY dan memberikan output yang sesuai dengan ketentuan

![Screenshot (1265)](https://github.com/RuleLuluDamara/Images/assets/105763198/5bf44084-69c8-4b20-ab87-b783dc318138)
![Screenshot (1266)](https://github.com/RuleLuluDamara/Images/assets/105763198/875989ef-4719-4701-8da8-4f3c88018cfa)
 
## 3e.
Pada user pertama yaitu nico diberikan command ADD Seperti dibawah, sehingga stream menerima perintah ADD dan memberikan output yang sesuai dengan ketentuan
 
![Screenshot (1271)](https://github.com/RuleLuluDamara/Images/assets/105763198/308c0091-8ddb-4f21-946e-49df8f5d6329)
![Screenshot (1272)](https://github.com/RuleLuluDamara/Images/assets/105763198/9957f2c2-9f36-4140-a6d9-e3241eac5159)

## 3f.
```text
Case 1
1. user 1 nico
2. user 2 ruby
3. user 3 susi -> SYSTEM OVERLOAD
```
![Screenshot (1275)](https://github.com/RuleLuluDamara/Images/assets/105763198/ea9e0300-c436-4a70-8be6-a1084d6a30c5)

```text
Case 2
1. user 1 nico -> EXIT -> user XX nico
2. user 2 ruby
3. user 3 susi -> user 1 Susi
```
![Screenshot (1276)](https://github.com/RuleLuluDamara/Images/assets/105763198/4a214f9a-cb64-4ffd-8e48-bb7e079245e9)
Dari 2 case diatas didapatkan UserID masing masing user
```text
USER <2605> = User 1 nico 
USER <2606> = User 2 ruby 
USER <2607> = User 3 Susi
```

## 3g.
![Screenshot (1277)](https://github.com/RuleLuluDamara/Images/assets/105763198/40fb4281-d619-4b12-b8fd-dfb53fcae876)

## Soal 4
Suatu hari, Amin, seorang mahasiswa Informatika mendapati suatu file bernama **hehe.zip**. Di dalam file .zip tersebut, terdapat sebuah folder bernama files dan file .txt bernama **extensions.txt** dan **max.txt**. Setelah melamun beberapa saat, Amin mendapatkan ide untuk merepotkan kalian dengan file tersebut<br><br> 

## 4a.
Download dan unzip file tersebut dalam kode c bernama **unzip.c**.

### Jawaban
#### Fungsi ```download_zip()```
```bash
void download_zip(char *zipPath) 
{
    pid_t pidDownload = fork();
    if (pidDownload < 0) 
    {
        perror("Failed to fork\n"); 
        exit(EXIT_FAILURE); 
    }
    else if (pidDownload == 0) 
    {
        char *url_new = malloc(128 * sizeof(char));
        if (!url_new) 
        {
            exit(EXIT_FAILURE);
        }
        printf("Downloading file : %s\n", ZIP_URL);
        char *argv[] = {"wget", "-qO", zipPath, ZIP_URL, NULL};
        execv("/bin/wget", argv);
    } 
    else 
    {
        int status;
        if (waitpid(pidDownload, &status, 0) < 0) 
        {
            perror("waitpid() failed");
            exit(EXIT_FAILURE);
        }else {
            while ((wait(&status)) > 0);
        }
    }
}
```
#### Fungsi ```unzipping()```
```bash
void unzipping(char *zipPath)
{
    char *foldername = "hehe";

    pid_t pidUnzip = fork();
    if(pidUnzip == 0){
        printf("Unzipping file : %s\n", zipPath);
        char *argv[] = {"unzip", "-q", "-n", zipPath, "-d", foldername, NULL};
        execv("/bin/unzip", argv);
        exit(0);
    }
    else if (pidUnzip < 0)
    {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    } 
    else 
    {
        int status = 0;
        if (waitpid(pidUnzip, &status, 0) == -1) {
            perror("waitpid() failed");
            exit(EXIT_FAILURE);
        }
        else {
            while ((wait(&status)) > 0);
        }
    }
} 

```
### Penjelasan Jawaban 4a :
1. Buat fungsi untuk mendownload file ```hehe.zip```
    ```bash
    printf("Downloading file : %s\n", ZIP_URL);
    char *argv[] = {"wget", "-qO", zipPath, ZIP_URL, NULL};
    execv("/bin/wget", argv);
    ```

2. Buat fungsi untuk unzip file ```hehe.zip```
    ```bash
    printf("Unzipping file : %s\n", zipPath);
    char *argv[] = {"unzip", "-q", "-n", zipPath, "-d", foldername, NULL};
    execv("/bin/unzip", argv);
    ```

### Dokumentasi dan Output 4a
![Screenshot_2023-05-13_165441](/uploads/0695728a48b2a2a87f75944e07b62ad4/Screenshot_2023-05-13_165441.png)

![Screenshot_2023-05-13_165117](/uploads/9a8830a67b6b2c70408be67e37ac243f/Screenshot_2023-05-13_165117.png)

![Screenshot_2023-05-13_165213](/uploads/e17d5701b8ec9959a02425408a0ca6de/Screenshot_2023-05-13_165213.png)

![Screenshot_2023-05-13_165257](/uploads/b50688132b8c61124e6756d3737505ba/Screenshot_2023-05-13_165257.png)


### 4b.
Selanjutnya, buatlah program **categorize.c** untuk mengumpulkan (move / copy) file sesuai extension-nya. Extension yang ingin dikumpulkan terdapat dalam file **extensions.txt**. Buatlah folder **categorized** dan di dalamnya setiap extension akan dibuatkan folder dengan nama sesuai nama extension-nya dengan nama folder semua lowercase. Akan tetapi, file bisa saja tidak semua lowercase. File lain dengan extension selain yang terdapat dalam .txt files tersebut akan dimasukkan ke folder **other**.<br>
Pada file **max.txt**, terdapat angka yang merupakan isi maksimum dari folder tiap extension kecuali folder **other**. Sehingga, jika penuh, buatlah folder baru dengan format extension (2), extension (3), dan seterusnya.

### Jawaban
#### Fungsi ```get_extension()```
```bash
void* get_extension() { 
    FILE *filePointer = fopen(ext_file, "r");

    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_PATH_LENGTH];
    char **lines = NULL;
    int index=0;

    while (fgets(buffer, MAX_PATH_LENGTH, filePointer)) {
        lines = realloc(lines, sizeof(char*) * (ext_count + 1));
        lines[ext_count] = malloc(sizeof(char) * (strlen(buffer) + 1));

        strncpy(lines[index], buffer, strlen(buffer) - 2);
        ext_dir[index] = 0;

        strcpy(extensions[index], lines[index]);
        index++;
        ext_count++;
    }
    fclose(filePointer);


    chdir(HOME_PATH);

    strcpy(logname, HOME_PATH);
    strcat(logname, "/log");

    strcpy(log_name, logname);
    strcat(log_name, ".txt");

    sprintf(filename, "categorized");
    mkdir(filename, 0777);

    filePointer = fopen(log_name, "a+");

    get_log_time(log_time);
    fprintf(filePointer, "%s MADE %s\n", log_time, filename);

    fclose(filePointer);
    
    for (int i = 0; i < ext_count; i++) {

        sprintf(filename, "categorized/%s", extensions[i]);
        mkdir(filename, 0777);

        filePointer = fopen(log_name, "a+");
        get_log_time(log_time);
        fprintf(filePointer, "%s MADE %s\n", log_time, filename);

        fclose(filePointer);

    }

    sprintf(filename, "categorized/other");
    mkdir(filename, 0777);

    filePointer = fopen(log_name, "a+");
    get_log_time(log_time);
    fprintf(filePointer, "%s MADE %s\n", log_time, filename);

    fclose(filePointer);
}
```

#### Penjelasan Fungsi ```get_extension()```
1. Buka file ```extensions.txt```
    ```bash
    filePointer = fopen(ext_file, "r"); 
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    ```
2. Scan tiap baris pada file ```extensions.txt```
3. Copy isi tiap baris ke dalam variabel ```lines[]``` dan hilangkan spasi pada akhir baris serta set total ekstensi dengan 0
    ```bash
    strncpy(lines[index], buffer, strlen(buffer) - 2);
    ext_dir[index] = 0;
    ```
4. Copy isi yang ada di dalam variabel ```lines[]``` ke dalam variabel global ```extensions[]```
    ```bash
    strcpy(extensions[index], lines[index]);
    index++;
    ext_count++;
    ```
5. Buat file txt ```log.txt``` untuk menyimpan data setiap terjadi manipulasi data
    ```bash
    strcpy(logname, HOME_PATH);
    strcat(logname, "/log");

    strcpy(log_name, logname);
    strcat(log_name, ".txt");
    ```
6. Buat folder ```categorized``` dan tulis perubahan dalam log
    ```bash
    sprintf(filename, "categorized");
    mkdir(filename, 0777);

    filePointer = fopen(log_name, "a+");

    get_log_time(log_time);
    fprintf(filePointer, "%s MADE %s\n", log_time, filename);
    ```
7. Buat folder dengan menggunakan nama yang tersimpan pada variabel global ```extensions[]``` dan tulis perubahan dalam log
    ```bash
    for (int i = 0; i < ext_count; i++) {

        sprintf(filename, "categorized/%s", extensions[i]);
        mkdir(filename, 0777);

        filePointer = fopen(log_name, "a+");
        get_log_time(log_time);
        fprintf(filePointer, "%s MADE %s\n", log_time, filename);
    }
    ```
8. Buat folder dengan nama ```other``` dan tulis perubahan dalam log
    ```bash
    sprintf(filename, "categorized/other");
    mkdir(filename, 0777);

    filePointer = fopen(log_name, "a+");
    get_log_time(log_time);
    fprintf(filePointer, "%s MADE %s\n", log_time, filename);
    ```

#### Fungsi ```read_max()```
```bash
void *read_max() {
    FILE *filePointer ;
    char line[255];
    int i = 0;

    filePointer = fopen(max_file, "r");

    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), filePointer) ) {
        max_values = atoi(line);
    }

    fclose(filePointer);

    scan_files(FILES);
}
```

#### Penjelasan Fungsi ```read_max()```
1. Scan baris pada file ```max.txt```
2. Simpan angka pada variabel global ```max_values```

#### Fungsi ```move_files()```
```bash
void *move_files() 
{
    FILE *filePointer;
    char cmd[9000];
    char ext[MAX_PATH_LENGTH];
    char line[MAX_PATH_LENGTH];
    char buffer[5000];

    filePointer = fopen(list_file, "r");

    while(fgets(line, MAX_PATH_LENGTH, filePointer)) {
        line[strlen(line)-1] = '\0';

        sprintf(ext, "%s", check_name(line));

        for(int i = 0; ext[i]; i++){
            ext[i] = tolower(ext[i]);
        }

        if (check_extension(ext) == -1) {
            sprintf(cmd, "cp '%s' ./categorized/other", line);
            system(cmd);

            get_log_time(log_time);
            sprintf(cmd, "echo '%s MOVED other file : %s > ./categorized/other' >> %s", log_time, line, log_name);
            system(cmd);
        }
        else { 
            if (ext_dir[check_extension(ext)] < max_values) {
                sprintf(cmd, "cp '%s' ./categorized/%s", line, ext);
                system(cmd);

                ext_dir[check_extension(ext)]++;

                get_log_time(log_time);
                sprintf(cmd, "echo '%s MOVED %s file : %s > ./categorized/%s' >> %s", log_time, ext, line, ext, log_name);
                system(cmd);
            }
            else { 
                int num_ext = ext_dir[check_extension(ext)] / max_values + 1;
                sprintf(buffer, "./categorized/%s (%d)", ext, num_ext);

                if(access(buffer, F_OK) != 0){
                    get_log_time(log_time);
                    sprintf(cmd, "echo '%s MADE %s' >> %s", log_time, buffer, log_file);
                    system(cmd);
                }

                mkdir(buffer, 0777);

                sprintf(cmd, "cp '%s' './categorized/%s (%d)'", line, ext, num_ext);
                system(cmd);

                ext_dir[check_extension(ext)]++;

                get_log_time(log_time);
                sprintf(cmd, "echo '%s MOVED %s file : %s > .%s' >> %s", log_time, ext, line, buffer, log_name);
                system(cmd);
            }
        }
    }
}
```

#### Penjelasan Fungsi ```move_files()```
1. Scan temporary file ```lists.txt```
2. Cek apakah ada ekstensi pada nama file pada ```lists.txt``` menggunakan fungsi ```check_name()```. dan simpan nama ekstensi pada variabel ```ext[]```
    ```bash
    const char *check_name(char *dir){
        const char *name = strrchr(dir, '.');
        if(!name || name == dir)
            return "";
        return name + 1;
    }
    ```
3. Ubah tulisan ekstensi menjadi lowercase
    ```bash
    for(int i = 0; ext[i]; i++){
        ext[i] = tolower(ext[i]);
    }
    ```
4. Cek apakah ekstensi pada ```ext[]``` ada pada ```extensions.txt``` atau tidak
    - Jika ada, file tersebut masuk folder sesuai dengan ekstensi masing-masing file dan menulis log
        - Jika folder ekstensi tersebut belum mencapai jumlah maksimal, file akan di cek apakah memiliki ekstensi yang sama dengan yang tersimpan dalam variabel ```extensions[]``` 
            ```bash
            int check_extension(char ext[]) 
            {
                for(int i=0; i<ext_count;i++){
                    if(strcmp(ext, extensions[i]) == 0)
                        return i;
                }
                return -1;
            }
            ```
            ```bash
            if (ext_dir[check_extension(ext)] < max_values) {
                sprintf(cmd, "cp '%s' ./categorized/%s", line, ext);
                system(cmd);

                ext_dir[check_extension(ext)]++;

                get_log_time(log_time);
                sprintf(cmd, "echo '%s MOVED %s file : %s > ./categorized/%s' >> %s", log_time, ext, line, ext, log_name);
                system(cmd);
            }
            ```
        - Jika folder ekstensi tersebut sudah mencapai jumlah maksimal, folder baru akan terbuat dan file akan masuk ke dalam folder baru tersebut
            ```bash
            else { // kalo folder pertama udah penuh, bikin folder part lanjutan
                int num_ext = ext_dir[check_extension(ext)] / max_values + 1;
                sprintf(buffer, "./categorized/%s (%d)", ext, num_ext);

                if(access(buffer, F_OK) != 0){
                    get_log_time(log_time);
                    sprintf(cmd, "echo '%s MADE %s' >> %s", log_time, buffer, log_file);
                    system(cmd);
                }

                mkdir(buffer, 0777);

                sprintf(cmd, "cp '%s' './categorized/%s (%d)'", line, ext, num_ext);
                system(cmd);

                ext_dir[check_extension(ext)]++;

                get_log_time(log_time);
                sprintf(cmd, "echo '%s MOVED %s file : %s > .%s' >> %s", log_time, ext, line, buffer, log_name);
                system(cmd);
            }
            ```
    - Jika berbeda, file tersebut masuk ke folder ```other``` dan menulis log
        ```bash
        if (check_extension(ext) == -1) {
            sprintf(cmd, "cp '%s' ./categorized/other", line);
            system(cmd);

            get_log_time(log_time);
            sprintf(cmd, "echo '%s MOVED other file : %s > ./categorized/other' >> %s", log_time, line, log_name);
            system(cmd);
        }
        ```


### Dokumentasi dan Output 4b
![Screenshot_2023-05-13_165241](/uploads/73fa1caa2261116c424ff08bbdcb155f/Screenshot_2023-05-13_165241.png)

![Screenshot_2023-05-13_165320](/uploads/36007f1694c84ea40bd5ffb3641b2dd7/Screenshot_2023-05-13_165320.png)

![Screenshot_2023-05-13_165333](/uploads/8b5ab52af4aa7266a6082c23fad5aacb/Screenshot_2023-05-13_165333.png)

![Screenshot_2023-05-13_165352](/uploads/f652b0fd8c4b6ac25fc1c925ea30e001/Screenshot_2023-05-13_165352.png)

### 4c.
Output-kan pada terminal banyaknya file tiap extension terurut ascending dengan semua lowercase, beserta other juga dengan format sebagai berikut: <br>
**extension_a : banyak_file <br> 
extension_b : banyak_file <br> 
extension_c : banyak_file <br> 
other : banyak_file** <br> 

### Jawaban
#### Fungsi ```count_extension()```
```bash
void *count_extension() 
{
    char cmd[9000];
    strcpy(extensions[ext_count], "other");
    DIR *dir;
    struct dirent *ent;
    dir = opendir("./categorized/other");

    get_log_time(log_time);
    sprintf(cmd, "echo '%s ACCESSED ./categorized/other' >> %s", log_time, log_name);
    system(cmd);

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            ext_dir[ext_count]++;
        }
    }

    closedir(dir);
    ext_count++;

    for(int i=0;i<ext_count-1; i++){
        for(int j=0; j<ext_count-i-1;j++){
            if(ext_dir[j] > ext_dir[j+1]){
                int temp;
                temp = ext_dir[j];
                ext_dir[j] = ext_dir[j+1];
                ext_dir[j+1] = temp;

                strcpy(filename, extensions[j]);
                strcpy(extensions[j], extensions[j+1]);
                strcpy(extensions[j+1], filename);
            }
        }
    }
    
    for(int i=0; i<ext_count; i++){
        printf("%s : %d\n", extensions[i], ext_dir[i]);
    }
}
```

#### Penjelasan Fungsi ```count_extension()```
1. Cek file dalam folder ```other``` terlebih dahulu
    ```bash
    strcpy(extensions[ext_count], "other");
    dir = opendir("./categorized/other");
    ```
2. Tulis pada ```log.txt``` bahwa folder ```other``` sudah diakses
    ```bash
    get_log_time(log_time);
    sprintf(cmd, "echo '%s ACCESSED ./categorized/other' >> %s", log_time, log_name);
    system(cmd);
    ```
3. Cek apakah file tersebut bertipe reguler atau directory
    - Jika file bertipe reguler, maka ```ext_dir[]``` akan bertambah
        ```bash
        if (ent->d_type == DT_REG) {
            ext_dir[ext_count]++;
        }
        ```
    - Jika file bertipe directory, maka ```ext_count``` akan bertambah. Ini artinya, akan dilanjut ke folder selanjutnya
        ```bash
        ext_count++;
        ```
4. Urutkan folder ekstensi dan jumlahnya secara ascending
    ```bash
    for(int i=0;i<ext_count-1; i++){
        for(int j=0; j<ext_count-i-1;j++){
            if(ext_dir[j] > ext_dir[j+1]){
                int temp;
                temp = ext_dir[j];
                ext_dir[j] = ext_dir[j+1];
                ext_dir[j+1] = temp;

                strcpy(filename, extensions[j]);
                strcpy(extensions[j], extensions[j+1]);
                strcpy(extensions[j+1], filename);
            }
        }
    }
    ```
5. Outputkan nama folder ekstensi yang ada beserta jumlahnya
    ```bash
    for(int i=0; i<ext_count; i++){
        printf("%s : %d\n", extensions[i], ext_dir[i]);
    }
    ```

### Dokumentasi dan Output 4c
![Screenshot_2023-05-13_165501](/uploads/f2fbf677584a9e475f00252bdfa8188b/Screenshot_2023-05-13_165501.png)


### 4d.
Setiap pengaksesan folder, sub-folder, dan semua folder pada program categorize.c **wajib menggunakan multithreading**. Jika tidak menggunakan akan ada pengurangan nilai.

### Jawaban
#### Fungsi ```scan_files()```
```bash
void *scan_files(void *arg) 
{
    DIR *dir;
    struct dirent *ent;
    char path[MAX_PATH_LENGTH];
    char cmd[9000];

    if ((dir = opendir((char*) arg)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR) { 
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                    continue;        
                } 

                sprintf(path, "%s/%s", (char*) arg, ent->d_name);

                get_log_time(log_time);
                sprintf(cmd, "echo '%s ACCESSED %s' >> %s", log_time, path, log_name);
                system(cmd);

                pthread_t tid;
                pthread_create(&tid, NULL, scan_files, (void*) path);
                pthread_join(tid, NULL);

            }
            else {
                sprintf(cmd, "echo '%s/%s' >> lists.txt", (char*) arg,ent->d_name);
                system(cmd);    
            }
        }

        closedir(dir);
    }
}
```
#### Penjelasan Fungsi ```scan_files()```
1. Open direktori
2. Cek tipe file yang ada pada direktori tersebut
    - Jika file tersebut bertipe directory, di cek kembali apakah terdapat folder lagi atau tidak
        - Jika terdapat folder lagi, maka sistem akan melanjutkan ke folder berikutnya menggunakan ```multithreading```
            ```bash
            if (strcmp(ent->d_name, ".")==0 || strcmp(ent->d_name, "..")==0) {  
                    continue;        
                } 

                sprintf(path, "%s/%s", (char*) arg, ent->d_name);

                get_log_time(log_time);
                sprintf(cmd, "echo '%s ACCESSED %s' >> %s", log_time, path, log_name);
                system(cmd);

                pthread_t tid;
                pthread_create(&tid, NULL, scan_files, (void*) path);
                pthread_join(tid, NULL);
            ```
        - Jika sudah tidak ada, maka direktori lengkap dari file tersebutr akan dicatat ke dalam file temporary ```lists.txt```
            ```bash
            sprintf(cmd, "echo '%s/%s' >> lists.txt", (char*) arg,ent->d_name);
            system(cmd);
            ```

### 4e.
Dalam setiap pengaksesan folder, pemindahan file, pembuatan folder pada program categorize.c buatlah log dengan format sebagai berikut : <br>
**DD-MM-YYYY HH:MM:SS ACCESSED [folder path] <br>
DD-MM-YYYY HH:MM:SS MOVED [extension] file : [src path] > [folder dst] <br>
DD-MM-YYYY HH:MM:SS MADE [folder name]** <br> 
Examples : <br>
02-05-2023 10:01:02 ACCESSED files <br> 
02-05-2023 10:01:03 ACCESSED files/abcd <br> 
02-05-2023 10:01:04 MADE categorized <br> 
02-05-2023 10:01:05 MADE categorized/jpg <br> 
02-05-2023 10:01:06 MOVED jpg file : files/abcd/foto.jpg > categorized/jpg <br> 
Catatan: <br>
- Path dimulai dari folder files atau categorized
- Simpan di dalam **log.txt**
- ACCESSED merupakan folder files beserta dalamnya
- Urutan log tidak harus sama

### Jawaban, Dokumentasi, dan Output 4e
Untuk jawaban nomor 4 poin e tersebar pada tiap fungsi. Langsung saja saya beri hasil berupa contoh isi dari ```log.txt```
1. ACCESSED<br>
    ![Screenshot_2023-05-13_164307](/uploads/44e57c9dca49583d4562746aa2d19fbd/Screenshot_2023-05-13_164307.png)
2. MOVED<br>
![Screenshot_2023-05-13_164339](/uploads/e555e3459a06a69dd33573427add9bbb/Screenshot_2023-05-13_164339.png)
3. MADE<br>
    ![Screenshot_2023-05-13_164128](/uploads/0ca3405d0e78312e79e4dc7b4b9d026c/Screenshot_2023-05-13_164128.png)

### 4f.
Untuk mengecek apakah log-nya benar, buatlah suatu program baru dengan nama logchecker.c untuk mengekstrak informasi dari log.txt dengan ketentuan sebagai berikut:
- Untuk menghitung banyaknya ACCESSED yang dilakukan.
- Untuk membuat list seluruh folder yang telah dibuat dan banyaknya file yang dikumpulkan ke folder tersebut, terurut secara ascending.
- Untuk menghitung banyaknya total file tiap extension, terurut secara ascending.

### Jawaban 4f
#### Fungsi ```accessed()```
```bash
int accessed() 
{
    FILE *filePointer;
    char line[MAX_PATH_LENGTH];
    int access = 0;

    filePointer = fopen(log_file, "r");
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_PATH_LENGTH, filePointer)) {
        if (strstr(line, "ACCESSED") != NULL) {
            access++;
        }
    }

    return access;
    fclose(filePointer);
}
```

#### Penjelasan Fungsi ```accessed()```
1. Buka file ```log.txt```
    ```bash
    filePointer = fopen(log_file, "r"); 
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }
    ```
2. Cek apakah ada baris yang ada string ```ACCESSED``` atau tidak
    - Jika ada, variabel ```access``` akan bertambah
        ```bash
        while (fgets(line, MAX_PATH_LENGTH, filePointer)) {
            if (strstr(line, "ACCESSED") != NULL) {
                access++;
            }
        }
        ```
    - Jika tidak, maka baris tersebut diabaikan dan dilanjut ke baris berikutnya

#### Fungsi ```dir_total()```
```bash
void dir_total() 
{
    FILE *filePointer;
    char line[MAX_PATH_LENGTH];

    filePointer = fopen(log_file, "r");

    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_PATH_LENGTH, filePointer)) {
        line[strlen(line)-1] = '\0';

        if (strstr(line, "MADE") != NULL) {
            char *k = strrchr(line, '/');

            if (k != NULL) {
                dirtotal[idxdir] = 0;
                strcpy(dirpath[idxdir++], k+1);
            }
        }

        if (strstr(line, "MOVED") != NULL) {
            char *k = strrchr(line, '/');

            if (k != NULL) {
                for (int i=0 ; i<idxdir ; i++) {
                    if (strcmp(dirpath[i], k+1) == 0) {
                        dirtotal[i]++;
                    }
                }
            }
        }
        
    }
    
    for(int i=0;i<idxdir; i++){
        for(int j=0; j<idxdir-1;j++){
            if(dirtotal[j] > dirtotal[j+1]){
                
                char buffer[MAX_PATH_LENGTH];
                strcpy(buffer, dirpath[j]);
                strcpy(dirpath[j], dirpath[j+1]);
                strcpy(dirpath[j+1], buffer);

                int temp;
                temp = dirtotal[j];
                dirtotal[j] = dirtotal[j+1];
                dirtotal[j+1] = temp;
            }
        }
    }

    for(int i=0; i<idxdir; i++){
        printf("%s : %d\n", dirpath[i], dirtotal[i]);
    }

    fclose(filePointer);
}
```

#### Penjelasan Fungsi ```dir_total()```
1. Buka file ```log.txt```
    ```bash
    filePointer = fopen(log_file, "r");
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }
    ```

2. Cari apakah ada baris yang ada string ```MADE``` atau tidak
    - Jika ada, masukkan baris tersebut ke dalam variabel ```dirpath[]```
        ```bash
        if (strstr(line, "MADE") != NULL) 
        {
            char *k = strrchr(line, '/');
            if (k != NULL) {
                dirtotal[idxdir] = 0;
                strcpy(dirpath[idxdir++], k+1);
            }
        }
        ```
    - Jika tidak ada, baris tersebut diabaikan

3. Cari apakah ada baris yang ada string ```MOVED``` atau tidak
    - Jika ada, total file dalam folder akan bertambah
        ```bash
        if (strstr(line, "MOVED") != NULL) {
            char *k = strrchr(line, '/');
            if (k != NULL) {
                for (int i=0 ; i<idxdir ; i++) {
                    if (strcmp(dirpath[i], k+1) == 0) {
                        dirtotal[i]++;
                    }
                }
            }
        }
        ```
    - Jika tidak ada, baris tersebut diabaikan

4. Urutkan secara ascending
    ```bash
    for(int i=0;i<idxdir; i++){
        for(int j=0; j<idxdir-1;j++){
            if(dirtotal[j] > dirtotal[j+1]){ 
                char buffer[MAX_PATH_LENGTH];
                strcpy(buffer, dirpath[j]);
                strcpy(dirpath[j], dirpath[j+1]);
                strcpy(dirpath[j+1], buffer);

                int temp;
                temp = dirtotal[j];
                dirtotal[j] = dirtotal[j+1];
                dirtotal[j+1] = temp;
            }
        }
    }
    ```
5. Print nama folder beserta jumlah filenya
    ```bash
    for(int i=0; i<idxdir; i++){
        printf("%s : %d\n", dirpath[i], dirtotal[i]);
    }
    ```

#### Fungsi ```ext_total()```
```bash
void ext_total() 
{
    FILE *filePointer;
    char line[MAX_PATH_LENGTH];

    filePointer = fopen(ext_file, "r");
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_PATH_LENGTH, filePointer)) {
        line[strlen(line)-2] = '\0';
        strcpy(extpath[idxext], line);
        exttotal[idxext] = 0;
        idxext++;
    }
    fclose(filePointer);

    strcpy(extpath[idxext], "other");
    exttotal[idxext]=0;
    idxext++;

    filePointer = fopen(log_file, "r"); 
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    while(fgets(line, MAX_PATH_LENGTH, filePointer)){

        if (strstr(line, "MOVED ") != NULL) {
            line[strlen(line)-1] = '\0';
            char *start = strstr(line, "MOVED ");

            char *en = strstr(start, " file :");
            
            strncpy(filename, start + strlen("MOVED "), en - start - strlen("MOVED "));
            filename[en - start - strlen("MOVED ")] = '\0';

            for (int i=0 ; i<idxdir ; i++) {
                if (strcmp(extpath[i], filename) == 0) {
                    exttotal[i]++;
                }
            }
        }


    }
    fclose(filePointer);

    for(int i=0;i<idxext; i++){
        for(int j=0; j<idxext-1;j++){
            if(exttotal[j] > exttotal[j+1]){
                char buffer[MAX_PATH_LENGTH];
                strcpy(buffer, extpath[j]);
                strcpy(extpath[j], extpath[j+1]);
                strcpy(extpath[j+1], buffer);

                int temp;
                temp = exttotal[j];
                exttotal[j] = exttotal[j+1];
                exttotal[j+1] = temp;
            }
        }
    }

    for(int i=0; i<idxext; i++){
        printf("%s : %d\n", extpath[i], exttotal[i]);
    }
}
```

#### Penjelasan Fungsi ```ext_total()```
1. Buka file ```extensions.txt```
    ```bash
    filePointer = fopen(ext_file, "r");
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }
    ```

2. Scan dan copy barisnya ke dalam variabel global ```extpath[]```. Inisialisasi variabel ```exttotal[]```, untuk menghitung total ekstensi, dengan 0.
    ```bash
    while (fgets(line, MAX_PATH_LENGTH, filePointer)) 
    {
        line[strlen(line)-2] = '\0';
        strcpy(extpath[idxext], line);
        exttotal[idxext] = 0;
        idxext++;
    }
    ```

3. Buka file ```log.txt``` dan scan per baris
    ```bash
    filePointer = fopen(log_file, "r"); 
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }
    ```

4. Cari apakah baris tersebut ada string ```MOVED``` atau tidak
    - Jika ada, cek ekstensinya, lalu total file dengan ekstensi tersebut akan bertambah
        ```bash
        if (strstr(line, "MOVED ") != NULL) {
            line[strlen(line)-1] = '\0';
            char *start = strstr(line, "MOVED ");

            char *en = strstr(start, " file :");
            
            strncpy(filename, start + strlen("MOVED "), en - start - strlen("MOVED "));
            filename[en - start - strlen("MOVED ")] = '\0';

            for (int i=0 ; i<idxdir ; i++) {
                if (strcmp(extpath[i], filename) == 0) {
                    exttotal[i]++;
                }
            }
        }
        ```
    - Jika tidak ada, abaikan

5. Urutkan nama ekstensi beserta jumlah filenya secara ascending
    ```bash
    for(int i=0;i<idxext; i++){
        for(int j=0; j<idxext-1;j++){
            if(exttotal[j] > exttotal[j+1]){
                char buffer[MAX_PATH_LENGTH];
                strcpy(buffer, extpath[j]);
                strcpy(extpath[j], extpath[j+1]);
                strcpy(extpath[j+1], buffer);

                int temp;
                temp = exttotal[j];
                exttotal[j] = exttotal[j+1];
                exttotal[j+1] = temp;
            }
        }
    }
    ```

6. Print nama ekstensi beserta jumlah filenya
    ```bash
    for(int i=0; i<idxext; i++){
        printf("%s : %d\n", extpath[i], exttotal[i]);
    }
    ```

### Dokumentasi dan Output 4f
![Screenshot_2023-05-13_165535](/uploads/540534a25bc87a2a73b2906864b09cb0/Screenshot_2023-05-13_165535.png)

## Kendala Nomor 4
Kendala yang dihadapi adalah:
1. Sempat tidak bisa melakukan rekursif saat scan folder
2. Semua file sempat masuk ke dalam folder categorized/other, bukan masuk ke folder dengan ekstensi masing-masing
3. Kebingungan cara sorting yang dimaksud pada soal, apakah urut ascending dari nama ekstensinya atau jumlahnya
4. Sempat typo ketika memasukkan data ke dalam log.txt, sehingga mempengaruhi pengerjaan nomor 4f
5. kami stress:)
