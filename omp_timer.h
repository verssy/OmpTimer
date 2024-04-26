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
    using clock = std::chrono::high_resolution_clock;

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
    OmpTimer(const std::string &name, const bool isInner, const int64_t line)
        : name(name), isInner(isInner), line(line), start(clock::now())
    {
        fallInRecursion =
            std::find(timersChain.begin(), timersChain.end(), line) != timersChain.end();
        if (fallInRecursion) {
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

        isRunning = true;
    }

    ~OmpTimer()
    {
        Stop();
    }

    void Stop()
    {
        if (fallInRecursion || !isRunning) {
            return;
        }

        if (isInner) {
            timersChain.pop_back();
        }

        lineToTime[line] +=
            std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start).count();

        isRunning = false;
    }

    static void PrintDurations()
    {
        Traverse(&root);
    }

private:
    static void Traverse(const Node *ptr, const int64_t depth = -1)
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
            Traverse(&i, depth + 1);
        }
    }

    const std::string name;
    const bool isInner;
    const int64_t line;
    const clock::time_point start;

    bool fallInRecursion = false;
    bool isRunning = false;
};

#endif  // OMP_TIMER_H
