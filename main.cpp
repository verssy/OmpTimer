#include <thread>
#include <iostream>

#include "omp_timer.h"

void work()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
}

int main()
{
    TIMER(main);
    work();

    for (int i = 0; i < 5; i++) {
        INNER_TIMER(for);
        work();

        if (i == 1) {
            INNER_TIMER(if);
            work();
        }
    }

    work();
}
