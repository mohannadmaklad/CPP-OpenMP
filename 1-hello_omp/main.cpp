#include <omp.h>
#include <iostream>

int main(void){
#pragma omp parallel
{
    int ID = omp_get_thread_num();
    std::cout << "hello " << ID << std::endl << "world " << ID << std::endl;
}
    
}