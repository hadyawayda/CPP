#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <stdexcept>

extern int g_comparisons;

class PmergeMe {
public:
    /* Constructs a fresh engine. */
    PmergeMe();
    /* Copies comparison state (no external resources). */
    PmergeMe(const PmergeMe& other);
    /* Assigns comparison state (no external resources). */
    PmergeMe& operator=(const PmergeMe& other);
    /* Destroys the engine. */
    ~PmergeMe();

    /* Parses argv, validates positives/uniqueness/size>=2, fills both containers or throws. */
    static void parseInputs(int argc, char** argv,
                            std::vector<unsigned int>& outVec,
                            std::deque<unsigned int>&  outDeq);

    /* Sorts vector<unsigned int> in place using Ford–Johnson with global comparison accounting. */
    void fordJohnsonVect(std::vector<unsigned int>& data);

    /* Sorts deque<unsigned int> in place using Ford–Johnson with global comparison accounting. */
    void fordJohnsonDeq(std::deque<unsigned int>& data);
};

#endif
