#include "PmergeMe.hpp"
#include <vector>
#include <deque>
#include <map>
#include <utility>
#include <cstddef>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) { (void)other; }
PmergeMe& PmergeMe::operator=(const PmergeMe& other) { (void)other; return *this; }
PmergeMe::~PmergeMe() {}

int PmergeMe::comparisons = 0;

/**
 * Local Jacobsthal number calculator (iterative).
 * Param: n – index
 * Return: J(n)
 */
static std::size_t jacobsthal_local(std::size_t n) {
    if (n == 0u) return 0u;
    if (n == 1u) return 1u;
    std::size_t a = 0u, b = 1u;
    for (std::size_t i = 2u; i <= n; ++i) {
        std::size_t c = b + 2u * a;
        a = b; b = c;
    }
    return b;
}

/**
 * Builds the Jacobsthal insertion order for indices 1..m-1 in descending blocks.
 * Param: m – number of pending elements
 * Return: order vector of indices in which to insert pending[1..m-1]
 */
static std::vector<std::size_t> buildJacobInsertionOrder_local(std::size_t m) {
    std::vector<std::size_t> order;
    if (m <= 1u) return order;
    const std::size_t last = m - 1u;
    std::size_t prevStart = 1u;
    std::size_t a = 0u, b = 1u;
    for (;;) {
        std::size_t J = b + 2u * a; a = b; b = J;
        if (J > last) J = last;
        for (std::size_t i = J + 1u; i-- > prevStart; ) order.push_back(i);
        if (J == last) break;
        prevStart = J + 1u;
    }
    return order;
}

/**
 * Binary lower_bound over [lo, hi_excl); counts one decision per step.
 * Params: c – random access container, lo – start index, hi_excl – end (excl), val – value
 * Return: first index i in [lo, hi_excl) with c[i] >= val
 * Side effects: increments PmergeMe::comparisons
 */
template<typename RACont>
static std::size_t lowerBoundIndexT(const RACont& c,
                                    std::size_t lo,
                                    std::size_t hi_excl,
                                    unsigned int val)
{
    if (hi_excl > c.size()) hi_excl = c.size();
    while (lo < hi_excl) {
        std::size_t mid = lo + (hi_excl - lo) / 2u;
        ++PmergeMe::comparisons;
        if (c[mid] < val) lo = mid + 1u;
        else               hi_excl = mid;
    }
    return lo;
}

/**
 * Binary search capped to [0, cap_excl) with inclusive high; counts decisions.
 * Params: c – chain, cap_excl – exclusive upper bound, val – value
 * Return: insertion index in [0, cap_excl]
 * Side effects: increments PmergeMe::comparisons
 */
template<typename RACont>
static std::size_t cappedLowerBoundT(const RACont& c,
                                     std::size_t cap_excl,
                                     unsigned int val)
{
    if (cap_excl == 0u) return 0u;
    std::size_t low = 0u, high = cap_excl - 1u;
    while (low <= high) {
        std::size_t mid = (low + high) / 2u;
        ++PmergeMe::comparisons;
        if (c[mid] == val) return mid;
        if (c[mid] > val) {
            if (mid == 0u) break;
            high = mid - 1u;
        } else {
            low = mid + 1u;
        }
    }
    return low;
}

/**
 * Pairs adjacent elements using exactly one comparison per pair.
 * Produces (min,max) pairs and a list of pair maxima (winners).
 * Params: src – input container; out: pairs, winners, hasStraggler, straggler
 * Side effects: increments PmergeMe::comparisons
 */
template<typename RACont>
static void pairPhaseT(const RACont& src,
                       std::vector<PmergeMe::Pair>& pairs,
                       std::vector<unsigned int>& winners,
                       bool& hasStraggler, unsigned int& straggler)
{
    pairs.clear(); winners.clear();
    const std::size_t n = src.size();
    pairs.reserve(n/2u); winners.reserve(n/2u);
    hasStraggler = ((n & 1u) != 0u);
    if (hasStraggler) straggler = src[n - 1u];
    for (std::size_t i = 0u; i + 1u < n; i += 2u) {
        const unsigned int a = src[i];
        const unsigned int b = src[i + 1u];
        ++PmergeMe::comparisons;
        if (a > b) {
            pairs.push_back(PmergeMe::Pair(b, a));
            winners.push_back(a);
        } else {
            pairs.push_back(PmergeMe::Pair(a, b));
            winners.push_back(b);
        }
    }
}

/**
 * Reorders pairs to match the sorted winners and builds chain/pending/partnerPos.
 * Params: pairs, winnersSorted; out: chain, pending, partnerPos
 */
