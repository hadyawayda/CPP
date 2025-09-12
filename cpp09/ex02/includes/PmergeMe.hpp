#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <cstddef> // size_t

class PmergeMe {
public:
    // Canonical Orthodox Form
    PmergeMe() {}
    PmergeMe(const PmergeMe&) {}
    PmergeMe& operator=(const PmergeMe&) { return *this; }
    ~PmergeMe() {}

    struct Ops {
        size_t pairLocalCompares;   // local compares in (min,max) pairing
        size_t pairMergeCompares;   // compares during stable sort of pairs by max
        size_t binarySearchCompares;// compares in bounded lower_bound
        size_t pairLocalSwaps;      // kept for display; do NOT increment
        size_t inserts;             // kept for display; do NOT increment
        size_t shifts;              // informational only (vector shifts)

        Ops() : pairLocalCompares(0), pairMergeCompares(0),
                binarySearchCompares(0), pairLocalSwaps(0),
                inserts(0), shifts(0) {}
    };

    static bool parseArgs(int argc, char** argv,
                          std::vector<unsigned int>& outVec,
                          std::deque<unsigned int>&  outDeq);

    static void sortVector(std::vector<unsigned int>& v, Ops* ops = 0);
    static void sortDeque (std::deque <unsigned int>& d, Ops* ops = 0);
};

#endif
