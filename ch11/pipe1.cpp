#include <iostream>
#include <unistd.h>

using namespace std;

/* Global variable */
const int BUF_SIZE = 30;

int main(int argc, char ** argv) {
	int fds[2];				// 파이프의 입구와 출구로 쓸 파일 디스크립터(그래서 2개다).
	char str[] = "Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();

	if(pid == 0) {
		write(fds[1], str, sizeof(str));
	} else {
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}

	return 0;
}

