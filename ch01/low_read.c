#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 100

void error_handling(char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(-1);
}

int main(int argc, char ** argv) {
	int fd;
	char buf[BUF_SIZE];

	fd = open("data.txt", O_RDONLY);
	if(fd == -1) {
		error_handling("open() error");
	}
	printf("file descriptor : %d\n", fd);

	if(read(fd, buf, sizeof(buf)) == -1) {
		error_handling("read error");
	}
	printf("message : %s", buf);
	close(fd);

	return 0;
}
