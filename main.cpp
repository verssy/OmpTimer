#include <omp.h>

#include <cstdlib>
#include <iostream>
#include <thread>

#include "omp_timer.h"

const int64_t numThreads = std::getenv("OMP_NUM_THREADS") ? std::min(omp_get_max_threads(), std::atoi(std::getenv("OMP_NUM_THREADS"))) : 1;

void Work()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
}

int main()
{
    TIMER(main);
    Work();
#pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < 5; i++) {
        INNER_TIMER(for);
        Work();
    }

    Work();
}
