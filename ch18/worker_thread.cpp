#include <cstdio>
#include <iostream>
#include <pthread.h>

int g_sum = 0;

void* thread_summation(void* arg) {
	int start = ((int*)arg)[0];
	int end = ((int*)arg)[1];

	while(start <= end) {
		g_sum += start;
		start++;
	}

	return nullptr;
}

int main(int argc, char ** argv) {
	pthread_t id_t1, id_t2;
	int range1[] = {1, 5};
	int range2[] = {6, 10};

	pthread_create(&id_t1, nullptr, thread_summation, (void*)range1);
	pthread_create(&id_t2, nullptr, thread_summation, (void*)range2);

	pthread_join(id_t1, nullptr);
	pthread_join(id_t2, nullptr);

	std::cout << "result : " << g_sum << std::endl;

	return 0;
}
