#include <omp.h>

#include <cstdlib>
#include <iostream>
#include <thread>

#include "omp_timer.h"

void InitOmp()
{
    const char* ompEnvThreads = std::getenv("OMP_NUM_THREADS");

    omp_set_dynamic(0);
    int numThreads = 1;
    if (ompEnvThreads) {
        numThreads = std::min(std::atoi(ompEnvThreads), omp_get_max_threads());
    }

    omp_set_num_threads(numThreads);
}

void Work()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
}

int main()
{
    InitOmp();

    const int64_t myThread = omp_get_thread_num();
    const int64_t numThreads = omp_get_num_threads();

    if (myThread == 0) {
        printf("Threads provided = %d\n", numThreads);
    }

    TIMER(main);
    Work();
#pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < 5; i++) {
        INNER_TIMER(for);
        Work();

        if (i == 1) {
            INNER_TIMER(if);
            Work();
        }
    }

    Work();
}
