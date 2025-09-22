#include "PmergeMe.hpp"
#include <vector>
#include <deque>
#include <map>

// Single class-scoped counter definition
int PmergeMe::comparisons = 0;

// ====================== Jacobsthal ======================
std::size_t PmergeMe::jacobsthal(std::size_t n) {
    if (n == 0u) return 0u;
    if (n == 1u) return 1u;
    std::size_t a = 0u, b = 1u;
    for (std::size_t i = 2u; i <= n; ++i) {
        std::size_t c = b + 2u * a;
        a = b; b = c;
    }
    return b;
}

// Descending Jacobsthal blocks over indices 1..m-1
std::vector<std::size_t> PmergeMe::buildJacobInsertionOrder(std::size_t m) {
    std::vector<std::size_t> order;
    if (m <= 1u) return order;

    const std::size_t last = m - 1u;
    std::size_t prevStart = 1u;

    // Generate J(2), J(3), ...
    std::size_t a = 0u, b = 1u; // J0=0, J1=1
    for (;;) {
        std::size_t J = b + 2u * a; a = b; b = J;
        if (J > last) J = last;

        // push J, J-1, ..., prevStart (inclusive), descending
        for (std::size_t i = J + 1u; i-- > prevStart; )
            order.push_back(i);

        if (J == last) break;
        prevStart = J + 1u;
    }
    return order;
}

// ====================== lowerBoundIndex ======================
std::size_t PmergeMe::lowerBoundIndex(const std::vector<unsigned int>& c,
                                      std::size_t lo, std::size_t hi_excl,
                                      unsigned int val)
{
    if (hi_excl > c.size()) hi_excl = c.size(); // clamp
    while (lo < hi_excl) {
        std::size_t mid = lo + (hi_excl - lo) / 2u;
        ++PmergeMe::comparisons;                // one counted decision per step
        if (c[mid] < val) lo = mid + 1u;
        else               hi_excl = mid;
    }
    return lo; // first index with c[idx] >= val
}

std::size_t PmergeMe::lowerBoundIndex(const std::deque<unsigned int>& c,
                                      std::size_t lo, std::size_t hi_excl,
                                      unsigned int val)
{
    if (hi_excl > c.size()) hi_excl = c.size(); // clamp
    while (lo < hi_excl) {
        std::size_t mid = lo + (hi_excl - lo) / 2u;
        ++PmergeMe::comparisons;                // one counted decision per step
        if (c[mid] < val) lo = mid + 1u;
        else               hi_excl = mid;
    }
    return lo; // first index with c[idx] >= val
}

// ====================== Orchestration: vector ======================
void PmergeMe::splitIntoPairs(const std::vector<unsigned int>& src,
                              std::vector<Pair>& outPairs,
                              bool& hasStraggler, unsigned int& straggler)
{
    outPairs.clear();
    const std::size_t n = src.size();
    const std::size_t k = n / 2u;
    outPairs.reserve(k);

    hasStraggler = (n % 2u != 0u);
    if (hasStraggler) straggler = src[n - 1u];

    for (std::size_t i = 0u; i + 1u < n; i += 2u) {
        unsigned int a = src[i];
        unsigned int b = src[i + 1u];
        // Do NOT count this local pair comparison per spec.
        if (a > b) { unsigned int t = a; a = b; b = t; }
        outPairs.push_back(Pair(a, b)); // (min, max)
    }
}

void PmergeMe::insertFirstPending(std::vector<unsigned int>& chain,
                                  const std::vector<unsigned int>& pending,
                                  std::vector<std::size_t>& maxPos)
{
    if (pending.empty()) return;
    chain.insert(chain.begin(), pending[0]);
    for (std::size_t i = 0u; i < maxPos.size(); ++i) maxPos[i] += 1u;
}

