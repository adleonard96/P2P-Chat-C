/*
 * server.c
 *
 *      Author: ALeonard, JThomas
 */
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr



int sockfd1F;
int sockfd2F;
int is_Alive = 1;

void *send_recieve1 (void *_args){
	char buff[MAX];

	for(;;){
		bzero(buff, MAX);
		read(sockfd1F, buff, sizeof(buff));
		write(sockfd2F, buff, sizeof(buff));
		// if msg contains "Exit" then server exit and chat ended.
		if ((strncmp("exit", buff, 4) == 0) || (is_Alive = 0)) {
			printf("Server Exit...\n");
			is_Alive = 0;
			break;
		}
	}
	
	pthread_exit(NULL);
}

void *send_recieve2 (void *_args){
	char buff[MAX];
	for(;;){
		bzero(buff, MAX);
		read(sockfd2F, buff, sizeof(buff));
		write(sockfd1F, buff, sizeof(buff));
		// if msg contains "Exit" then server exit and chat ended.
		if ((strncmp("exit", buff, 4) == 0) || (is_Alive = 0)) {
			printf("Server Exit...\n");
			is_Alive = 0;
			break;
		}
	}
	
	pthread_exit (NULL);
}	
void chatRoom(int sockfd, int sockfd2)
{
	
	

	pthread_t thread_id1;
	pthread_t thread_id2;
	sockfd1F = sockfd;
	sockfd2F = sockfd2;
	pthread_create(&thread_id1, NULL, send_recieve1, NULL);
	pthread_create(&thread_id2, NULL, send_recieve2, NULL);
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL);
}

// Driver function
int main()
{
	
	int sockfd, sockfd2, connfd, len;
	int userCount = 0;
	int userArray[2];
	struct sockaddr_in servaddr, cli;
	

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}	
	else
		//printf("Socket successfully created..\n");
		
	
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	
	
	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		//printf("Socket successfully binded..\n");
	
	while(userCount < 2){
	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		//printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	userArray[userCount] = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		//printf("server acccept the client...\n");
		userCount ++;
	}
	
	// Function for chatting between client and server
	chatRoom(userArray[0], userArray[1]);

	// After chatting close the socket
	close(sockfd);
	sleep(2);
	
	
}


