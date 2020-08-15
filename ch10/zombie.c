#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	pid_t pid = fork();

	if(pid == 0) { 	// if child process
		puts("This is child process");
	} else { 	// if parent process
		printf("This is parent process : %d \n", pid);
		sleep(30);
	}

	if(pid == 0) {
		puts("End of child process");
	} else {
		puts("End of parent process");
	}

	return 0;
}
