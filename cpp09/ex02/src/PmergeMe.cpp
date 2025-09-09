#include "PmergeMe.hpp"
#include <sstream>
#include <limits>
#include <cctype>
#include <algorithm>
#include <utility>

// ---------- parsing ----------
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

// ----- Jacobsthal numbers and insertion order (C++98) -----
static size_t jacobsthal(size_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    size_t a = 0, b = 1; // J(0)=0, J(1)=1
    for (size_t i = 2; i <= n; ++i) {
        size_t c = b + 2 * a; // J(n)=J(n-1)+2*J(n-2)
        a = b; b = c;
    }
    return b;
}

static std::vector<size_t> buildInsertionOrder(size_t pairCount) {
    std::vector<size_t> order;
    if (pairCount <= 1) return order;

    const size_t last = pairCount - 1;
    size_t prevStart = 1;

    for (size_t j = 2; ; ++j) {
        size_t J = jacobsthal(j);
        if (J > last) J = last;

        // push J, J-1, ..., prevStart (descending)
        for (size_t i = J; ; --i) {
            if (i < prevStart) break;
            order.push_back(i);
            if (i == prevStart) break;
        }

        if (J == last) break;
        prevStart = J + 1;
    }
    return order;
}

// ---------- metrics helpers (no lambdas; functor comparators) ----------
template <typename U>
struct LessCounted {
    PmergeMe::Metrics* m;
    LessCounted(PmergeMe::Metrics* mm) : m(mm) {}
    bool operator()(const U& a, const U& b) const {
        if (m) m->binarySearchComparisons++;
        return a < b;
    }
};

template <typename U>
struct PairByMaxThenMinCounted {
    PmergeMe::Metrics* m;
    PairByMaxThenMinCounted(PmergeMe::Metrics* mm) : m(mm) {}
    bool operator()(const std::pair<U,U>& x, const std::pair<U,U>& y) const {
        if (m) m->pairSortComparisons++;
        if (x.second != y.second) return x.second < y.second;
        return x.first < y.first;
    }
};

// accountMoves: overloads to add element move counts only for vector
static void accountMoves(std::vector<unsigned int>&, size_t moved, PmergeMe::Metrics* m) {
    if (m) m->elementMoves += moved;
}
static void accountMoves(std::deque<unsigned int>&, size_t /*moved*/, PmergeMe::Metrics* /*m*/) {
    // do nothing for deque
}

// ---------- core algorithm ----------
template <typename Cont>
static size_t binaryInsertLimited(Cont& chain, unsigned int value,
                                  size_t hiIndex, PmergeMe::Metrics* m)
{
    typename Cont::iterator begin = chain.begin();
    typename Cont::iterator end   = chain.begin() + (hiIndex + 1);
    LessCounted<unsigned int> comp(m);

    typename Cont::iterator pos = std::lower_bound(begin, end, value, comp);
    size_t idx     = static_cast<size_t>(pos - chain.begin());
    size_t oldSize = chain.size();
    chain.insert(pos, value);

    if (m) m->inserts++;
    accountMoves(chain, oldSize - idx, m); // elements shifted right of idx
    return idx;
}

template <typename Cont>
static void fordJohnsonSort(Cont& data, PmergeMe::Metrics* m) {
    typedef unsigned int U;

    const size_t n = data.size();
    if (n <= 1) return;

    // collect pairs (min, max)
    std::vector< std::pair<U,U> > pairs;
    pairs.reserve(n/2 + 1);
    bool hasStraggler = (n % 2 != 0);
    U straggler = 0;

    size_t i = 0;
    for (; i + 1 < n; i += 2) {
        U a = data[i];
        U b = data[i+1];
        if (m) m->pairComparisons++;
        if (a <= b) pairs.push_back(std::make_pair(a, b));
        else        pairs.push_back(std::make_pair(b, a));
    }
    if (hasStraggler) straggler = data[i];

    // sort pairs by max, then min
    std::sort(pairs.begin(), pairs.end(), PairByMaxThenMinCounted<U>(m));

    // main chain = all maxima, ascending
    Cont chain;
    // make sure capacity/growth is okay; just insert one-by-one
    chain.insert(chain.end(), pairs[0].second);
    for (size_t k = 1; k < pairs.size(); ++k)
        chain.insert(chain.end(), pairs[k].second);

    // positions of maxima in the chain (to limit each binary insert)
    std::vector<size_t> maxPos(pairs.size());
    for (size_t k = 0; k < pairs.size(); ++k) maxPos[k] = k;

    // insert min of first pair within [0..maxPos[0]]
    {
        U m0 = pairs[0].first;
        size_t ins = binaryInsertLimited(chain, m0, maxPos[0], m);
        for (size_t j = 0; j < maxPos.size(); ++j)
            if (maxPos[j] >= ins) ++maxPos[j];
    }

    // Jacobsthal-driven insertion order for remaining mins (indices 1..pairs-1)
    std::vector<size_t> order = buildInsertionOrder(pairs.size());
    for (size_t t = 0; t < order.size(); ++t) {
        size_t idx = order[t];
        U value = pairs[idx].first;
        size_t hi = maxPos[idx];
        size_t ins = binaryInsertLimited(chain, value, hi, m);
        for (size_t j = 0; j < maxPos.size(); ++j)
            if (maxPos[j] >= ins) ++maxPos[j];
    }

    // straggler (if any) inserts into full chain
    if (hasStraggler) {
        LessCounted<U> comp(m);
        typename Cont::iterator pos = std::lower_bound(chain.begin(), chain.end(), straggler, comp);
        chain.insert(pos, straggler);
        if (m) m->inserts++;
        // elementMoves for vector accounted by accountMoves in binaryInsertLimited; here optional
    }

    // write back
    for (size_t k = 0; k < n; ++k) data[k] = chain[k];
}

// ---------- wrappers ----------
void PmergeMe::sortVector(std::vector<unsigned int>& v, Metrics* m) { fordJohnsonSort(v, m); }
void PmergeMe::sortDeque (std::deque <unsigned int>& d, Metrics* m) { fordJohnsonSort(d, m); }
