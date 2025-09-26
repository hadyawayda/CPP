#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <stdexcept>

extern int g_comparisons;

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    static void parseInputs(int argc, char** argv,
                            std::vector<unsigned int>& outVec,
                            std::deque<unsigned int>&  outDeq);

    void fordJohnsonVect(std::vector<unsigned int>& data);
    void fordJohnsonDeq(std::deque<unsigned int>& data);
};

#endif
