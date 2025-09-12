#include "PmergeMe.hpp"
#include <iostream>
#include <sys/time.h>
#include <vector>
#include <deque>

static long long nowMicros() {
    timeval tv; gettimeofday(&tv, 0);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
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

    printLine("Before:", refV);

    std::vector<unsigned int> v = refV;
    std::deque<unsigned int>  d = refD;

    PmergeMe::Ops ov, od;

    long long t0 = nowMicros();
    PmergeMe::sortVector(v, &ov);
    long long t1 = nowMicros();

    long long t2 = nowMicros();
    PmergeMe::sortDeque(d, &od);
    long long t3 = nowMicros();

    printLine("After: ", v);

    const size_t N = refV.size();
    std::cout << "Time to process a range of " << N
              << " elements with std::vector : " << (t1 - t0) << " us\n";
    std::cout << "Time to process a range of " << N
              << " elements with std::deque  : " << (t3 - t2) << " us\n";

    // Optional: ops dump
    std::cout << "Ops (vector) pairCmp=" << ov.pairLocalCompares
              << " pairSwaps=" << ov.pairLocalSwaps
              << " mergeCmp=" << ov.pairMergeCompares
              << " binCmp="   << ov.binarySearchCompares
              << " inserts="  << ov.inserts
              << " shifts="   << ov.shifts
              << std::endl;

    std::cout << "Ops (deque)  pairCmp=" << od.pairLocalCompares
              << " pairSwaps=" << od.pairLocalSwaps
              << " mergeCmp=" << od.pairMergeCompares
              << " binCmp="   << od.binarySearchCompares
              << " inserts="  << od.inserts
              << std::endl;

    return 0;
}
