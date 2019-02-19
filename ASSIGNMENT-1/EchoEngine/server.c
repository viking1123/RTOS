/*
 SERVER-receives messages from clients
 */
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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
    key_t msg_queue_key;
    int qid;
    char ch;
    int i=0;
    int n=0;
    char a[100];
    struct message message;
    clock_t t;

    if ((msg_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1) {
        perror ("ftok");
        exit (1);
    }

    if ((qid = msgget (msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
        perror ("msgget");
        exit (1);
    }

    printf ("Server: Hello, World!\n");

    while (1) {
        // read an incoming message

        t=clock();
        if (msgrcv (qid, &message, sizeof (struct message_text), 0, 0) == -1) {
            perror ("msgrcv");
            exit (1);
        }

        //printf ("Server: message received.\n");

        // process message
        
        int length = strlen (message.message_text.buf);
        char buf [20];
        sprintf (buf, " %d", length);
        strcat (message.message_text.buf, buf);
        

        int client_qid = message.message_text.qid;
        message.message_text.qid = qid;

        strcpy(a, message.message_text.buf);

        while (a[n] != '\0') 
        {
            ch = a[n];
            if (ch >= 'A' && ch <= 'Z')
                a[n] = a[n] + 32;
            else if (ch >= 'a' && ch <= 'z')
                a[n] = a[n] - 32;  
            n++;  
        }

         strcpy(message.message_text.buf,a);

        // send reply message to client
        if (msgsnd (client_qid, &message, sizeof (struct message_text), 0) == -1) {  
            perror ("msgget");
            exit (1);
        }

        t=clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;

        printf ("Server: %d characters received\n",length);
        printf("\n %f seconds taken..", time_taken);
    }
}