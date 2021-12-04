#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define MAXPORT 80
#define PORT 8080
#define SA struct sockaddr
#define defualtIP "127.0.0.1"
int sockfdGlobe;
char buffRead[MAXPORT] = {'a','b','c'};
int is_Alive = 1;

/*
 * client.c
 *
 *  Created on: Nov 25, 2021
 *      Author: ALeonard, JThomas
 */
void *read2(void *_args){
	int count = 0;
	char buff[MAXPORT];
	for(;;){
		bzero(buff, MAXPORT);
		read(sockfdGlobe, buff, sizeof(buff));
		if (is_Alive == 0) {
			//printf("Read thread exit\n");
			break;
		}
		if(count != 0)
			fprintf(stdout, "\nFrom other user: %sEnter a string:", buff);
		count++;
			// if msg contains "Exit" then server exit and chat ended.	
		
	}
	pthread_exit(NULL);
}
void *write2(void *_args){
	int count = 0;
	int n;
	is_Alive = 1;
	for(;;){
		bzero(buffRead, sizeof(buffRead));
		if(count != 0) 
			fprintf(stdout, "Enter a string: ");
		n = 0;
		count ++;
		bzero(buffRead, MAXPORT);
		while ((buffRead[n++] = getchar()) != '\n'){
		};
		write(sockfdGlobe, buffRead, sizeof(buffRead));
		if ((strncmp(buffRead, "exit", 4)) == 0) {
			//printf("Client Exit...\n");
			is_Alive = 0;
		}
		// if msg contains "Exit" then server exit and chat ended.
		if (is_Alive == 0) {
			//printf("client exit \n");
			break;
		}
	}
	pthread_exit(NULL);
}

void func(int sockfd)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	pthread_t thread_id1;
	pthread_t thread_id2;
	sockfdGlobe = sockfd;
	sleep(2);
	//creates thread that reads from server
	pthread_create(&thread_id1, NULL, read2, NULL);
	//creates thread that writes to server
	pthread_create(&thread_id2, NULL, write2 , NULL);
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL); 

}

int chatClient(char IP[])
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;


	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n\n");

	// function for chat
	func(sockfd);
	// close the socket
	close(sockfd);
}

int main (int argc, char **argv){
	char hostOrClient[4];
	printf("Would you like to \"Host\" or \"Join\"?\n"); 
	scanf("%s", hostOrClient);
	// logic if server is clicked
    if((strcmp(hostOrClient, "Host") == 0) || (strcmp(hostOrClient, "host") == 0)){
		printf("\n**Launching server**\n");
		system("./Server &");	
		sleep(2);//Need time for the sever to fire up 
		chatClient(defualtIP);
    }
    // logic if client is clicked
    if((strcmp(hostOrClient, "Join") == 0) || (strcmp(hostOrClient, "join") == 0)){
		char ip[16];
		printf("\nPlease enter the IP of the chat room you wish to join then press ENTER.\n");
		sleep(2);
		scanf("%s",ip);
		chatClient(ip);
    }
    printf("\n**** P2P CHAT HAS ENDED ****\n");
    return 0;
}
