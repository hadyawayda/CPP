#include "PmergeMe.hpp"
#include <vector>
#include <deque>
#include <map>

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

    // J0=0, J1=1
    std::size_t a = 0u, b = 1u;
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

// ====================== lowerBoundIndex (+ partner-cap) ======================
std::size_t PmergeMe::lowerBoundIndex(const std::vector<unsigned int>& c,
                                      std::size_t lo, std::size_t hi_excl,
                                      unsigned int val)
{
    if (hi_excl > c.size()) hi_excl = c.size();
    while (lo < hi_excl) {
        std::size_t mid = lo + (hi_excl - lo) / 2u;
        ++PmergeMe::comparisons;  // one counted decision per step
        if (c[mid] < val) lo = mid + 1u;
        else               hi_excl = mid;
    }
    return lo;
}

std::size_t PmergeMe::lowerBoundIndex(const std::deque<unsigned int>& c,
                                      std::size_t lo, std::size_t hi_excl,
                                      unsigned int val)
{
    if (hi_excl > c.size()) hi_excl = c.size();
    while (lo < hi_excl) {
        std::size_t mid = lo + (hi_excl - lo) / 2u;
        ++PmergeMe::comparisons;  // one counted decision per step
        if (c[mid] < val) lo = mid + 1u;
        else               hi_excl = mid;
    }
    return lo;
}

std::size_t PmergeMe::lowerBoundPartnerCap(const std::vector<unsigned int>& c,
                                           std::size_t partner_pos,
                                           unsigned int val)
{
    // strictly before partner: [0, partner_pos)
    std::size_t hi_excl = partner_pos;
    if (hi_excl > c.size()) hi_excl = c.size();
    return lowerBoundIndex(c, 0u, hi_excl, val);
}

std::size_t PmergeMe::lowerBoundPartnerCap(const std::deque<unsigned int>& c,
                                           std::size_t partner_pos,
                                           unsigned int val)
{
    std::size_t hi_excl = partner_pos;
    if (hi_excl > c.size()) hi_excl = c.size();
    return lowerBoundIndex(c, 0u, hi_excl, val);
}

// ====================== One-compare-per-pair phase ======================
void PmergeMe::pairPhase(const std::vector<unsigned int>& src,
                         std::vector<Pair>& pairs,
                         std::vector<unsigned int>& winners,
                         bool& hasStraggler, unsigned int& straggler)
{
    pairs.clear();
    winners.clear();

    const std::size_t n = src.size();
    pairs.reserve(n/2u);
    winners.reserve(n/2u);

    hasStraggler = (n % 2u != 0u);
    if (hasStraggler) straggler = src[n - 1u];

    for (std::size_t i = 0u; i + 1u < n; i += 2u) {
        const unsigned int a = src[i];
        const unsigned int b = src[i + 1u];

        // Exactly ONE comparison per pair:
        ++PmergeMe::comparisons;
        if (a > b) {
            pairs.push_back(Pair(b, a)); // (min, max)
            winners.push_back(a);        // max
        } else {
            pairs.push_back(Pair(a, b));
            winners.push_back(b);
        }
    }
}

void PmergeMe::pairPhase(const std::deque<unsigned int>& src,
                         std::vector<Pair>& pairs,
                         std::vector<unsigned int>& winners,
                         bool& hasStraggler, unsigned int& straggler)
{
    pairs.clear();
    winners.clear();

    const std::size_t n = src.size();
    pairs.reserve(n/2u);
    winners.reserve(n/2u);

    hasStraggler = (n % 2u != 0u);
    if (hasStraggler) straggler = src[n - 1u];

    for (std::size_t i = 0u; i + 1u < n; i += 2u) {
        const unsigned int a = src[i];
        const unsigned int b = src[i + 1u];

        ++PmergeMe::comparisons;         // one comparison per pair
        if (a > b) {
            pairs.push_back(Pair(b, a));
            winners.push_back(a);
        } else {
            pairs.push_back(Pair(a, b));
            winners.push_back(b);
        }
    }
}

