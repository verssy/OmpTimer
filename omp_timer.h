#ifndef OMP_TIMER_H
#define OMP_TIMER_H

#include <iostream>

struct OmpTimer {
    static inline int64_t timerDepth = 0;

    explicit OmpTimer(const bool isInner) : isInner(isInner)
    {
        if (isInner) {
            timerDepth++;
        }

        printf("Timer[inner = %d, depth = %d] START\n", isInner, timerDepth);
    }

    ~OmpTimer()
    {
        printf("Timer[inner = %d, depth = %d] END\n", isInner, timerDepth);

        if (isInner) {
            timerDepth--;
        }
    }

private:
    const bool isInner;
};

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)
#define CREATE_TIMER(name, boolVal) name(boolVal)
#define TIMER OmpTimer CREATE_TIMER(CONCAT(__local_timer_, __LINE__), false)
#define INNER_TIMER OmpTimer CREATE_TIMER(CONCAT(__local_timer_, __LINE__), true)

#endif
