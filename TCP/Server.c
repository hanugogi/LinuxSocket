#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#define SOCK 1024
#define SOCK_SIZE sizeof(struct sockaddr_in)
#define TYPE SOCK_STREAM

int main(int argc, char *argv[]){
	int socket_listen, socket_connect;

	if(argc < 2){
		printf("Usage: %s [ip address] [port]\n", argv[0]);
		exit(0);
	}
	else if(argc < 3){
		printf("Usage: %s %s [port]\n", argv[0], argv[1]);
		exit(0);
	}

	struct sockaddr_in server;
	sockaddr_in.sin_family = AF_INET;
	sockaddr_in.sin_port = argv[2];
	sockaddr_in.sin_addr.s_addr = argv[1];

	struct sigaction act;
	act.sa_handler = closesock;
	sigfillset(&(act.sa_mask));
	sigaction(SIGPIPE, &act, NULL);

