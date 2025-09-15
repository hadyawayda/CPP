#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <cstddef> // size_t

class PmergeMe {
public:
    struct Metrics {
        size_t pairComparisons;          // compares while forming (min,max) pairs
        size_t pairSortComparisons;      // comparator calls while sorting pairs
        size_t binarySearchComparisons;  // comparator calls in bounded lower_bound
        size_t inserts;                  // number of insert operations
        size_t elementMoves;             // only meaningful for std::vector

        Metrics()
        : pairComparisons(0), pairSortComparisons(0),
          binarySearchComparisons(0), inserts(0), elementMoves(0) {}
    };

    // Parse argv into positive integers (>0). Accepts numbers separated by spaces inside args.
    static bool parseArgs(int argc, char** argv,
                          std::vector<unsigned int>& outVec,
                          std::deque<unsigned int>&  outDeq);

    // Sort using a Ford–Johnson style merge-insert with Jacobsthal insertion order.
    static void sortVector(std::vector<unsigned int>& v, Metrics* m = 0);
    static void sortDeque (std::deque<unsigned int>&  d, Metrics* m = 0);
};

#endif
