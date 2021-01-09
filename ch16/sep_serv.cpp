#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 1024;

/* 스트림이 분리된 표준 함수를 사용한 단순 메시지 보내기 프로그램입니다. */
int main(int argc, char** argv) {
	int serv_sock, clnt_sock;
	FILE * readfp;
	FILE * writefp;

	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	char buf[BUF_SIZE] = {0,};

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(serv_sock, 5);
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

	readfp = fdopen(clnt_sock, "r");
	writefp = fdopen(clnt_sock, "w");

	fputs("FROM SERVER : Hi client? \n", writefp);
	fputs("THIS IS TEST FOR HALF-CLOSE \n", writefp);
	fflush(writefp);

	fclose(writefp);
	fgets(buf, sizeof(buf), readfp);
	fputs(buf, stdout);
	fclose(readfp);

	return 0;
}
