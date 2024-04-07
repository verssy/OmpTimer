#include <iostream>

#include "omp_timer.h"

int main()
{
    TIMER;
    for (int i = 0; i < 5; i++) {
        INNER_TIMER;
    }
}
