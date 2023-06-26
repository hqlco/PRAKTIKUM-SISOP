#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <ctype.h>
#include <json-c/json.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>

#define MAX_MESSAGE_SIZE 512
#define MAX_LENGTH 1024

const char* filepath = "song-playlist.json";
const char* outputpath = "playlist.txt";

int usr1 = 1, usr2 = 1, numUsr = 1;
//struk message
struct message_buffer {
    long message_type;
    char message_text[MAX_MESSAGE_SIZE];
    pid_t pid;
} message;

//membandingkan dua baris (string) dalam sebuah array of pointers to strings
//dengan memperhitungkan huruf besar-kecil (case-sensitive).
int compareRows(const void* a, const void* b) {
    char *str_a = *(char **)a;
    char *str_b = *(char **)b;
    
    
    char *str_a_lower = strdup(str_a);
    for (int i = 0; str_a_lower[i]; i++) {
        str_a_lower[i] = tolower(str_a_lower[i]);
    }
    char *str_b_lower = strdup(str_b);
    for (int i = 0; str_b_lower[i]; i++) {
        str_b_lower[i] = tolower(str_b_lower[i]);
    }

    int result = strcmp(str_a_lower, str_b_lower);

    free(str_a_lower); // pointer str_a_lower dan str_b_lower akan di-free terlebih dahulu untuk menghindari memory leak.
    free(str_b_lower);

    return result;
    
    }

void sortPlaylist(const char* filepath){
    	char* rows[MAX_LENGTH];
    	char line[MAX_LENGTH];
    	int count = 0;
    	
    //Deklarasi dan inisialisasi variabel, membuka file, dan membaca setiap baris dari file ke 
    //dalam array "rows" yang sudah dialokasikan dengan ukuran maksimum sebesar MAX_LENGTH
	FILE* file = fopen(filepath, "r");
	    if (file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	    }
	    
	    while (fgets(line, MAX_LENGTH, file) != NULL) {
		line[strcspn(line, "\n")] = '\0';

		rows[count] = malloc(strlen(line) + 1);
		strcpy(rows[count], line);

		count++;
	    }
	    
	    //Mengurutkan baris-baris dalam array "rows" menggunakan fungsi qsort dan fungsi perbandingan "compareRows".
	    
	    fclose(file);
	    qsort(rows, count, sizeof(char*), compareRows);

	    file = fopen(filepath, "w");
	    if (file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	    }
		
		// menulis setiap baris dari array "rows" ke dalam file, membebaskan memori yang dialokasikan 
        //untuk setiap elemen dalam array "rows",
	    int i;
	    for (i = 0; i < count; i++) {
		fprintf(file, "%s\n", rows[i]);

		free(rows[i]);
	    }

	    fclose(file);
	    printf("File sorted per row successfully.\n");
}


