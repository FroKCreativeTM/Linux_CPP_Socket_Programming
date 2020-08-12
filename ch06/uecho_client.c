#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(-1);
}

int main(int argc, char ** argv) {
	int sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t addr_size;
	struct sockaddr_in serv_addr, from_addr;

	if(argc != 3) {
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	while(1) {
		fputs("Insert message(q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		
		if(!strcmp("q\n", message) || !strcmp("Q\n", message)) 
			break;

		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		addr_size = sizeof(from_addr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &addr_size);
		message[str_len] = 0;
		printf("Message from server : %s", message);
	}
	close(sock);
	return 0;
}
