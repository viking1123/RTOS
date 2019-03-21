#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){
	//clock_t t;
	int n;
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf(" Socket created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Connected \n");
		
		
n=recv(clientSocket, buffer, sizeof(buffer), 0);

					buffer[n]='\0';
			printf("Server: \t%s\n", buffer);
	




FILE *fp;


/*
fp=fopen("parameters.csv","w+");
fprintf(fp,"parameter1,parameter2,parameter3,parameter4,parameter5 \n");
fclose(fp);
*/

fp=fopen("parameters.csv","a+");


fprintf(fp,buffer);

fprintf(fp,"\n");
fclose(fp);
	

	return 0;
}
