#include "PmergeMe.hpp"
#include <iostream>
#include <sys/time.h> // gettimeofday
#include <iomanip>

static long long nowMicros() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return static_cast<long long>(tv.tv_sec) * 1000000LL + tv.tv_usec;
}

template <typename Cont>
static void printLine(const char* prefix, const Cont& c) {
    std::cout << prefix;
    for (typename Cont::const_iterator it = c.begin(); it != c.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    std::vector<unsigned int> refV;
    std::deque<unsigned int>  refD;

    if (!PmergeMe::parseArgs(argc, argv, refV, refD)) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    // Before
    printLine("Before:", refV);

    // Vector
    std::vector<unsigned int> v = refV;
    PmergeMe::Metrics mv;
    long long t0 = nowMicros();
    PmergeMe::sortVector(v, &mv);
    long long t1 = nowMicros();

    // Deque
    std::deque<unsigned int> d = refD;
    PmergeMe::Metrics md;
    long long t2 = nowMicros();
    PmergeMe::sortDeque(d, &md);
    long long t3 = nowMicros();

    // After (use the vector result)
    printLine("After: ", v);

    const size_t N = refV.size();
    std::cout << "Time to process a range of " << N
              << " elements with std::vector : " << (t1 - t0) << " us" << std::endl;
    std::cout << "Time to process a range of " << N
              << " elements with std::deque  : " << (t3 - t2) << " us" << std::endl;

    std::cout << "Ops (vector): pairs_cmp=" << mv.pairComparisons
              << ", sort_cmp=" << mv.pairSortComparisons
              << ", binsearch_cmp=" << mv.binarySearchComparisons
              << ", inserts=" << mv.inserts
              << ", shifts=" << mv.elementMoves
              << std::endl;

    std::cout << "Ops (deque) : pairs_cmp=" << md.pairComparisons
              << ", sort_cmp=" << md.pairSortComparisons
              << ", binsearch_cmp=" << md.binarySearchComparisons
              << ", inserts=" << md.inserts
              << std::endl;

    return 0;
}
