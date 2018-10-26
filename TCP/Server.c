#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 70048
#define SOCK_SIZE sizeof(struct sockaddr_in)
#define TYPE SOCK_STREAM

void closesock(int signo);
 
int socket_server, socket_client, header_fd, html_fd;

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Message: %s [port]\n", argv[0]);
		exit(0);
	}

	struct sockaddr_in server_in, client_in;
	server_in.sin_family = AF_INET;
	server_in.sin_port = htons(atoi(argv[1]));
	server_in.sin_addr.s_addr = htonl(INADDR_ANY);

	char accepted = 0;
	int recv_length, send_length, client_length = sizeof(client_in);
	int pid, header_fd, html_fd;
	char *command[10], recv_buff[1024], send_buff[SIZE] = {0};
	printf("Message: Initalize Valuable\n");

	if((socket_server = socket(AF_INET, TYPE, 0)) < 0){
		perror("Message: Fail to make Socket");
		exit(0);
	}
	printf("Message: Make Server Socket\n");

	struct sigaction act;
	act.sa_handler = &closesock;
	sigfillset(&(act.sa_mask));
	perror("sigfillset");
	sigaction(SIGPIPE, &act, NULL);
	perror("sigaction");

	if(bind(socket_server, (struct sockaddr_in*)&server_in, sizeof(server_in)) < 0){
		perror("Message: Can't bind Socket");
		exit(0);
	}
	printf("Server IP: %d\n", ntohl(server_in.sin_addr.s_addr));

	if(listen(socket_server,  5) < 0){
		perror("Message: Fail to Make Listen Queue");
		exit(0);
	}
	printf("Message: Listening Other Socket...\n\n");


	while(1){
			if(((socket_client = accept(socket_server, (struct sockaddr_in*)&client_in, &client_length)) < 0)){
				perror("Message: Accept Failed");
				sleep(1);
				continue;
			}
			if((pid=fork()) == 0)
				continue;

			if((recv_length = recv(socket_client, recv_buff, SIZE, 0)) < 0){
				perror("Message: Failed to Recieve Data");
				exit(0);
			}
			printf("Read %d bytes\n", recv_length);

			command[0] = strtok(recv_buff, " ");
			command[1] = strtok(NULL, " ");
			char *temp = command[1];command[1] = ".";
			strcat(command[1], temp);

			command[2] = strtok(NULL, " ");command[2][8] = '\0';
			printf("Command: %s, %s, %s\n\n", command[0], command[1], command[2]);

			if(command[0] == "GET")
				printf("Called by GET Method\n");
			else if(command[0] == "POST")
				printf("Called by POST Method\n");
			else
				printf("Unkwon Method\n");

			if(strcmp(command[1], "./") == 0)
				command[1] = "./index.html";

			printf("Request \"%s\" File\n", command[1]);
			
			printf("HTML Virsion is %s\n\n", command[2]);

			if((header_fd = open(command[1], O_RDONLY)) < 0){
				perror("Open");
				header_fd = open("404.html", O_RDONLY);
			}
			read(header_fd, send_buff, SIZE);
			perror("Read");
			
			if((send_length = send(socket_client, send_buff, strlen(send_buff), 0)) < 0){
				perror("Message: Failed to Send Data");
				exit(0);
			}
			printf("Message: Send %d bytes\n\n", send_length);
	}

}

void closesock(int signo){
	close(socket_server);
	close(socket_client);
	close(header_fd);
	close(html_fd);
}
