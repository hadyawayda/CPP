#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <sys/time.h>

/* Returns current time in microseconds. */
static long long nowMicros() {
    struct timeval tv; gettimeofday(&tv, 0);
    return static_cast<long long>(tv.tv_sec) * 1000000LL + tv.tv_usec;
}

/* Prints a single line with a prefix and all elements of the container. */
template <typename Cont>
static void printLine(const char* prefix, const Cont& c) {
    std::cout << prefix;
    for (typename Cont::const_iterator it = c.begin(); it != c.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;
}

/* Program entry: parses inputs, runs vector and deque sorts, reports timing and comparisons. */
int main(int argc, char** argv) {
    std::vector<unsigned int> inputV;
    std::deque<unsigned int>  inputD;

    try {
        PmergeMe::parseInputs(argc, argv, inputV, inputD);
    } catch (...) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    printLine("Before:", inputV);

    std::vector<unsigned int> v = inputV;
    std::deque<unsigned int>  d = inputD;

    PmergeMe engine;

    const int startTotal = g_comparisons;

    const int startVec = g_comparisons;
    long long t0 = nowMicros();
    engine.fordJohnsonVect(v);
    long long t1 = nowMicros();
    const int vecCmp = g_comparisons - startVec;

    const int startDeq = g_comparisons;
    long long t2 = nowMicros();
    engine.fordJohnsonDeq(d);
    long long t3 = nowMicros();
    const int deqCmp = g_comparisons - startDeq;

    const int totalCmp = g_comparisons - startTotal;

    printLine("After: ", v);

    const std::size_t N = inputV.size();
    std::cout << "Time to process a range of " << N
              << " elements with std::vector : " << (t1 - t0) << " us\n";
    std::cout << "Comparisons (vector): " << vecCmp << "\n";
    std::cout << "Time to process a range of " << N
              << " elements with std::deque  : " << (t3 - t2) << " us\n";
    std::cout << "Comparisons (deque): " << deqCmp << "\n";
    std::cout << "Comparisons (total since start): " << totalCmp << std::endl;

    for (std::size_t i = 1; i < v.size(); ++i)
        if (v[i-1] > v[i]) { std::cerr << "ERROR: vector not sorted\n"; return 1; }
    for (std::size_t i = 1; i < d.size(); ++i)
        if (d[i-1] > d[i]) { std::cerr << "ERROR: deque not sorted\n"; return 1; }

    return 0;
}