// ====================== Local inclusive-high binary searches ======================
// NOTE: We use an inclusive 'high' index (cap - 1). Each loop counts exactly 1 step.
static std::size_t searchInclusiveHi(const std::vector<unsigned int>& vec,
                                     std::size_t hi_incl,
                                     unsigned int target)
{
    if (vec.empty()) return 0u;
    if (hi_incl >= vec.size()) hi_incl = vec.size() - 1u;

    std::size_t low = 0u, high = hi_incl;
    while (low <= high) {
        std::size_t mid = (low + high) / 2u;
        ++PmergeMe::comparisons;                   // exactly one counted step
        if (vec[mid] == target) return mid;        // early return on equality
        else if (vec[mid] > target) {
            if (mid == 0u) break;                  // prevent underflow
            high = mid - 1u;
        } else {
            low = mid + 1u;
        }
    }
    return low;                                    // insertion point
}

static std::size_t searchInclusiveHi(const std::deque<unsigned int>& vec,
                                     std::size_t hi_incl,
                                     unsigned int target)
{
    if (vec.empty()) return 0u;
    if (hi_incl >= vec.size()) hi_incl = vec.size() - 1u;

    std::size_t low = 0u, high = hi_incl;
    while (low <= high) {
        std::size_t mid = (low + high) / 2u;
        ++PmergeMe::comparisons;                   // exactly one counted step
        if (vec[mid] == target) return mid;
        else if (vec[mid] > target) {
            if (mid == 0u) break;
            high = mid - 1u;
        } else {
            low = mid + 1u;
        }
    }
    return low;
}

// ====================== Orchestration: vector ======================
void PmergeMe::insertFirstPending(std::vector<unsigned int>& chain,
                                  const std::vector<unsigned int>& pending,
                                  std::vector<std::size_t>& partnerPos)
{
    if (pending.empty()) return;
    chain.insert(chain.begin(), pending[0]);
    for (std::size_t i = 0u; i < partnerPos.size(); ++i) partnerPos[i] += 1u;
}

void PmergeMe::insertPendingByJacob(std::vector<unsigned int>& chain,
                                    const std::vector<unsigned int>& pending,
                                    std::vector<std::size_t>& partnerPos)
{
    const std::size_t m = pending.size();
    if (m <= 1u) return;

    const std::vector<std::size_t> order = buildJacobInsertionOrder(m);
    for (std::size_t t = 0u; t < order.size(); ++t) {
        const std::size_t idx = order[t];                // 1..m-1
        const unsigned int val = pending[idx];

        // strict cap: loser must be inserted strictly before its partner
        std::size_t cap_excl = partnerPos[idx];          // search [0 .. cap_excl)
        std::size_t pos = 0u;
        if (cap_excl == 0u) pos = 0u;
        else                 pos = searchInclusiveHi(chain, cap_excl - 1u, val);

        chain.insert(chain.begin() + pos, val);

        // bump partner positions at/after insertion point
        for (std::size_t j = 0u; j < partnerPos.size(); ++j)
            if (partnerPos[j] >= pos) partnerPos[j] += 1u;
    }
}

void PmergeMe::insertStraggler(std::vector<unsigned int>& chain,
                               bool hasStraggler, unsigned int straggler)
{
    if (!hasStraggler) return;
    const std::size_t pos = lowerBoundIndex(chain, 0u, chain.size(), straggler);
    chain.insert(chain.begin() + pos, straggler);
}

// ====================== Orchestration: deque ======================
void PmergeMe::insertFirstPending(std::deque<unsigned int>& chain,
                                  const std::vector<unsigned int>& pending,
                                  std::vector<std::size_t>& partnerPos)
{
    if (pending.empty()) return;
    chain.push_front(pending[0]);
    for (std::size_t i = 0u; i < partnerPos.size(); ++i) partnerPos[i] += 1u;
}

