#include <stdio.h>
#include <unistd.h>

int g_val = 10;
int main(int argc, char** argv) {
	pid_t pid;
	int lval = 20;
	g_val++, lval += 5;

	pid = fork();
	if(pid == 0) { 	// if child process
		g_val += 2, lval += 2;
	} else {	// if parent process
		g_val -= 2, lval -= 2;
	}

	if(pid == 0) {
		printf("Child process [%d %d] \n", g_val, lval);
	} else {
		printf("Parent process : [%d %d] \n", g_val, lval);
	}

	return 0;
}