void decrypt(const char* filepath, const char* outputpath) {
	int numSongs;

	//Membuka file dengan path filepath dalam mode r untuk membaca, kemudian memeriksa apakah file berhasil dibuka atau tidak
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
    
    //mem-parse string JSON ke dalam objek JSON. Kemudian, menghapus memori yang dialokasikan untuk fileContents dengan free.
    struct json_object* json = json_tokener_parse(fileContents);
    free(fileContents);
    
    //Memeriksa apakah objek JSON yang di-parse dari file adalah array JSON
    // Jika tidak, menampilkan pesan error dan menghapus objek JSON dengan json_object_put, lalu keluar dari fungsi dengan return.
    
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
    
    //Selanjutnya, program akan melakukan loop sebanyak numSongs, yang merupakan panjang array json. 
    //Pada setiap iterasi, program akan mengambil objek lagu pada indeks i dari array json menggunakan 
    //json_object_array_get_idx(), dan kemudian mengambil objek method dan objek song dari objek lagu menggunakan 
    //json_object_object_get_ex().

    for (int i = 0; i < numSongs; i++) {
        struct json_object* songObj = json_object_array_get_idx(json, i);
        struct json_object* methodObj;
        struct json_object* songStringObj;

        if (json_object_object_get_ex(songObj, "method", &methodObj) &&
            json_object_object_get_ex(songObj, "song", &songStringObj)) {
            const char* method = json_object_get_string(methodObj);
            const char* songString = json_object_get_string(songStringObj);
        
        // metode "hex", program akan memecah string lagu menjadi pasangan dua karakter yang masing-masing mewakili 
        //satu byte, dan mengonversi setiap pasangan menjadi bilangan heksadesimal menggunakan strtol(). 
        //Kemudian, setiap byte yang dihasilkan akan digabungkan menjadi sebuah string karakter yang didekripsi dan 
        //ditulis ke file output menggunakan fprintf(). Setelah itu, alokasi memori untuk decryptedString akan dibebaskan 
        //dengan menggunakan free().
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

            /*metode "base64", program akan memecah string lagu menjadi blok 4 karakter, dan mengonversi setiap blok 
            menjadi tiga byte dengan menggunakan lookup table. Kemudian, setiap byte yang dihasilkan akan digabungkan 
            menjadi sebuah string karakter yang didekripsi dan ditulis ke file output menggunakan fprintf(). Setelah itu, 
            alokasi memori untuk decryptedString akan dibebaskan dengan menggunakan free().
            */
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

            /*metode enkripsi yang digunakan adalah "rot13" Untuk setiap karakter, blok program akan mengecek 
            apakah karakter tersebut merupakan huruf kecil atau huruf besar dalam bahasa Inggris menggunakan 
            kondisi if-else. Jika ya, karakter tersebut akan dienkripsi menggunakan algoritma ROT13. Jika tidak, 
            karakter tersebut akan langsung ditambahkan pada  decryptedString. decryptedString akan di-release dari 
            memory menggunakan fungsi free(). Jika metode enkripsi yang digunakan tidak dikenal, maka program akan 
            mencetak pesan "Metode dekripsi tidak diketahui.
            */
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
    printf("Deskripsi Berhasil\n");
}

//fungsi ini dugunakan untuk menampilkan playlist yang sudah disroting

void listSong(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filepath);
        return;
    }

    char line[256];
    printf("Decrypted Songs:\n");
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  // Remove trailing newline character
        printf("%s\n", line);
    }

    fclose(file);
}

char playlist[MAX_LENGTH][MAX_LENGTH];
int playlist_size = 0;

//Kode ini merupakan implementasi dari fungsi load_playlist() yang bertujuan untuk membaca isi file playlist 
//dengan path outputpath dan menyimpannya dalam array playlist.
void load_playlist() {
    FILE* file = fopen(outputpath, "r");
    
    char line[MAX_LENGTH];
    //dilakukan loop dengan menggunakan fgets() untuk membaca setiap baris dalam file. Setiap baris kemudian 
    //dihapus karakter newline di akhir baris dengan menggunakan fungsi strcspn() dan menggantinya dengan karakter null-terminating.
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';  

        if (playlist_size < MAX_LENGTH) {
            strncpy(playlist[playlist_size], line, sizeof(playlist[playlist_size]));
            playlist_size++;
        } else {
            printf("Playlist is full. Skipping song: %s\n", line);
        }
    }

    fclose(file);
}

//Mendefinisikan sebuah fungsi dengan tipe data return boolean dengan parameter sebuah string.
bool is_song_in_playlist(const char* song) {
    FILE* playlist_file = fopen(outputpath, "r");
    if (playlist_file == NULL) {
        perror("Failed to open playlist file");
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), playlist_file)) {
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, song) == 0) {
            fclose(playlist_file);
            return true;
        }
    }

    fclose(playlist_file);
    return false;
}

