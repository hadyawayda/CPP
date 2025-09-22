#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <utility>  // std::pair
#include <cstddef>  // size_t

class PmergeMe {
public:
    // Count ONLY: (a) binary-search steps and (b) pair-sorting comparator decisions.
    static int comparisons;

    // ---------- Jacobsthal helpers ----------
    static std::size_t jacobsthal(std::size_t n);
    // Returns indices 1..m-1 in Ford–Johnson order (descending Jacobsthal blocks)
    static std::vector<std::size_t> buildJacobInsertionOrder(std::size_t m);

    // ---------- Binary search (first idx in [lo, hi_excl) where c[idx] >= val) ----------
    static std::size_t lowerBoundIndex(const std::vector<unsigned int>& c,
                                       std::size_t lo, std::size_t hi_excl,
                                       unsigned int val);

    static std::size_t lowerBoundIndex(const std::deque<unsigned int>& c,
                                       std::size_t lo, std::size_t hi_excl,
                                       unsigned int val);

    // Convenience: partner-capped lower_bound (search [0 .. partner_pos], inclusive)
    static std::size_t lowerBoundPartnerCap(const std::vector<unsigned int>& c,
                                            std::size_t partner_pos,
                                            unsigned int val);

    static std::size_t lowerBoundPartnerCap(const std::deque<unsigned int>& c,
                                            std::size_t partner_pos,
                                            unsigned int val);

    // ---------- Pair type and explicit, stable pair sort by (max, then min) ----------
    typedef std::pair<unsigned int, unsigned int> Pair; // (min, max)

    // Stable insertion sort of pairs by (max ascending, then min ascending).
    // Counts ONE comparison per ordering decision.
    static void sortPairsByMax(std::vector<Pair>& pairs);

    // ---------- Orchestration helpers (vector) ----------
    static void splitIntoPairs(const std::vector<unsigned int>& src,
                               std::vector<Pair>& outPairs,
                               bool& hasStraggler, unsigned int& straggler);

    static void buildChainsFromPairs(const std::vector<Pair>& pairs,
                                     std::vector<unsigned int>& chain,
                                     std::vector<unsigned int>& pending,
                                     std::vector<std::size_t>& maxPos);

    static void insertFirstPending(std::vector<unsigned int>& chain,
                                   const std::vector<unsigned int>& pending,
                                   std::vector<std::size_t>& maxPos);

    static void insertPendingByJacob(std::vector<unsigned int>& chain,
                                     const std::vector<unsigned int>& pending,
                                     std::vector<std::size_t>& maxPos);

    static void insertStraggler(std::vector<unsigned int>& chain,
                                bool hasStraggler, unsigned int straggler);

    // ---------- Orchestration helpers (deque) ----------
    static void splitIntoPairs(const std::deque<unsigned int>& src,
                               std::vector<Pair>& outPairs,
                               bool& hasStraggler, unsigned int& straggler);

    static void buildChainsFromPairs(const std::vector<Pair>& pairs,
                                     std::deque<unsigned int>& chain,
                                     std::vector<unsigned int>& pending,
                                     std::vector<std::size_t>& maxPos);

    static void insertFirstPending(std::deque<unsigned int>& chain,
                                   const std::vector<unsigned int>& pending,
                                   std::vector<std::size_t>& maxPos);

    static void insertPendingByJacob(std::deque<unsigned int>& chain,
                                     const std::vector<unsigned int>& pending,
                                     std::vector<std::size_t>& maxPos);

    static void insertStraggler(std::deque<unsigned int>& chain,
                                bool hasStraggler, unsigned int straggler);

    // ---------- Public wrappers ----------
    static void fordJohnsonSort(std::vector<unsigned int>& v);
    static void fordJohnsonSort(std::deque<unsigned int>& d);
};

#endif // PMERGEME_HPP
