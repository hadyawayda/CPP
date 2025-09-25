#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <stdexcept>

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    /** Validates argv: positive integers, unique, at least two. */
    static void validateArgs(char **av);

    /** Sorts in place using Ford–Johnson (vector). */
    void fordJohnsonVect(std::vector<unsigned int>& data);

    /** Sorts in place using Ford–Johnson (deque). */
    void fordJohnsonDeq(std::deque<unsigned int>& data);

    /** Comparison count from last vector run. */
    int vectorComparisons() const;

    /** Comparison count from last deque run. */
    int dequeComparisons() const;

private:
    int _cmpVec;
    int _cmpDeq;
};

#endif
