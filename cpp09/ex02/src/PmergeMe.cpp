#include "PmergeMe.hpp"
#include <sstream>
#include <limits>
#include <cctype>
#include <vector>
#include <deque>

// ====================== parsing ======================
static bool parseOneToken(const std::string& tok, unsigned int& value) {
    if (tok.empty()) return false;
    for (std::string::size_type i = 0; i < tok.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(tok[i])))
            return false;

    std::istringstream iss(tok);
    unsigned long ul = 0;
    iss >> ul;
    if (!iss || !iss.eof()) return false;
    if (ul == 0 || ul > std::numeric_limits<unsigned int>::max()) return false;

    value = static_cast<unsigned int>(ul);
    return true;
}

bool PmergeMe::parseArgs(int argc, char** argv,
                         std::vector<unsigned int>& outVec,
                         std::deque<unsigned int>&  outDeq)
{
    for (int i = 1; i < argc; ++i) {
        std::istringstream ss(argv[i]);
        std::string tok;
        while (ss >> tok) {
            unsigned int v;
            if (!parseOneToken(tok, v)) return false;
            outVec.push_back(v);
            outDeq.push_back(v);
        }
    }
    return !outVec.empty();
}

// ====================== tiny helpers ======================
static inline void cswap(unsigned int& x, unsigned int& y) { if (x > y) { unsigned int t = x; x = y; y = t; } }

template <typename Cont>
static void tinySort(Cont& a) {
    const size_t n = a.size();
    if (n <= 1) return;
    if (n == 2) { cswap(a[0], a[1]); return; }
    // n == 3: small sorting network
    if (a[0] > a[1]) { unsigned int t = a[0]; a[0] = a[1]; a[1] = t; }
    if (a[1] > a[2]) { unsigned int t = a[1]; a[1] = a[2]; a[2] = t; }
    if (a[0] > a[1]) { unsigned int t = a[0]; a[0] = a[1]; a[1] = t; }
}

// Custom lower_bound with small-window edge checks.
// Searches in [lo, hi) and counts ONLY comparisons.
template <typename Cont>
static size_t lowerBoundIndex(const Cont& c, size_t lo, size_t hi,
                              unsigned int val, PmergeMe::Ops* ops)
{
    size_t width = (hi > lo ? hi - lo : 0);
    if (width == 0) return lo;
    if (width == 1) {
        if (ops) ops->binarySearchCompares++;
        return (c[lo] < val) ? lo + 1 : lo;
    }
    if (width == 2) {
        if (ops) ops->binarySearchCompares++;
        if (!(c[lo] < val)) return lo;
        if (ops) ops->binarySearchCompares++;
        return (c[lo + 1] < val) ? lo + 2 : lo + 1;
    }

    // Tiny window (<=8): do a couple of edge checks that often short-circuit.
    if (width <= 8) {
        if (ops) ops->binarySearchCompares++;
        if (!(c[lo] < val)) return lo;       // at far left

        if (ops) ops->binarySearchCompares++;
        if (c[hi - 2] < val) return hi - 1;  // right before partner cap

        size_t L = lo + 1, R = hi - 1;
        while (L < R) {
            size_t mid = L + (R - L) / 2;
            if (ops) ops->binarySearchCompares++;
            if (c[mid] < val) L = mid + 1;
            else              R = mid;
        }
        return L;
    }

    // Standard binary search for larger windows
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (ops) ops->binarySearchCompares++;
        if (c[mid] < val) lo = mid + 1;
        else              hi = mid;
    }
    return lo;
}

// Write src into dst (sizes must match)
template <typename Cont1, typename Cont2>
static void copyBack(Cont1& dst, const Cont2& src) {
    for (size_t i = 0; i < src.size(); ++i) dst[i] = src[i];
}

// Track element shifts for vector only
static void accountMoves(std::vector<unsigned int>&, size_t pos, size_t oldSize, PmergeMe::Ops* ops) {
    if (ops) ops->shifts += (oldSize > pos ? (oldSize - pos) : 0);
}
static void accountMoves(std::deque<unsigned int>&, size_t /*pos*/, size_t /*oldSize*/, PmergeMe::Ops* /*ops*/) {
    // Deque: leave empty (implementation-dependent; we won't estimate)
}

