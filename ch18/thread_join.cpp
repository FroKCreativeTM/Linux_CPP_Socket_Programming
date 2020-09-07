#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

void* thread_main(void* arg) {
	int i;
	int cnt = *((int*)arg);

	char* message = new char[50];
	strcpy(message, "Hello This is a thread\n");

	for(i = 0; i < cnt; i++) {
		std::cout << "running thread" << std::endl;
	}

	return (void*) message;
}

int main(int argc, char ** argv) {
	pthread_t t_id;
	int thread_param = 5;
	void* thr_ret = nullptr;

	if(pthread_create(&t_id, nullptr, thread_main, (void*)&thread_param) != 0) {
		std::cout << "pthread_create() error" << std::endl;
		return -1;
	}

	if(pthread_join(t_id, &thr_ret) != 0) {
		std::cout << "pthread_join() error" << std::endl;
		return -1;
	}

	std::cout << "thread return message : " << (char*)thr_ret << std::endl;
	
	if(thr_ret) {
		delete[] thr_ret;
	}
	thr_ret = nullptr;

	return 0;
}
