#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 1024

int main(int argc, char *argv[]){
	char buff[SIZE], temp[20];
	int len, msg_size;

	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;

	if(argc != 2){
		printf("usage: %s [port]\n", argv[0]);
		exit(0);
	}

	if(server_fd = socket(AF_INET, SOCK_STREAM, 0) == -1){
		printf("Server: Can't open stream socket\n");
		exit(0);
	}
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		printf("Server: Cannot bind local address.\n");
		exit(0);
	}

	if(listen(server_fd, 5) < 0){
		printf("Server: Cannot listening Content\n");
		exit(0);
	}

	memset(buff, 0, sizeof(buff));
	printf("Server: Waiting Connection Request.\n");
	
	len = sizeof(client_addr);

	while(1){
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
		if(client_fd < 0){
			printf("ServerL accept Failed\n");
			exit(0);
		}

		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
		printf("Server: %s Client Connectd.\n", temp);

		msg_size = read(client_fd, buff, 1024);
		printf("Client >> %s\n", buff);
		
		memset(buff, 0x00, sizeof(buff));
		printf("Server: Put Message\n>>");
		scanf("%s", buff);
		write(client_fd, buff, sizeof(buff));

		close(client_fd);
		printf("Server: Client Closed.\n");
	}
}
