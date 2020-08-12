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
	socklen_t addr_size;	// 불필요해진 변수
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

	// connected UDP를 위한 함수
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	while(1) {
		fputs("Insert message(q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		
		if(!strcmp("q\n", message) || !strcmp("Q\n", message)) 
			break;

		// sendto가 아니라 write를 이용해서 직접 쓴다.
		// connected의 다른 점은 역시 IP와 PORT를 이용해서 직접
		// 목적지를 알려준다!
		// 이 경우 하나의 호스트와 오랜 시간 데이터를 송수신하는 경우에 유리하다.
		write(sock, message, strlen(message));
		
		addr_size = sizeof(from_addr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &addr_size);

		message[str_len] = 0;
		printf("Message from server : %s", message);
	}
	close(sock);
	return 0;
}
