#ifndef OMP_TIMER_H
#define OMP_TIMER_H

#include <chrono>
#include <iostream>
#include <string>

class OmpTimer
{
public:
    OmpTimer(const std::string name, const bool isInner)
        : name(name), isInner(isInner), start(clock::now())
    {
        if (isInner) {
            timerDepth++;
        }

        printf("|%s %s [START]\n", std::string(2 * timerDepth, '-').c_str(), name.c_str());
    }

    ~OmpTimer()
    {
        printf("|%s %s [END] = %lfs\n", std::string(2 * timerDepth, '-').c_str(), name.c_str(),
            durca<std::chrono::nanoseconds>());
        if (isInner) {
            timerDepth--;
        }
    }

private:
    template<typename DurationUnit>
    double durca()
    {
        using namespace std::chrono;

        constexpr double multiplier =
            std::is_same_v<std::chrono::seconds, DurationUnit> ? 1
            : std::is_same_v<std::chrono::milliseconds, DurationUnit> ? 1e-3
            : std::is_same_v<std::chrono::microseconds, DurationUnit> ? 1e-6
            : 1e-9;

        return duration_cast<DurationUnit>(clock::now() - start).count() * multiplier;
    }

private:
    using clock = std::chrono::high_resolution_clock;

    const std::string name;
    const bool isInner;
    const clock::time_point start;
    static inline int64_t timerDepth = 0;
#pragma omp threadprivate(timerDepth)
};

#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)
#define CREATE_TIMER(timerVariableName, timerName, isInner) timerVariableName(timerName, isInner)
#define TIMER(name) OmpTimer CREATE_TIMER(CONCAT(__local_timer_, __LINE__), #name, false)
#define INNER_TIMER(name) OmpTimer CREATE_TIMER(CONCAT(__local_timer_, __LINE__), #name, true)

#endif
