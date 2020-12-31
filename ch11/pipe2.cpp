#include <iostream>
#include <unistd.h>

const int BUF_SIZE = 30;

int main(int agrc, char ** argv) {
	int fds[2];				// 입출구 파일 디스크립터
	char str1[] = "Hello~";
	char str2[] = "Thanks for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();

	if(pid == 0) {	// 자식 프로세스는 부모 프로세스 다음에 실행한다.
		write(fds[1], str1, sizeof(str1));
		sleep(2);			// 만약 여기를 지운다면 파이프라인은 자식 프로세스가 먼저 선점하고 끝내버리기 때문에 좀비 프로세스가 된다!!
		read(fds[0], buf, BUF_SIZE);
		printf("Child proc output : %s\n", buf);
	} else {	// 여기부터 실행된다. (그리고 끝난다)
		read(fds[0], buf, BUF_SIZE);
		printf("Parent proc output : %s\n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}

	return 0;
}
