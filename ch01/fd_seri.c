#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

void error_handling(char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(-1);
}

int main(int argc, char ** argv) {
	int fd1, fd2, fd3;
	fd1 = socket(PF_INET, SOCK_STREAM, 0);			// TCP
	fd2 = open("test.dat", O_CREAT | O_WRONLY | O_TRUNC);
	fd3 = socket(PF_INET, SOCK_DGRAM, 0);			// UDP

	printf("file descriptor 1 : %d\n", fd1);
	printf("file descriptor 2 : %d\n", fd2);
	printf("file descriptor 3 : %d\n", fd3);

	close(fd1); close(fd2); close(fd3);


	return 0;
}
