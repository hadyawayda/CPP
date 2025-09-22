#include "PmergeMe.hpp"

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

std::size_t PmergeMe::lowerBoundPartnerCap(const std::vector<unsigned int>& c,
                                           std::size_t partner_pos,
                                           unsigned int val)
{
    std::size_t hi_excl = partner_pos + 1u;
    if (hi_excl > c.size()) hi_excl = c.size();
    return lowerBoundIndex(c, 0u, hi_excl, val);
}

std::size_t PmergeMe::lowerBoundPartnerCap(const std::deque<unsigned int>& c,
                                           std::size_t partner_pos,
                                           unsigned int val)
{
    std::size_t hi_excl = partner_pos + 1u;
    if (hi_excl > c.size()) hi_excl = c.size();
    return lowerBoundIndex(c, 0u, hi_excl, val);
}

// ====================== stable pair sort by (max, then min) ======================
// File-local helper: return true if L should come AFTER R (i.e., L > R by key).
// ONE counted comparison per ordering decision.
static bool pairGreaterByMaxThenMin(const PmergeMe::Pair& L,
                                    const PmergeMe::Pair& R)
{
    ++PmergeMe::comparisons; // count once per decision
    if (L.second != R.second) return (L.second > R.second);
    return (L.first > R.first);
}

// Stable insertion sort by (max asc, then min asc).
void PmergeMe::sortPairsByMax(std::vector<Pair>& pairs)
{
    if (pairs.size() <= 1u) return;

    for (std::size_t i = 1u; i < pairs.size(); ++i) {
        Pair key = pairs[i];
        std::size_t j = i;

        while (j > 0u) {
            // If pairs[j-1] > key by (max, then min), shift right
            if (pairGreaterByMaxThenMin(pairs[j - 1u], key)) {
                pairs[j] = pairs[j - 1u];
                --j;
            } else {
                break;
            }
        }
        if (j != i) pairs[j] = key;
    }
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

void PmergeMe::buildChainsFromPairs(const std::vector<Pair>& pairs,
                                    std::vector<unsigned int>& chain,
                                    std::vector<unsigned int>& pending,
                                    std::vector<std::size_t>& maxPos)
{
    chain.clear();
    pending.clear();
    maxPos.clear();

    const std::size_t k = pairs.size();
    if (k == 0u) return;

    chain.reserve(k + 1u);     // +1 for the first pending we will insert at front
    pending.reserve(k);
    maxPos.reserve(k);

    // Maxima (winners) into 'chain' in the order of sorted pairs
    for (std::size_t i = 0u; i < k; ++i) chain.push_back(pairs[i].second);
    // Minima (losers) aligned to the same order
    for (std::size_t i = 0u; i < k; ++i) pending.push_back(pairs[i].first);
    // Partner positions (each pair's max is at index i initially)
    for (std::size_t i = 0u; i < k; ++i) maxPos.push_back(i);
}

void PmergeMe::insertFirstPending(std::vector<unsigned int>& chain,
                                  const std::vector<unsigned int>& pending,
                                  std::vector<std::size_t>& maxPos)
{
    if (pending.empty()) return;
    chain.insert(chain.begin(), pending[0]);
    // shift all partner positions right by 1
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
        const std::size_t idx = order[t];           // 1..m-1
        const unsigned int val = pending[idx];
        const std::size_t hi_excl = maxPos[idx] + 1u;   // partner-capped range

        std::size_t pos = lowerBoundIndex(chain, 0u, hi_excl, val);
        chain.insert(chain.begin() + pos, val);

        // bump every partner position at/after insertion point
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
        // Do NOT count this local pair comparison per spec.
        if (a > b) { unsigned int t = a; a = b; b = t; }
        outPairs.push_back(Pair(a, b)); // (min, max)
    }
}

void PmergeMe::buildChainsFromPairs(const std::vector<Pair>& pairs,
                                    std::deque<unsigned int>& chain,
                                    std::vector<unsigned int>& pending,
                                    std::vector<std::size_t>& maxPos)
{
    chain.clear();
    pending.clear();
    maxPos.clear();

    const std::size_t k = pairs.size();
    if (k == 0u) return;

    // Maxima (winners) into 'chain'
    for (std::size_t i = 0u; i < k; ++i) chain.push_back(pairs[i].second);
    // Minima (losers) aligned to same order
    for (std::size_t i = 0u; i < k; ++i) pending.push_back(pairs[i].first);
    // Partner positions
    for (std::size_t i = 0u; i < k; ++i) maxPos.push_back(i);
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
        const std::size_t idx = order[t];            // 1..m-1
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

// ====================== Public wrappers ======================
void PmergeMe::fordJohnsonSort(std::vector<unsigned int>& v)
{
    const std::size_t n = v.size();
    if (n <= 1u) return;

    // 1) Pair and straggler
    std::vector<Pair> pairs;
    bool hasStraggler = false;
    unsigned int straggler = 0u;
    splitIntoPairs(v, pairs, hasStraggler, straggler);

    if (pairs.empty()) {
        // Only straggler exists → already "sorted"
        return;
    }

    // 2) Stable sort pairs by (max, then min) — counts comparisons
    sortPairsByMax(pairs);

    // 3) Build main chain (maxima) + pending (minima) and partner positions
    std::vector<unsigned int> chain;
    std::vector<unsigned int> pending;
    std::vector<std::size_t>  maxPos;
    buildChainsFromPairs(pairs, chain, pending, maxPos);

    // 4) Insert first pending at front (no comparison)
    insertFirstPending(chain, pending, maxPos);

    // 5) Insert remaining minima in Jacobsthal order (bounded binary searches)
    insertPendingByJacob(chain, pending, maxPos);

    // 6) Insert straggler (if any) via full-range lower_bound
    insertStraggler(chain, hasStraggler, straggler);

    // 7) Copy back
    for (std::size_t i = 0u; i < n; ++i) v[i] = chain[i];
}

void PmergeMe::fordJohnsonSort(std::deque<unsigned int>& d)
{
    const std::size_t n = d.size();
    if (n <= 1u) return;

    // 1) Pair and straggler
    std::vector<Pair> pairs;
    bool hasStraggler = false;
    unsigned int straggler = 0u;
    splitIntoPairs(d, pairs, hasStraggler, straggler);

    if (pairs.empty()) {
        return;
    }

    // 2) Stable sort pairs by (max, then min) — counts comparisons
    sortPairsByMax(pairs);

    // 3) Build main chain (maxima) + pending (minima) and partner positions
    std::deque<unsigned int>     chain;
    std::vector<unsigned int>    pending;
    std::vector<std::size_t>     maxPos;
    buildChainsFromPairs(pairs, chain, pending, maxPos);

    // 4) First pending at front (no comparison)
    insertFirstPending(chain, pending, maxPos);

    // 5) Insert others by Jacobsthal order with partner-capped binary searches
    insertPendingByJacob(chain, pending, maxPos);

    // 6) Straggler, if any
    insertStraggler(chain, hasStraggler, straggler);

    // 7) Copy back
    for (std::size_t i = 0u; i < n; ++i) d[i] = chain[i];
}