// ====================== pair type & stable merge-sort ======================
struct PairMM {
    unsigned int minv;
    unsigned int maxv;
};

// stable merge by 'maxv' (take left on tie). Counts one compare per decision.
static void mergePairs(std::vector<PairMM>& a,
                       std::vector<PairMM>& buf,
                       size_t lo, size_t mid, size_t hi,
                       PmergeMe::Ops* ops)
{
    size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (ops) ops->pairMergeCompares++;
        if (a[i].maxv <= a[j].maxv) buf[k++] = a[i++];
        else                        buf[k++] = a[j++];
    }
    while (i < mid) buf[k++] = a[i++];
    while (j < hi)  buf[k++] = a[j++];
    for (size_t t = lo; t < hi; ++t) a[t] = buf[t];
}

static void mergeSortPairsRec(std::vector<PairMM>& a,
                              std::vector<PairMM>& buf,
                              size_t lo, size_t hi,
                              PmergeMe::Ops* ops)
{
    if (hi - lo <= 1) return;
    size_t mid = lo + (hi - lo) / 2;
    mergeSortPairsRec(a, buf, lo, mid, ops);
    mergeSortPairsRec(a, buf, mid, hi, ops);
    mergePairs(a, buf, lo, mid, hi, ops);
}

static void stableSortPairsByMax(std::vector<PairMM>& pairs, PmergeMe::Ops* ops) {
    if (pairs.size() <= 1) return;
    std::vector<PairMM> buf(pairs.size());
    mergeSortPairsRec(pairs, buf, 0, pairs.size(), ops);
}

// ====================== orchestration helpers ======================

// Step 1: build pair list (min,max) + straggler
template <typename Cont>
static void splitIntoPairs(const Cont& arr,
                           std::vector<PairMM>& pairs,
                           bool& hasStraggler, unsigned int& straggler,
                           PmergeMe::Ops* ops)
{
    const size_t n = arr.size();
    const size_t pairCount = n / 2;
    hasStraggler = (n % 2 != 0);
    if (hasStraggler) straggler = arr[n - 1];

    pairs.clear();
    pairs.reserve(pairCount);

    for (size_t i = 0; i < pairCount; ++i) {
        unsigned int a0 = arr[2 * i];
        unsigned int b0 = arr[2 * i + 1];
        if (ops) ops->pairLocalCompares++;
        if (a0 > b0) {
            // if (ops) ops->pairLocalSwaps++;
            unsigned int t = a0; a0 = b0; b0 = t;
        }
        PairMM p; p.minv = a0; p.maxv = b0;
        pairs.push_back(p);
    }
}

// Step 2: main chain (maxima) and pending (mins) from sorted pairs
template <typename Cont>
static void buildChainsFromPairs(const std::vector<PairMM>& pairs,
                                 Cont& chain,
                                 std::vector<unsigned int>& pending,
                                 std::vector<size_t>& maxPos)
{
    const size_t pairCount = pairs.size();
    chain.clear();
    pending.assign(pairCount, 0);
    maxPos.assign(pairCount, 0);

    if (pairCount == 0) return;
    chain.insert(chain.end(), pairs[0].maxv);
    for (size_t i = 1; i < pairCount; ++i) chain.insert(chain.end(), pairs[i].maxv);
    for (size_t i = 0; i < pairCount; ++i) pending[i] = pairs[i].minv;
    for (size_t i = 0; i < pairCount; ++i) maxPos[i] = i;
}

// Step 3: insert first pending at front
template <typename Cont>
static void insertFirstPendingIfAny(Cont& chain,
                                    const std::vector<unsigned int>& pending,
                                    std::vector<size_t>& maxPos,
                                    std::vector<size_t>& minPos,   // <-- add
                                    PmergeMe::Ops* ops)
{
    if (pending.empty()) return;
    const size_t oldSize = chain.size();
    chain.insert(chain.begin(), pending[0]);
    // if (ops) ops->inserts++;
    accountMoves(chain, 0, oldSize, ops);
    for (size_t i = 0; i < maxPos.size(); ++i) ++maxPos[i];
    if (minPos.size() < pending.size()) minPos.assign(pending.size(), 0);
    minPos[0] = 0; // record position of the first pending
}

