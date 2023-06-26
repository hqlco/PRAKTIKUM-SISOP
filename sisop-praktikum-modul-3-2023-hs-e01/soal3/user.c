#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>

#define MAX_MESSAGE_SIZE 512

//struct untuk message 
struct message_buffer {
    long message_type;
    char message_text[MAX_MESSAGE_SIZE];
    pid_t pid;
} message;
  
int main()
{
    key_t key; // declare key 
    pid_t pid; // declare pid
    int msgid;
    char cmd[MAX_MESSAGE_SIZE];
    key = ftok("songfile", 'R'); //deklarasi unique key untuk message queue 
    msgid = msgget(key, 0666 | IPC_CREAT);
    pid =  getpid();
    	
    while(1) {
    	printf("Enter command : ");
    	fgets(cmd,MAX_MESSAGE_SIZE,stdin);
    	cmd[strcspn(cmd, "\n")] = '\0'; //replace newline character dengan message
    	message.pid = pid; //Memasukkan pid kedalam message.pid yang akan digunakan sebagai userid
    	message.message_type = 1;
    	strcpy(message.message_text, cmd); //copy message input ke dalam message strukutr
    	//Send message ke message queue
    	msgsnd(msgid, &message, sizeof(message), 0);
    }
    return 0;
}