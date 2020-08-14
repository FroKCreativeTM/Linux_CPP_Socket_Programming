// 이 프로그램은 reuseaddr_eserver.c의 문제점을 해결한 프로그램입니다..
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(-1);
}

int main(int argc, char** argv) {
	int serv_sock, clnt_sock;
	char message[30];
	int option, str_len;
	socklen_t optlen, clnt_addr_size;
	struct sockaddr_in serv_addr, clnt_addr;
	
	if(argc != 2) {
		printf("Usage : %s <PORT>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1) {
		error_handling("socket() error");
	}

	// time and wait를 위한 코드 부분
	optlen = sizeof(option);
	option = TRUE;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
			error_handling("bind() error");
	}
	if(listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

	while((str_len = read(clnt_sock, message, sizeof(message))) != 0) {
		write(clnt_sock, message, str_len);
		write(1, message, str_len);
	}

	close(serv_sock);
	close(clnt_sock);

	return 0;
}