void PmergeMe::insertPendingByJacob(std::deque<unsigned int>& chain,
                                    const std::vector<unsigned int>& pending,
                                    std::vector<std::size_t>& partnerPos)
{
    const std::size_t m = pending.size();
    if (m <= 1u) return;

    const std::vector<std::size_t> order = buildJacobInsertionOrder(m);
    for (std::size_t t = 0u; t < order.size(); ++t) {
        const std::size_t idx = order[t];                // 1..m-1
        const unsigned int val = pending[idx];

        std::size_t cap_excl = partnerPos[idx];          // strictly before partner
        std::size_t pos = 0u;
        if (cap_excl == 0u) pos = 0u;
        else                 pos = searchInclusiveHi(chain, cap_excl - 1u, val);

        chain.insert(chain.begin() + pos, val);

        for (std::size_t j = 0u; j < partnerPos.size(); ++j)
            if (partnerPos[j] >= pos) partnerPos[j] += 1u;
    }
}

void PmergeMe::insertStraggler(std::deque<unsigned int>& chain,
                               bool hasStraggler, unsigned int straggler)
{
    if (!hasStraggler) return;
    const std::size_t pos = lowerBoundIndex(chain, 0u, chain.size(), straggler);
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

    // value -> stable list of original pair indices (supports duplicates)
    std::map<unsigned int, std::vector<std::size_t> > bucket;
    for (std::size_t i = 0u; i < k; ++i)
        bucket[pairs[i].second].push_back(i);

    chain.reserve(k + 1u);
    pending.reserve(k);
    partnerPos.reserve(k);

    for (std::size_t i = 0u; i < winnersSorted.size(); ++i) {
        const unsigned int w = winnersSorted[i];
        std::vector<std::size_t>& list = bucket[w];
        const std::size_t idx = list.front(); list.erase(list.begin());
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
        const unsigned int w = winnersSorted[i];
        std::vector<std::size_t>& list = bucket[w];
        const std::size_t idx = list.front(); list.erase(list.begin());
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

    // 1) One-compare-per-pair phase (no pair sorting)
    std::vector<Pair> pairs;
    std::vector<unsigned int> winners;
    bool hasStraggler = false;
    unsigned int straggler = 0u;
    pairPhase(v, pairs, winners, hasStraggler, straggler);
    if (pairs.empty()) return;

    // 2) Recursively sort winners (maxima)
    fordJohnsonSort(winners);

    // 3) Build chain/pending aligned to winners' order
    std::vector<unsigned int> chain, pending;
    std::vector<std::size_t>  partnerPos;
    orderPairsByWinners(pairs, winners, chain, pending, partnerPos);

    // 4) Insert first pending at front (no comparisons)
    insertFirstPending(chain, pending, partnerPos);

    // 5) Insert remaining minima in Jacobsthal order (strict before-partner cap)
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

    // 1) One-compare-per-pair phase (no pair sorting)
    std::vector<Pair> pairs;
    std::vector<unsigned int> winners;
    bool hasStraggler = false;
    unsigned int straggler = 0u;
    pairPhase(d, pairs, winners, hasStraggler, straggler);
    if (pairs.empty()) return;

    // 2) Recursively sort winners (maxima)
    fordJohnsonSort(winners);

    // 3) Build chain/pending aligned to winners' order
    std::deque<unsigned int>   chain;
    std::vector<unsigned int>  pending;
    std::vector<std::size_t>   partnerPos;
    orderPairsByWinners(pairs, winners, chain, pending, partnerPos);

    // 4) Insert first pending at front (no comparisons)
    insertFirstPending(chain, pending, partnerPos);

    // 5) Insert remaining minima via Jacobsthal order (strict before-partner cap)
    insertPendingByJacob(chain, pending, partnerPos);

    // 6) Straggler
    insertStraggler(chain, hasStraggler, straggler);

    // 7) Copy back
    for (std::size_t i = 0u; i < n; ++i) d[i] = chain[i];
}