// Step 4: bounded insert helper (binary search in [0..hiIncl])
template <typename Cont>
static void boundedInsert(Cont& chain,
                          unsigned int val,
                          size_t hiIncl,
                          std::vector<size_t>& maxPos,
                          PmergeMe::Ops* ops)
{
    const size_t hiExcl = hiIncl + 1;
    const size_t pos = lowerBoundIndex(chain, 0, hiExcl, val, ops);
    const size_t oldSize = chain.size();
    chain.insert(chain.begin() + pos, val);
    // if (ops) ops->inserts++;
    accountMoves(chain, pos, oldSize, ops);

    for (size_t j = 0; j < maxPos.size(); ++j)
        if (maxPos[j] >= pos) ++maxPos[j];
}

// Step 5: Jacobsthal-driven inserts for the rest of 'pending'
// For each index (descending within each Jacobsthal block), search in [0 .. maxPos[idx]+1)
template <typename Cont>
static void insertPendingJacob(Cont& chain,
                               const std::vector<unsigned int>& pending,
                               std::vector<size_t>& maxPos,
                               std::vector<size_t>& /*minPos*/,
                               PmergeMe::Ops* ops)
{
    const size_t m = pending.size();
    if (m <= 1) return;

    const size_t last = m - 1;
    size_t a = 0, b = 1, prevStart = 1;

    for (;;) {
        size_t J = b + 2 * a; a = b; b = J;
        if (J > last) J = last;

        for (size_t idx = J + 1; idx-- > prevStart; ) {
            size_t hiExcl = maxPos[idx] + 1;              // partner cap
            size_t pos    = lowerBoundIndex(chain, 0, hiExcl, pending[idx], ops);

            const size_t oldSize = chain.size();
            chain.insert(chain.begin() + pos, pending[idx]);
            accountMoves(chain, pos, oldSize, ops);        // informational for vector

            for (size_t j = 0; j < maxPos.size(); ++j)
                if (maxPos[j] >= pos) ++maxPos[j];
        }

        if (J == last) break;
        prevStart = J + 1;
    }
}

// Step 6: straggler insertion (full-range binary search)
template <typename Cont>
static void insertStragglerIfAny(Cont& chain,
                                 bool hasStraggler, unsigned int straggler,
                                 const std::vector<size_t>& /*minPos*/,
                                 PmergeMe::Ops* ops)
{
    if (!hasStraggler) return;
    size_t pos = lowerBoundIndex(chain, 0, chain.size(), straggler, ops);
    const size_t oldSize = chain.size();
    chain.insert(chain.begin() + pos, straggler);
    // if (ops) ops->inserts++; // keep disabled if OP=comparisons-only
    accountMoves(chain, pos, oldSize, ops);
}

// Full Ford-Johnson orchestration
template <typename Cont>
static void fordJohnsonSortImpl(Cont& arr, PmergeMe::Ops* ops) {
    const size_t n = arr.size();
    if (n <= 3) { tinySort(arr); return; }

    std::vector<PairMM> pairs;
    bool hasStraggler = false; unsigned int straggler = 0;
    splitIntoPairs(arr, pairs, hasStraggler, straggler, ops);

    stableSortPairsByMax(pairs, ops);

    Cont chain;
    std::vector<unsigned int> pending;
    std::vector<size_t> maxPos;
    std::vector<size_t> minPos; // NEW

    buildChainsFromPairs(pairs, chain, pending, maxPos);

    insertFirstPendingIfAny(chain, pending, maxPos, minPos, ops);
    insertPendingJacob(chain, pending, maxPos, minPos, ops);
    insertStragglerIfAny(chain, hasStraggler, straggler, minPos, ops);

    copyBack(arr, chain);
}

// public wrappers
void PmergeMe::sortVector(std::vector<unsigned int>& v, PmergeMe::Ops* ops) { fordJohnsonSortImpl(v, ops); }
void PmergeMe::sortDeque (std::deque <unsigned int>& d, PmergeMe::Ops* ops) { fordJohnsonSortImpl(d, ops); }
