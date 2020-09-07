#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

const int NUM_THREAD = 100;
long long num = 0;
pthread_mutex_t mutex;

void * thread_inc(void* arg) {
	// 뮤텍스를 이용해서 공유 자원을 이 함수만 이용할 수 있게 잠군다.
	pthread_mutex_lock(&mutex);
	{
		for(int i = 0; i < 50000000; i++) {
			num += i;
		}
	}
	pthread_mutex_unlock(&mutex);
	return nullptr;
}

void * thread_dsc(void* arg) {
	pthread_mutex_lock(&mutex);
	for(int i = 0; i < 50000000; i++) {
		num -= i;
	}
	pthread_mutex_unlock(&mutex);
	return nullptr;
}

int main(int argc, char** argv) {
	pthread_t thread_id[NUM_THREAD];

	pthread_mutex_init(&mutex, nullptr);

	for(int i = 0; i < NUM_THREAD; i++) {
		if(i % 2) {
			pthread_create(&(thread_id[i]), nullptr, thread_inc, nullptr);
		} else { 
			pthread_create(&(thread_id[i]), nullptr, thread_dsc, nullptr);
		}
	}

	for(int i = 0; i < NUM_THREAD; i++) {
		pthread_join(thread_id[i], nullptr);
	}

	std::cout << "result : " << num << std::endl;
	pthread_mutex_destroy(&mutex);

	return 0;
}