// implementasi dari fungsi add_song yang berfungsi untuk menambahkan sebuah lagu ke dalam playlist. 
//Berikut adalah penjelasan kode ini perbaris:
void add_song(char* song, int userid) {
	FILE* playlist_file = fopen(outputpath, "a");
    int song_found = 0;
    
    //Memeriksa apakah lagu yang akan ditambahkan sudah ada pada playlist atau belum. Jika sudah ada, 
    //maka variabel song_found akan diubah menjadi 1, sedangkan jika belum ada, variabel song_found akan tetap 0.
    for (int i = 0; i < playlist_size; i++) {
        if (strcasecmp(playlist[i], song) == 0) {
            song_found = 1;
            break;
        }
    }
    
    //Jika lagu sudah ada pada playlist, maka fungsi akan mencetak pesan "SONG ALREADY ON PLAYLIST". 
    //Jika belum ada, maka lagu akan ditambahkan ke dalam playlist dengan menggunakan fungsi fprintf(), 
    //dan kemudian file akan ditutup dengan menggunakan fungsi fclose(). Selain itu, fungsi juga mencetak 
    //pesan "USER <userid> ADD <song>" yang menunjukkan bahwa user dengan ID <userid> telah menambahkan lagu 
    //dengan nama <song> ke dalam playlist.
    if (song_found) {
        printf("SONG ALREADY ON PLAYLIST\n");
    } else {
        fprintf(playlist_file, "%s\n", song);
        fclose(playlist_file);
        printf("USER <%d> ADD %s\n", userid, song);
    }
}

//Fungsi play_song akan memainkan sebuah lagu dengan nama yang diberikan sebagai argumen. Fungsi ini memiliki 
//parameter song yang merupakan string yang berisi nama lagu yang akan diputar, dan parameter userid yang merupakan 
//bilangan bulat panjang yang merepresentasikan ID pengguna.
void play_song(const char* song, long userid) {

//Pertama-tama, fungsi ini akan membuat sebuah variabel array uppercase_song yang berisi string nama lagu yang diubah 
//menjadi huruf kapital menggunakan fungsi toupper().
    char uppercase_song[MAX_LENGTH];
    strncpy(uppercase_song, song, sizeof(uppercase_song));
    for (int i = 0; i < strlen(uppercase_song); i++) {
        uppercase_song[i] = toupper(uppercase_song[i]);
    }
	
    int count = 0;
    char matching_songs[MAX_LENGTH][MAX_LENGTH];
    //fungsi ini akan menghitung jumlah lagu dalam playlist yang cocok dengan song dan menyimpan daftar lagu tersebut 
    //ke dalam array matching_songs. Pencocokan lagu dilakukan dengan membandingkan nama lagu yang telah diubah menjadi 
    //huruf kapital menggunakan fungsi strstr().

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
        printf("USER <%ld> PLAYING \"%s\"\n", userid, matching_songs[0]);
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

int main() {
	    //deklarasikan beberapa variabel
	    key_t key;
	    sem_t sem;
	    int msgid;
	    const char* filepath = "song-playlist.json";
	    const char* playlist = "playlist.txt";
	    key = ftok("songfile", 'R');
	    //membuat message queue dengan msgget dan semaphore dengan semget. 
	    msgid = msgget(key, 0666 | IPC_CREAT);
	    int semid = semget(key, 1, 0666 | IPC_CREAT);
	    //menginisialisasi semaphore dengan jumlah akses sebanyak 2, dan memuat playlist dari file menggunakan 
        //fungsi load_playlist().
	    sem_init(&sem, 0, 2);
	    load_playlist();
	    
	    //berada dalam loop tak terbatas untuk menerima pesan dari message queue. Fungsi msgrcv digunakan 
        //menerima pesan dari message queue. Pesan akan disimpan pada variabel message, dan panjang maksimum pesan 
        //adalah sizeof(message)
	    while(1) {
	    	//Menerima pesan dari message queue dengan memanggil fungsi msgrcv(). Fungsi ini akan memblokir proses 
            //sampai ada pesan yang diterima dari message queue.
		msgrcv(msgid, &message, sizeof(message), 0, 0);
		printf("Received message: %s \n", message.message_text);
		
		numUsr = 1;
		sem_wait(&sem);
		//Menunggu semaphore untuk masuk ke critical section. Jika semaphore bernilai 0, maka proses akan diblokir 
        //sampai semaphore bernilai lebih dari 0.

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
	    sem_destroy(&sem);//menghancurkan semaphore
	    return 0;
}