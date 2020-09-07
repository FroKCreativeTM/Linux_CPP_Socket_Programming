#include <cstdio>
#include <iostream>
#include <pthread.h>

const int NUM_THREAD = 100;
long long g_num = 0;

void* thread_inc(void* arg) {
	int i;
	for(i = 0; i < 50000000; i++) {
		g_num += 1;
	}
	return nullptr;
}

void* thread_dsc(void* arg) {
	int i;
	for(i = 0; i < 50000000; i++) {
		g_num -= 1;
	}
	return nullptr;
}	

int main(int argc, char** argv) {
	pthread_t thread_id[NUM_THREAD];
	int i;

	std::cout << "sizeof long long : " << sizeof(long long) << std::endl;
	for(i = 0; i < NUM_THREAD; i++) {
		if( i % 2 ) { 
			pthread_create(&(thread_id[i]), nullptr, thread_inc, nullptr);
		} else {
			pthread_create(&(thread_id[i]), nullptr, thread_dsc, nullptr);
		}
	}

	for(i = 0; i < NUM_THREAD; i++) {
		pthread_join(thread_id[i], nullptr);
	}

	std::cout << "result : " << g_num << std::endl;

	return 0;
}
