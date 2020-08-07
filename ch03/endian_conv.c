#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char ** argv) {
	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned int host_addr = 0x12345678;
	unsigned int net_addr;

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("host port : %#x\n", host_port);
	printf("net port : %#x\n", net_port);
	printf("host addresss : %#x\n", host_addr);
	printf("net address : %#x\n", net_addr);

	return 0;
}
