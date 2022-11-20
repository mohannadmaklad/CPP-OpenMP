#include <iostream>
#include <omp.h>

constexpr int num_of_steps = 100000000;
constexpr double step_len = 1.0/num_of_steps;
constexpr int num_of_threads = 8;
constexpr int steps_per_segment = num_of_steps/num_of_threads;

double sum = 0.0;
double parallel_sum[num_of_threads] = {0.0};

double no_threads_solution(void){
	double x;

	for(auto i=0; i<num_of_steps; i++){
		x = (i+0.5) * step_len;
		sum += 4.0/(1.0 + (x*x));
	}

	return sum*step_len; //total area
}

int main(){
	auto t1 = omp_get_wtime();
	//std::cout<< "solution " << no_threads_solution() << std::endl;
	no_threads_solution();
	auto t2 = omp_get_wtime();

	//reset and try the parallel solution
	sum = 0.0;


	//2 threads
	omp_set_num_threads(num_of_threads);

	auto t3 = omp_get_wtime();
	#pragma omp parallel
	{
		auto id = omp_get_thread_num();
		//std::cout << "id " << id << std::endl; 
		auto actual_num_of_threads = omp_get_num_threads();
		//std::cout << "actual num of threads " << actual_num_of_threads << std::endl;
		double x;

		int start_index = id * steps_per_segment;
		int end_index = start_index + steps_per_segment;
		for(auto i= start_index; i< end_index; i++){
			x = (i+0.5) * step_len;
			parallel_sum[id] += 4.0/(1.0 + (x*x));
		}

		parallel_sum[id] *= step_len;
	}

	double res2 = 0.0;
	for(auto i=0; i<num_of_threads; i++) res2 += parallel_sum[i];
	//std::cout << "res2 " << res2 << std::endl;
	auto t4 = omp_get_wtime();

	std::cout << t2-t1 << " elapsed for the normal solution\n";
	std::cout << t4-t3 << " elapsed for the parallel solution\n";

}