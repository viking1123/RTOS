
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
	int hours, minutes, seconds;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1)
	{
		
		
	n=recv(clientSocket, buffer, sizeof(buffer), 0);
		buffer[n]='\0';
		printf("Parameters from Server:  %s ", buffer);
				
/*
	FILE *fp;
	int i;
	fp = fopen("parameters.csv","w+");
	fprintf(fp,"parameter1,parameter2,parameter3,parameter4,parameter5,time ");

	fclose(fp);	
*/


	time_t now;
	
	time(&now);

	struct tm *local = localtime(&now);

	hours = local->tm_hour;			// get hours since midnight	(0-23)
	minutes = local->tm_min;		// get minutes passed after the hour (0-59)
	seconds = local->tm_sec;		// get seconds passed after the minute (0-59)

	// print local time
	

	FILE *fp1;
	fp1 = fopen("parameters.csv","a+");
	fprintf(fp1,buffer);
	fprintf(fp1," , %02d:%02d:%02d \n", hours, minutes, seconds);
	fclose(fp1);
	
	printf(" taken at time : %02d:%02d:%02d \n", hours, minutes, seconds);

	}

	return 0;
}
