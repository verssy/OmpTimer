#include <omp.h>

#include <cstdlib>
#include <iostream>
#include <thread>

#include "omp_timer.h"

void Work()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(300ms);
}

void RecursiveTest(const int64_t depth = 3)
{
    OmpTimer timer("RecursiveFunc", true);
    if (depth > 0) {
        Work();
        RecursiveTest(depth - 1);
    }
}

void InnerFunctionCallTest()
{
    OmpTimer timer("InnerFunctionCallFunc", true);
    Work();
}

void Test()
{
    printf(
        "EXPECTED:\n"
        "Main=1.5s\n"
        " |-Test=1.2s\n"
        " | |-RecursiveFunc=0.9s\n"
        " | |-InnerFunctionCallFunc=0.3s\n\n");

    OmpTimer timer("Test", true);

    RecursiveTest();
    InnerFunctionCallTest();
}

int main()
{
    OmpTimer timer("Main", false);

    Test();
    Work();
    timer.Stop();

    printf("TIMER MEASUREMENTS:\n");
    OmpTimer::PrintDurations();
}