void PmergeMe::insertPendingByJacob(std::vector<unsigned int>& chain,
                                    const std::vector<unsigned int>& pending,
                                    std::vector<std::size_t>& maxPos)
{
    const std::size_t m = pending.size();
    if (m <= 1u) return;

    const std::vector<std::size_t> order = buildJacobInsertionOrder(m);
    for (std::size_t t = 0u; t < order.size(); ++t) {
        const std::size_t idx = order[t];              // 1..m-1
        const unsigned int val = pending[idx];
        const std::size_t hi_excl = maxPos[idx] + 1u;  // partner-capped range

        std::size_t pos = lowerBoundIndex(chain, 0u, hi_excl, val);
        chain.insert(chain.begin() + pos, val);

        for (std::size_t j = 0u; j < maxPos.size(); ++j)
            if (maxPos[j] >= pos) maxPos[j] += 1u;
    }
}

void PmergeMe::insertStraggler(std::vector<unsigned int>& chain,
                               bool hasStraggler, unsigned int straggler)
{
    if (!hasStraggler) return;
    std::size_t pos = lowerBoundIndex(chain, 0u, chain.size(), straggler);
    chain.insert(chain.begin() + pos, straggler);
}

// ====================== Orchestration: deque ======================
void PmergeMe::splitIntoPairs(const std::deque<unsigned int>& src,
                              std::vector<Pair>& outPairs,
                              bool& hasStraggler, unsigned int& straggler)
{
    outPairs.clear();
    const std::size_t n = src.size();
    const std::size_t k = n / 2u;
    outPairs.reserve(k);

    hasStraggler = (n % 2u != 0u);
    if (hasStraggler) straggler = src[n - 1u];

    for (std::size_t i = 0u; i + 1u < n; i += 2u) {
        unsigned int a = src[i];
        unsigned int b = src[i + 1u];
        if (a > b) { unsigned int t = a; a = b; b = t; }
        outPairs.push_back(Pair(a, b));
    }
}

void PmergeMe::insertFirstPending(std::deque<unsigned int>& chain,
                                  const std::vector<unsigned int>& pending,
                                  std::vector<std::size_t>& maxPos)
{
    if (pending.empty()) return;
    chain.push_front(pending[0]);
    for (std::size_t i = 0u; i < maxPos.size(); ++i) maxPos[i] += 1u;
}

void PmergeMe::insertPendingByJacob(std::deque<unsigned int>& chain,
                                    const std::vector<unsigned int>& pending,
                                    std::vector<std::size_t>& maxPos)
{
    const std::size_t m = pending.size();
    if (m <= 1u) return;

    const std::vector<std::size_t> order = buildJacobInsertionOrder(m);
    for (std::size_t t = 0u; t < order.size(); ++t) {
        const std::size_t idx = order[t];             // 1..m-1
        const unsigned int val = pending[idx];
        const std::size_t hi_excl = maxPos[idx] + 1u;

        std::size_t pos = lowerBoundIndex(chain, 0u, hi_excl, val);
        chain.insert(chain.begin() + pos, val);

        for (std::size_t j = 0u; j < maxPos.size(); ++j)
            if (maxPos[j] >= pos) maxPos[j] += 1u;
    }
}

void PmergeMe::insertStraggler(std::deque<unsigned int>& chain,
                               bool hasStraggler, unsigned int straggler)
{
    if (!hasStraggler) return;
    std::size_t pos = lowerBoundIndex(chain, 0u, chain.size(), straggler);
    chain.insert(chain.begin() + pos, straggler);
}

// ====================== Duplicate-safe reordering helpers (file-local) ======================
static void orderPairsByWinners(const std::vector<PmergeMe::Pair>& pairs,
                                const std::vector<unsigned int>& winnersSorted,
                                std::vector<unsigned int>& chain,
                                std::vector<unsigned int>& pending,
                                std::vector<std::size_t>& partnerPos)
{
    chain.clear(); pending.clear(); partnerPos.clear();
    const std::size_t k = pairs.size(); if (k == 0u) return;

    // value -> stable list of original pair indices
    std::map<unsigned int, std::vector<std::size_t> > bucket;
    for (std::size_t i = 0u; i < k; ++i)
        bucket[pairs[i].second].push_back(i);

    chain.reserve(k + 1u);
    pending.reserve(k);
    partnerPos.reserve(k);

    for (std::size_t i = 0u; i < winnersSorted.size(); ++i) {
        unsigned int w = winnersSorted[i];
        std::vector<std::size_t>& list = bucket[w];
        std::size_t idx = list.front(); list.erase(list.begin());
        chain.push_back(pairs[idx].second);   // winner
        pending.push_back(pairs[idx].first);  // aligned loser
        partnerPos.push_back(i);              // current position of winner
    }
}

