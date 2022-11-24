#include <iostream>
#include <omp.h>

constexpr int num_of_steps = 100000000;
constexpr double step_len = 1.0/num_of_steps;
constexpr int num_of_threads = 2;
constexpr int steps_per_segment = num_of_steps/num_of_threads;

double serial_sum = 0.0;
double parallel_sum = 0.0;

double no_threads_solution(void){
	double x;

	for(auto i=0; i<num_of_steps; i++){
		x = (i+0.5) * step_len;
		serial_sum += 4.0/(1.0 + (x*x));
	}

	return serial_sum*step_len;
}

void parallel_solution(){
	omp_set_num_threads(num_of_threads);
	#pragma omp parallel
	{
		auto id = omp_get_thread_num();

		int start_index = id * steps_per_segment;
		int end_index = start_index + steps_per_segment;

		double x;
		//local_sum to solve false sharing problem
		double local_sum = 0;

		for(auto i= start_index; i<end_index; i++){
			x = (i+0.5) * step_len;
			local_sum += 4.0/(1.0 + (x*x));
		}

		//parallel_sum is shared between all threads
		#pragma omp critical
		parallel_sum += local_sum * step_len;
	}
}

int main(){
	
	//Sequential Solution
	auto t1 = omp_get_wtime();
	no_threads_solution();
	auto t2 = omp_get_wtime();

	
	auto t3 = omp_get_wtime();
	parallel_solution();
	auto t4 = omp_get_wtime();

	std::cout << "Result of the normal solution: " << serial_sum << std::endl;
	std::cout << t2-t1 << " elapsed for the normal solution\n\n";
	
	std::cout << "Result of the parallel solution: " << parallel_sum << std::endl;
	std::cout << t4-t3 << " elapsed for the parallel solution\n";
}