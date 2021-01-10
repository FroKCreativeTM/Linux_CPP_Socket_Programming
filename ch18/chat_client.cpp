#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

using namespace std;

const int BUF_SIZE = 1024;
const int NAME_SIZE = 20;

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

void* ReceiveMessage(void * arg) {
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;

	while(true) {
		str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);

		if(str_len == -1) {
			return (void*)-1;
		}
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}
	return nullptr;
}

void* SendMessage(void * arg) {
	int sock = *((int*)arg);
        char name_msg[NAME_SIZE + BUF_SIZE];

	while(true) {
		fgets(msg, BUF_SIZE, stdin);
		if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
			close(sock);
			exit(0);
		}
		sprintf(name_msg, "%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
	}
	return nullptr;
}

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(-1);
}

int main(int argc, char** argv) {
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t send_thread, recv_thread;
	void * thread_return;

	if(argc != 4) {
		cerr << "Usage : " << argv[0] << " <IP> <PORT> <NAME>" << endl;
		exit(1);
	}

	sprintf(name, "[%s]", argv[3]);
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connect error");
	}

	pthread_create(&send_thread, NULL, SendMessage, (void*)&sock);
	pthread_create(&recv_thread, NULL, ReceiveMessage, (void*)&sock);
	pthread_join(send_thread, &thread_return);
	pthread_join(recv_thread, &thread_return);
	close(sock);

	return 0;
}
