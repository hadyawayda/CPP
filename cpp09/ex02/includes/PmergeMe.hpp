#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <utility>
#include <cstddef>

/**
 * Ford–Johnson (merge-insertion) sorter with comparison counting.
 * Public entry points handle std::vector and std::deque of unsigned int.
 */
class PmergeMe {
public:
    /** Default constructor (orthodox canonical form). */
    PmergeMe();
    /** Copy constructor (orthodox canonical form). */
    PmergeMe(const PmergeMe& other);
    /** Copy assignment (orthodox canonical form). */
    PmergeMe& operator=(const PmergeMe& other);
    /** Destructor (orthodox canonical form). */
    ~PmergeMe();

    /**
     * Returns J(n), the n-th Jacobsthal number.
     * Param: n – index
     * Return: J(n)
     */
    static std::size_t jacobsthal(std::size_t n);

    /**
     * Sorts the container in ascending order using Ford–Johnson.
     * Param: v – vector of unsigned int (modified in place)
     * Side effects: increments PmergeMe::comparisons
     */
    static void fordJohnsonSort(std::vector<unsigned int>& v);

    /**
     * Sorts the container in ascending order using Ford–Johnson.
     * Param: d – deque of unsigned int (modified in place)
     * Side effects: increments PmergeMe::comparisons
     */
    static void fordJohnsonSort(std::deque<unsigned int>& d);

    /** Global comparison counter incremented by the algorithm. */
    static int comparisons;

    /** Pair type for (min, max) produced in the pairing phase. */
    typedef std::pair<unsigned int, unsigned int> Pair;
};

#endif // PMERGEME_HPP
