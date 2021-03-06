#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

using namespace std;

const int BUF_SIZE = 1024;
const int EPOLL_SIZE = 50;

void SetNonblockingMode(int fd) {
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(-1);
}

int main(int argc, char** argv) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	int str_len, i;
	char buf[BUF_SIZE];

	struct epoll_event * ep_events;
	struct epoll_event event;
	int epfd, event_cnt;

	if(argc != 2) {
		cout << "Usage : " << argv[0] << " <PORT>" << endl;
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0,  sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind error");
	}
	if(listen(serv_sock, 5) == -1) {
		error_handling("listen error");
	}

	epfd = epoll_create(EPOLL_SIZE);
	ep_events = (epoll_event *)malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while(true) {
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);

		// 에러 발생
		if(event_cnt == -1) {
			puts("epoll_wait error");
			break;
		}

		puts("return epoll_wait");
		for(i = 0; i < event_cnt; i++) {
			if(ep_events[i].data.fd == serv_sock) {
				clnt_addr_size = sizeof(clnt_addr);

				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_sock, &clnt_addr_size);
				SetNonblockingMode(clnt_sock);
				event.events = EPOLLIN | EPOLLET;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				cout << "connected client : " << clnt_sock << endl;
			} else {

				while(true) {
					str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);

					if(str_len == 0) {
						epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
						close(ep_events[i].data.fd);
						cout << "closed client : " << ep_events[i].data.fd << endl;
						break;
					} else if(str_len < 0) {
						if(errno == EAGAIN) {
							break;
						}
					} else {
						write(ep_events[i].data.fd, buf, str_len);
					}
				}
			}
		}
	}

	close(serv_sock);
	close(epfd);

	return 0;
}
