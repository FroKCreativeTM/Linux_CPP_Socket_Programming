#include <iostream>
#include <unistd.h>

const int BUF_SIZE = 30;

int main(int argc, char** argv) {
	int fds1[2], fds2[2];	// 각 프로세스가 쓸 파이프라인을 만들어준다.
	char str1[] = "Hello~";
	char str2[] = "Welcome to my server";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds1), pipe(fds2);
	pid = fork();

	if(pid == 0) { // 자식 프로세스인 경우
		write(fds1[1], str1, sizeof(str1));
		read(fds2[0], buf, BUF_SIZE);
		printf("Child proc output : %s \n", buf);
	} else {
		read(fds1[0], buf, BUF_SIZE);
		printf("Parent proc output : %s \n", buf);
		write(fds2[1], str2, sizeof(str2));
		sleep(3);
	}

	return 0;
}
