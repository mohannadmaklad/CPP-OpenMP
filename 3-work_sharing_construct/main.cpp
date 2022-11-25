#include <iostream>
#include <omp.h>

constexpr int total_work = 100;
constexpr int chunk_size = 10;

void static_schedule(void){
	//#pragma omp parallel for schedule(static) // equivalent to #pragma omp parallel for schedule(static,1)
	#pragma omp parallel for schedule(static,chunk_size)

		for(int i=0; i<total_work;i++){
			#pragma omp critical
			std::cout << "i= " << i << " " << "thread num " << omp_get_thread_num() << std::endl;
		}
}

void dynamic_schedule(void){
	#pragma omp parallel for schedule(dynamic,chunk_size)

		for(int i=0; i<total_work;i++){
			#pragma omp critical //Doesn't make sense but just to get a clean output
			std::cout << "i= " << i << " " << "thread num " << omp_get_thread_num() << std::endl;
		}
}


int main(){
	static_schedule();
	std::cout << "Dynamic:" << std::endl;
	dynamic_schedule();
}