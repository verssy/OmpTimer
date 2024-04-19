#ifndef OMP_TIMER_H
#define OMP_TIMER_H

#include <algorithm>
#include <chrono>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <string>

class OmpTimer {
private:
    struct Node {
        int64_t line;
        std::set<Node> childLines;

        Node() = default;

        bool operator<(const Node &another) const
        {
            return line < another.line;
        }
    };

    static inline Node root;
    static inline std::deque<int64_t> timersChain;

    static inline std::map<int64_t, int64_t> lineToTime;
    static inline std::map<int64_t, std::string> lineToName;

public:
    OmpTimer(const std::string name, const bool isInner, const int64_t line)
        : name(name), isInner(isInner), line(line), start(clock::now())
    {
        if (fallInRecursion =
                std::find(timersChain.begin(), timersChain.end(), line) != timersChain.end()) {
            return;
        }

        lineToName[line] = name;
        Node *node = &root;
        Node *prev = &root;
        for (auto &i : timersChain) {
            prev = node;
            node = const_cast<Node *>(&*node->childLines.insert(Node{i}).first);
        }

        if (isInner || node == &root) {
            node->childLines.insert(Node{line});
            timersChain.push_back(line);
        } else {
            prev->childLines.insert(Node{line});
        }
    }

    ~OmpTimer()
    {
        if (fallInRecursion) {
            return;
        }

        if (isInner) {
            timersChain.pop_back();
        }

        lineToTime[line] +=
            std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start).count();
    }

    static void PrintDurations()
    {
        Traverse(-1, &root);
    }

private:
    static void Traverse(const int64_t depth, const Node *ptr)
    {
        if (depth > 0) {
            for (int64_t i = 0; i < depth; i++) {
                printf(" |");
            }

            printf("-");
        }

        if (ptr->line > 0) {
            printf("%s=%lfs.\n", lineToName[ptr->line].c_str(), lineToTime[ptr->line] / 1e6);
        }

        for (auto &i : ptr->childLines) {
            Traverse(depth + 1, &i);
        }
    }

    using clock = std::chrono::high_resolution_clock;

    const std::string name;
    const bool isInner;
    const int64_t line;
    const clock::time_point start;

    bool fallInRecursion = false;
};

#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)
#define CREATE_TIMER(timerVariableName, timerName, isInner, line) \
    timerVariableName(timerName, isInner, line)
#define TIMER(name) OmpTimer CREATE_TIMER(CONCAT(__local_timer_, __LINE__), #name, false, __LINE__)
#define INNER_TIMER(name) \
    OmpTimer CREATE_TIMER(CONCAT(__local_timer_, __LINE__), #name, true, __LINE__)

#endif  // OMP_TIMER_H
