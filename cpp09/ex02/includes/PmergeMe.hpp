#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <utility>  // std::pair
#include <cstddef>  // size_t

class PmergeMe {
public:
    // Count ONLY: (a) binary-search steps and (b) pair-orientation decisions.
    static int comparisons;

    typedef std::pair<unsigned int, unsigned int> Pair; // (min, max)

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

    // Convenience: partner-capped lower_bound (search [0 .. partner_pos), exclusive)
    static std::size_t lowerBoundPartnerCap(const std::vector<unsigned int>& c,
                                            std::size_t partner_pos,
                                            unsigned int val);

    static std::size_t lowerBoundPartnerCap(const std::deque<unsigned int>& c,
                                            std::size_t partner_pos,
                                            unsigned int val);

    // ---------- One-compare-per-pair split ----------
    static void pairPhase(const std::vector<unsigned int>& src,
                          std::vector<Pair>& pairs,
                          std::vector<unsigned int>& winners,
                          bool& hasStraggler, unsigned int& straggler);

    static void pairPhase(const std::deque<unsigned int>& src,
                          std::vector<Pair>& pairs,
                          std::vector<unsigned int>& winners,
                          bool& hasStraggler, unsigned int& straggler);

    // ---------- Orchestration helpers (vector) ----------
    static void insertFirstPending(std::vector<unsigned int>& chain,
                                   const std::vector<unsigned int>& pending,
                                   std::vector<std::size_t>& partnerPos);

    static void insertPendingByJacob(std::vector<unsigned int>& chain,
                                     const std::vector<unsigned int>& pending,
                                     std::vector<std::size_t>& partnerPos);

    static void insertStraggler(std::vector<unsigned int>& chain,
                                bool hasStraggler, unsigned int straggler);

    // ---------- Orchestration helpers (deque) ----------
    static void insertFirstPending(std::deque<unsigned int>& chain,
                                   const std::vector<unsigned int>& pending,
                                   std::vector<std::size_t>& partnerPos);

    static void insertPendingByJacob(std::deque<unsigned int>& chain,
                                     const std::vector<unsigned int>& pending,
                                     std::vector<std::size_t>& partnerPos);

    static void insertStraggler(std::deque<unsigned int>& chain,
                                bool hasStraggler, unsigned int straggler);

    // ---------- Public wrappers ----------
    static void fordJohnsonSort(std::vector<unsigned int>& v);
    static void fordJohnsonSort(std::deque<unsigned int>& d);
};

#endif // PMERGEME_HPP
