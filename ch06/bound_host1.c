#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message) {
	fputs(message, stderr);