static void orderPairsByWinners(const std::vector<PmergeMe::Pair>& pairs,
                                const std::vector<unsigned int>& winnersSorted,
                                std::deque<unsigned int>& chain,
                                std::vector<unsigned int>& pending,
                                std::vector<std::size_t>& partnerPos)
{
    chain.clear(); pending.clear(); partnerPos.clear();
    const std::size_t k = pairs.size(); if (k == 0u) return;

    std::map<unsigned int, std::vector<std::size_t> > bucket;
    for (std::size_t i = 0u; i < k; ++i)
        bucket[pairs[i].second].push_back(i);

    for (std::size_t i = 0u; i < winnersSorted.size(); ++i) {
        unsigned int w = winnersSorted[i];
        std::vector<std::size_t>& list = bucket[w];
        std::size_t idx = list.front(); list.erase(list.begin());
        chain.push_back(pairs[idx].second);
        pending.push_back(pairs[idx].first);
        partnerPos.push_back(i);
    }
}

// ====================== Public wrappers (recursive winners) ======================
void PmergeMe::fordJohnsonSort(std::vector<unsigned int>& v)
{
    const std::size_t n = v.size();
    if (n <= 1u) return;

    // 1) Pair & straggler
    std::vector<Pair> pairs;
    bool hasStraggler = false;
    unsigned int straggler = 0u;
    splitIntoPairs(v, pairs, hasStraggler, straggler);
    if (pairs.empty()) return;

    // 2) Recursively sort winners (maxima) with Ford–Johnson
    std::vector<unsigned int> winners;
    winners.reserve(pairs.size());
    for (std::size_t i = 0u; i < pairs.size(); ++i)
        winners.push_back(pairs[i].second);
    fordJohnsonSort(winners); // recursion

    // 3) Rebuild chain/pending/partnerPos in winners' sorted order (duplicate-safe)
    std::vector<unsigned int> chain, pending;
    std::vector<std::size_t>  partnerPos;
    orderPairsByWinners(pairs, winners, chain, pending, partnerPos);

    // 4) Insert first pending at front (no comparisons)
    insertFirstPending(chain, pending, partnerPos);

    // 5) Insert the rest via Jacobsthal order
    insertPendingByJacob(chain, pending, partnerPos);

    // 6) Straggler (if any)
    insertStraggler(chain, hasStraggler, straggler);

    // 7) Copy back
    for (std::size_t i = 0u; i < n; ++i) v[i] = chain[i];
}

void PmergeMe::fordJohnsonSort(std::deque<unsigned int>& d)
{
    const std::size_t n = d.size();
    if (n <= 1u) return;

    // 1) Pair & straggler
    std::vector<Pair> pairs;
    bool hasStraggler = false;
    unsigned int straggler = 0u;
    splitIntoPairs(d, pairs, hasStraggler, straggler);
    if (pairs.empty()) return;

    // 2) Recursively sort winners (maxima)
    std::vector<unsigned int> winners;
    winners.reserve(pairs.size());
    for (std::size_t i = 0u; i < pairs.size(); ++i)
        winners.push_back(pairs[i].second);
    fordJohnsonSort(winners);

    // 3) Rebuild chain/pending/partnerPos in winners' sorted order
    std::deque<unsigned int>   chain;
    std::vector<unsigned int>  pending;
    std::vector<std::size_t>   partnerPos;
    orderPairsByWinners(pairs, winners, chain, pending, partnerPos);

    // 4) First loser at front (no comparisons)
    insertFirstPending(chain, pending, partnerPos);

    // 5) Remaining losers via Jacobsthal order
    insertPendingByJacob(chain, pending, partnerPos);

    // 6) Straggler
    insertStraggler(chain, hasStraggler, straggler);

    // 7) Copy back
    for (std::size_t i = 0u; i < n; ++i) d[i] = chain[i];
}
