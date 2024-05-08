#include <iostream>
#include <thread>

#include "../omp_timer.h"

void Work()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(300ms);
}

void RecursiveFunction(const int64_t depth = 3)
{
    OmpTimer timer("RecursiveFunc", true);
    if (depth > 0) {
        Work();
        RecursiveFunction(depth - 1);
    }
}
void InnerFunction()
{
    OmpTimer timer("InnerFunctionCallFunc", true);
    Work();
}
void TestRecursionAndInnerFunction()
{
    OmpTimer::Reset();

    OmpTimer funcTimer(__FUNCTION__, false);
    RecursiveFunction();
    InnerFunction();
    funcTimer.Stop();

    printf(
        "EXPECTED:\n"
        "%s=1.2s\n"
        " |-RecursiveFunc=0.9s\n"
        " |-InnerFunctionCallFunc=0.3s\n\n",
        __FUNCTION__);

    printf("ACTUAL:\n");
    OmpTimer::PrintDurations();
}

int main()
{
    TestRecursionAndInnerFunction();
}
