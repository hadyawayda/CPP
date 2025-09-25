#include "PmergeMe.hpp"
#include <vector>
#include <deque>
#include <map>
#include <set>
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
 * Generates Jacobsthal numbers up to a limit (similar to oiskanda's approach).
 * Param: limit – maximum value
 * Return: vector of Jacobsthal numbers
 */
static std::vector<std::size_t> generateJacobsthalSequence(std::size_t limit) {
    std::vector<std::size_t> jacobSeq;
    std::size_t index = 3; // Start from J(3) = 5
    std::size_t a = 0u, b = 1u, c = 1u; // J(0)=0, J(1)=1, J(2)=1
    
    // Generate J(3), J(4), J(5), ...
    while (true) {
        c = b + 2u * a;
        if (c > limit) break;
        jacobSeq.push_back(c);
        a = b; b = c;
        index++;
    }
    return jacobSeq;
}

/**
 * Builds insertion order from Jacobsthal sequence (inspired by oiskanda's genInsertionOrder).
 * Param: m – number of pending elements
 * Return: order vector of indices in which to insert pending[1..m-1]
 */
static std::vector<std::size_t> buildJacobInsertionOrder_local(std::size_t m) {
    std::vector<std::size_t> order;
    if (m <= 1u) return order;
    
    std::vector<std::size_t> jacobSeq = generateJacobsthalSequence(m);
    std::vector<bool> used(m + 1, false);
    
    // Process each Jacobsthal number in descending order within its range
    for (std::size_t i = 0; i < jacobSeq.size(); ++i) {
        std::size_t jacobVal = jacobSeq[i];
        // Add indices from jacobVal down to the previous range
        std::size_t start = (i == 0) ? 2u : jacobSeq[i-1] + 1u;
        
        for (std::size_t idx = jacobVal; idx >= start && idx >= 1u; --idx) {
            if (idx <= m && !used[idx]) {
                order.push_back(idx - 1u); // Convert to 0-based indexing
                used[idx] = true;
            }
        }
    }
    
    // Add any remaining indices
    for (std::size_t idx = 1u; idx < m; ++idx) {
        if (!used[idx + 1u]) {
            order.push_back(idx);
        }
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
 * Calculates optimized search boundary using msarkis-inspired approach.
 * Implements progressive expansion for minimal comparisons.
 * Params: chainSize, currentIndex, totalPending
 * Return: search boundary limit
 */
static std::size_t calculateSearchBoundary(std::size_t chainSize, 
                                         std::size_t currentIndex, 
                                         std::size_t /* totalPending */)
{
    // msarkis approach: use progressive boundaries
    static std::size_t lastBoundary = 1;
    
    if (currentIndex == 0) {
        lastBoundary = 1;  // Reset for new sequence
    } else {
        lastBoundary = 2 * lastBoundary + 1;  // msarkis's expansion: high = 2*high+1
    }
    
    return std::min(lastBoundary, chainSize);
}

/**
 * Performs optimized binary search with computed boundaries.
 * Integrates msarkis-style optimizations for minimal comparisons.
 * Params: container, searchLimit, targetValue
 * Return: insertion position
 * Side effects: increments PmergeMe::comparisons
 */
template<typename RACont>
static std::size_t performBoundedSearch(const RACont& container,
                                       std::size_t searchLimit,
                                       unsigned int targetValue)
{
    if (container.empty() || searchLimit == 0) return 0;
    
    std::size_t left = 0;
    std::size_t right = std::min(searchLimit, container.size());
    
    while (left < right) {
        std::size_t middle = left + (right - left) / 2;
        ++PmergeMe::comparisons;
        
        if (container[middle] < targetValue) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    
    return left;
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
 * Constructs optimized element insertion sequence.
 * Combines Jacobsthal numbers with position-based prioritization.
 * Params: totalElements
 * Return: ordered sequence of indices for insertion
 */
static std::vector<std::size_t> constructInsertionSequence(std::size_t totalElements)
{
    if (totalElements <= 1) return std::vector<std::size_t>();
    
    // Generate Jacobsthal-based sequence
    std::vector<std::size_t> jacobsthalIndices = buildJacobInsertionOrder_local(totalElements);
    
    // Apply msarkis-inspired ordering optimizations
    std::vector<std::size_t> optimizedSequence;
    optimizedSequence.reserve(jacobsthalIndices.size());
    
    for (std::size_t i = 0; i < jacobsthalIndices.size(); ++i) {
        std::size_t index = jacobsthalIndices[i];
        if (index < totalElements && index > 0) { // Skip index 0 (handled separately)
            optimizedSequence.push_back(index);
        }
    }
    
    return optimizedSequence;
}

/**
 * Inserts the first pending element optimally without comparison.
 * Implements strategic first-element placement for minimal operations.
 * Params: chain, pending, partnerPos (modified)
 */
template<typename Chain>
static void insertFirstPendingT(Chain& chain,
                                const std::vector<unsigned int>& pending,
                                std::vector<std::size_t>& partnerPos)
{
    if (pending.empty()) return;
    
    // Strategic first insertion - always optimal at beginning
    chain.insert(chain.begin(), pending[0]);
    for (std::size_t i = 0u; i < partnerPos.size(); ++i) partnerPos[i] += 1u;
}

/**
 * Executes strategic element placement using msarkis-optimized techniques.
 * Applies adaptive boundaries and position-aware search strategies.
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
    
    // Get strategically optimized insertion sequence
    const std::vector<std::size_t> insertionOrder = constructInsertionSequence(m);
    
    // Process each element with adaptive optimization
    for (std::size_t orderIndex = 0u; orderIndex < insertionOrder.size(); ++orderIndex) {
        const std::size_t elementIndex = insertionOrder[orderIndex];
        if (elementIndex >= pending.size()) continue;
        
        const unsigned int elementValue = pending[elementIndex];
        
        // Calculate position-aware search boundary
        std::size_t searchBoundary = calculateSearchBoundary(
            chain.size(), elementIndex, pending.size());
        
        // Apply partner position constraint for optimal bounds
        if (elementIndex < partnerPos.size()) {
            std::size_t partnerConstraint = partnerPos[elementIndex];
            if (partnerConstraint <= chain.size()) {
                searchBoundary = std::min(searchBoundary, partnerConstraint);
            }
        }
        
        // Execute optimized bounded search
        std::size_t insertionPosition = performBoundedSearch(
            chain, searchBoundary, elementValue);
        
        // Strategic element insertion
        chain.insert(chain.begin() + insertionPosition, elementValue);
        
        // Efficiently update all affected positions
        for (std::size_t j = 0u; j < partnerPos.size(); ++j) {
            if (partnerPos[j] >= insertionPosition) {
                partnerPos[j] += 1u;
            }
        }
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

    // Optimization: handle very small arrays with optimal sorts
    if (n == 2u) {
        ++PmergeMe::comparisons;
        if (c[0] > c[1]) {
            unsigned int temp = c[0];
            c[0] = c[1];
            c[1] = temp;
        }
        return;
    }
    
    if (n == 3u) {
        // Optimized 3-element sort using only 3 comparisons maximum
        ++PmergeMe::comparisons;
        if (c[0] > c[1]) {
            unsigned int temp = c[0];
            c[0] = c[1];
            c[1] = temp;
        }
        ++PmergeMe::comparisons;
        if (c[1] > c[2]) {
            unsigned int temp = c[1];
            c[1] = c[2];
            c[2] = temp;
            ++PmergeMe::comparisons;
            if (c[0] > c[1]) {
                temp = c[0];
                c[0] = c[1];
                c[1] = temp;
            }
        }
        return;
    }
    
    // Optimization: handle 4-element case specially
    if (n == 4u) {
        // Sort pairs first
        ++PmergeMe::comparisons;
        if (c[0] > c[1]) {
            unsigned int temp = c[0]; c[0] = c[1]; c[1] = temp;
        }
        ++PmergeMe::comparisons;
        if (c[2] > c[3]) {
            unsigned int temp = c[2]; c[2] = c[3]; c[3] = temp;
        }
        // Now we have sorted pairs (c[0] <= c[1]) and (c[2] <= c[3])
        // Sort the larger elements
        ++PmergeMe::comparisons;
        if (c[1] > c[3]) {
            unsigned int temp = c[1]; c[1] = c[3]; c[3] = temp;
            temp = c[0]; c[0] = c[2]; c[2] = temp;
        }
        // Insert c[2] into position
        ++PmergeMe::comparisons;
        if (c[2] < c[0]) {
            unsigned int temp = c[2];
            c[2] = c[1]; c[1] = c[0]; c[0] = temp;
        } else {
            ++PmergeMe::comparisons;
            if (c[2] < c[1]) {
                unsigned int temp = c[2];
                c[2] = c[1]; c[1] = temp;
            }
        }
        return;
    }

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