template<typename Chain>
static void orderPairsByWinnersT(const std::vector<PmergeMe::Pair>& pairs,
                                 const std::vector<unsigned int>& winnersSorted,
                                 Chain& chain,
                                 std::vector<unsigned int>& pending,
                                 std::vector<std::size_t>& partnerPos)
{
    chain.clear(); pending.clear(); partnerPos.clear();
    const std::size_t k = pairs.size(); if (k == 0u) return;
    std::map<unsigned int, std::vector<std::size_t> > bucket;
    for (std::size_t i = 0u; i < k; ++i) bucket[pairs[i].second].push_back(i);
    pending.reserve(k); partnerPos.reserve(k);
    for (std::size_t i = 0u; i < winnersSorted.size(); ++i) {
        unsigned int w = winnersSorted[i];
        std::vector<std::size_t>& list = bucket[w];
        const std::size_t idx = list.front(); list.erase(list.begin());
        chain.push_back(pairs[idx].second);
        pending.push_back(pairs[idx].first);
        partnerPos.push_back(i);
    }
}

/**
 * Inserts the first pending element at the front and shifts partner positions.
 * Params: chain, pending, partnerPos (modified)
 */
template<typename Chain>
static void insertFirstPendingT(Chain& chain,
                                const std::vector<unsigned int>& pending,
                                std::vector<std::size_t>& partnerPos)
{
    if (pending.empty()) return;
    chain.insert(chain.begin(), pending[0]);
    for (std::size_t i = 0u; i < partnerPos.size(); ++i) partnerPos[i] += 1u;
}

/**
 * Inserts remaining pending elements in Jacobsthal order strictly before partners.
 * Uses capped binary search; updates partner positions.
 * Params: chain, pending, partnerPos (modified)
 * Side effects: increments PmergeMe::comparisons
 */
template<typename Chain>
static void insertPendingByJacobT(Chain& chain,
                                  const std::vector<unsigned int>& pending,
                                  std::vector<std::size_t>& partnerPos)
{
    const std::size_t m = pending.size();
    if (m <= 1u) return;
    const std::vector<std::size_t> order = buildJacobInsertionOrder_local(m);
    for (std::size_t t = 0u; t < order.size(); ++t) {
        const std::size_t idx = order[t];
        const unsigned int val = pending[idx];
        std::size_t cap_excl = partnerPos[idx];
        if (cap_excl > chain.size()) cap_excl = chain.size();
        const std::size_t pos = cappedLowerBoundT(chain, cap_excl, val);
        chain.insert(chain.begin() + pos, val);
        for (std::size_t j = 0u; j < partnerPos.size(); ++j)
            if (partnerPos[j] >= pos) partnerPos[j] += 1u;
    }
}

/**
 * Inserts the straggler into the full chain using standard lower_bound.
 * Params: chain (modified), hasStraggler, straggler
 * Side effects: increments PmergeMe::comparisons
 */
template<typename Chain>
static void insertStragglerT(Chain& chain,
                             bool hasStraggler,
                             unsigned int straggler)
{
    if (!hasStraggler) return;
    const std::size_t pos = lowerBoundIndexT(chain, 0u, chain.size(), straggler);
    chain.insert(chain.begin() + pos, straggler);
}

/**
 * Core Ford–Johnson implementation generic over std::vector/std::deque.
 * Param: c – container to sort in place
 * Side effects: modifies c; increments PmergeMe::comparisons
 */
template<typename Chain>
static void fordJohnsonCoreT(Chain& c)
{
    const std::size_t n = c.size();
    if (n <= 1u) return;

    std::vector<PmergeMe::Pair> pairs;
    std::vector<unsigned int>   winners;
    bool hasStraggler = false;
    unsigned int straggler = 0u;
    pairPhaseT(c, pairs, winners, hasStraggler, straggler);
    if (pairs.empty()) return;

    PmergeMe::fordJohnsonSort(winners);

    Chain                     chain;
    std::vector<unsigned int> pending;
    std::vector<std::size_t>  partnerPos;
    orderPairsByWinnersT(pairs, winners, chain, pending, partnerPos);

    insertFirstPendingT(chain, pending, partnerPos);
    insertPendingByJacobT(chain, pending, partnerPos);
    insertStragglerT(chain, hasStraggler, straggler);

    for (std::size_t i = 0u; i < n; ++i) c[i] = chain[i];
}

/**
 * Public Jacobsthal wrapper.
 * Param: n – index
 * Return: J(n)
 */
std::size_t PmergeMe::jacobsthal(std::size_t n) {
    return jacobsthal_local(n);
}

/**
 * Public sorter for std::vector<unsigned int>.
 * Param: v – vector to sort in place
 * Side effects: increments PmergeMe::comparisons
 */
void PmergeMe::fordJohnsonSort(std::vector<unsigned int>& v) {
    fordJohnsonCoreT(v);
}

/**
 * Public sorter for std::deque<unsigned int>.
 * Param: d – deque to sort in place
 * Side effects: increments PmergeMe::comparisons
 */
void PmergeMe::fordJohnsonSort(std::deque<unsigned int>& d) {
    fordJohnsonCoreT(d);
}
