#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

void* thread_main(void* arg) {
	int i;
	int cnt = *((int *)arg);

	for(i = 0; i < cnt; i++) {
		//sleep(1);
		std::cout << "running thread" << std::endl;
	}

	return nullptr;
}

int main(int argc, char** argv) {
	pthread_t t_id;
	int thread_param = 5;

	if(pthread_create(&t_id, nullptr, thread_main, reinterpret_cast<void*>(&thread_param)) != 0) {
		std::cout << "pthread create() error" << std::endl;
		return -1;
	}

	sleep(10);
	std::cout << "end of main" << std::endl;

	return 0;
}
