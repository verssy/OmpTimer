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

// Temporary wrapper instead of unrealized method:
//   OmpTimer::Stop()
struct OmpTimerHandler {
    ~OmpTimerHandler()
    {
        printf("TIMER MEASUREMENTS:\n");
        OmpTimer::PrintDurations();
    }
} __ompTimerHandler;

void RecursiveTest(const int64_t depth = 3)
{
    OmpTimer timer("RecursiveFunc", true, __LINE__);
    if (depth > 0) {
        Work();
        RecursiveTest(depth - 1);
    }
}

void InnerFunctionCallTest()
{
    OmpTimer timer("InnerFunctionCallFunc", true, __LINE__);
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

    OmpTimer timer("Test", true, __LINE__);

    RecursiveTest();
    InnerFunctionCallTest();
}

int main()
{
    OmpTimer timer("Main", false, __LINE__);

    Test();

    Work();
}
