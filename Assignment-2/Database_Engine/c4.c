//---------------------------client.c----------------------
//client thread receiving file
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

int main(int argc, char **argv)
{
 int sd;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];
 clock_t t;

 //basic check of the arguments
 //additional checks can be inserted
 if (argc !=2) {
  perror("Usage: TCPClient <IP address of the server");
  exit(1);
 }

 //Create a socket for the client
 //If sd<0 there was an error in the creation of the socket
 if ((sd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }

 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order

 //Connection of the client to the socket
 if (connect(sd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }
else
printf("\nconnected to server now.\n");
/*file start*/

	int n;
	char buf_recv[1000];
	char buf_send[1000];
	char *filename;
	char file_buffer[1000];
	FILE *fp;
	//code for receivind statement "Enter the file name"
	n = recv(sd, buf_recv, sizeof(buf_recv), 0);
	buf_recv[n]='\0';
	printf("%s",buf_recv);


printf("\n\nNow sending the file name: \n\n");
scanf("%s",buf_send);

t=clock();

send(sd,buf_send,strlen(buf_send),0);


printf("\n\n receiving the string \"File found or not\"\n\n");
	n = recv(sd, buf_recv, sizeof(buf_recv), 0);
	buf_recv[n]='\0';
	printf("%s",buf_recv);
	fflush(stdout);


printf("\n\n Receiving the file content \n\n");
n=recv(sd, file_buffer, sizeof(file_buffer), 0);

t=clock()-t;
double time_taken=((double)t)/CLOCKS_PER_SEC;

printf ("\n %f sec taken ",time_taken);

file_buffer[n]='\0';
fflush(stdout);

fp = fopen("received_file.txt","w");
fputs(file_buffer,fp);
fclose(fp);
close(sd);

return 0;
}
