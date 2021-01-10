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
const int MAX_CLNT = 256;

int clnt_cnt = 0;			// This variable Must be locked!
int clnt_socks[MAX_CLNT];
pthread_mutex_t t_mutex;

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(-1);
}

void SendMessage(const char * message, int len) {
	int i;
	pthread_mutex_lock(&t_mutex); 
	{
		for(i = 0; i < clnt_cnt; ++i) {
			write(clnt_socks[i], message, len);
		}
	}
	pthread_mutex_unlock(&t_mutex);
}

void * HandleClient(void * arg) {
	int clnt_sock = *((int*)arg);
	int str_len = 0, i;
	char message[BUF_SIZE];

	while((str_len = read(clnt_sock, message, sizeof(message))) != 0) {
		SendMessage(message, str_len);
	}

	pthread_mutex_lock(&t_mutex);
	{
		for(i = 0; i < clnt_cnt; ++i) {
			if(clnt_sock == clnt_socks[i]) {
				while(i++ < clnt_cnt - 1) {
					clnt_socks[i] = clnt_socks[i + 1];
				}
				break;
			}
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&t_mutex);

	close(clnt_sock);
	return nullptr;
}

int main(int argc, char** argv) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
 	socklen_t clnt_size;
	pthread_t t_id;

	if(argc != 2) {
		cerr << "Usage : " << argv[0] << " <PORT>" << endl;
		exit(1);
	}

	pthread_mutex_init(&t_mutex, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind error");
	} 
	if(listen(serv_sock, 5) == -1) {
		error_handling("listen error");
	}

	while(true) {
		clnt_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_size);

		pthread_mutex_lock(&t_mutex);
		{
			clnt_socks[clnt_cnt++] = clnt_sock;
		}
		pthread_mutex_unlock(&t_mutex);

		pthread_create(&t_id, NULL, HandleClient, reinterpret_cast<void*>(&clnt_sock));
		pthread_detach(t_id);
		cout << "Connected client IP : " << inet_ntoa(clnt_addr.sin_addr) << endl;
	}

	close(serv_sock);
	return 0;
}
