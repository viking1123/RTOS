#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#define SERVER_KEY_PATHNAME "/tmp/mqueue_server_key"
#define PROJECT_ID 'M'
#define QUEUE_PERMISSIONS 0660

struct message_text {
    int qid;
    char buf [200];
};

struct message {
    long message_type;
    struct message_text message_text;
};

int main (int argc, char **argv)
{
    FILE *fp;
    char file_name[20];
    char buffer[100];
    key_t msg_queue_key;
    int qid;
    struct message message;
    clock_t t;

    if ((msg_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1) 
    {
        perror ("ftok");
        exit (1);
    }

    if ((qid = msgget (msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) 
    {
        perror ("msgget");
        exit (1);
    }

    printf ("Hello, welcome to server !\n");

    while (1) 
    {
        t = clock();
        // read an incoming message
        if (msgrcv (qid, &message, sizeof (struct message_text), 0, 0) == -1)
        {
            perror ("msgrcv");
            exit (1);
        }

    printf ("message received.\n");

    strcpy(file_name,message.message_text.buf);

  //Opeining input file, throw an error if invalid file
    fp= fopen(file_name, "r");
    if(fp == NULL)
    {
        printf("Cannot open file = %s\n", file_name);
        return 1;
    }
    //Copy input characters into msg.buffer, loops breaks when EOF is reached
    int i = 0;
    while(1)
    {
        buffer[i] = fgetc(fp);
        if(buffer[i]==EOF)
        {
                buffer[i] = '\0';
                break;
        }
        i++;
        }

    strcpy(message.message_text.buf,buffer);

        int client_qid = message.message_text.qid;
        message.message_text.qid = qid;

        // send reply message to client
        if (msgsnd (client_qid, &message, sizeof (struct message_text), 0) == -1)
     {  
            perror ("msgget");
            exit (1);
        }

     printf("response has been sent to client.\n");
     t = clock() - t;
     double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    
    printf("%f seconds taken for this request \n", time_taken); 

}
}