#include <iostream>

#include "../omp_timer.h"

int main()
{
    printf("TESTS EXECUTION:\n");
    OmpTimer timer("Testing main", false);

    timer.Stop();
    OmpTimer::PrintDurations();
}