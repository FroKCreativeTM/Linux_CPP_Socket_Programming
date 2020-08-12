#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(-1);
}

int main(int argc, char ** argv) {
	int i;
	struct hostent* host;

	if(argc != 2) {
		printf("");

	return 0;
}
