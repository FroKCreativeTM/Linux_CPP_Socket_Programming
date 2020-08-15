// sigaction은 signal함수보다 많이 사용한다.
// 그 이유는 signal 함수는 유닉스 계열마다 동작 방식이 다르지만
// sigaction은 차이가 거의 없기 때문이다.

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
	if(sig == SIGALRM) {
		puts("Time out");
	}
	alarm(2);
}

int main(int argc, char** argv) {
	int i;
	struct sigaction act;
	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, 0);

	alarm(2);

	for(i = 0; i < 3; i++) {
		puts("wait....");
		sleep(100);
	}

	return 0;
}
