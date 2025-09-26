#include "PmergeMe.hpp"
#include <set>
#include <sstream>
#include <climits>
#include <algorithm>
#include <cctype>

int g_comparisons = 0;

/*------------------------ Orthodox Canonical Form ------------------------*/

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) { (void)other; }
PmergeMe& PmergeMe::operator=(const PmergeMe& other) { (void)other; return *this; }
PmergeMe::~PmergeMe() {}

/*--------------------- Internal: Jacobsthal helpers ---------------------*/

/* Returns Jacobsthal cutoffs J(3),J(4),... that are <= size. */
static std::vector<int> jBase(int size) {
    std::vector<int> out;
    int a = 0, b = 1;
    for (int k = 2;; ++k) {
        int j = b + 2 * a;
        a = b; b = j;
        if (k >= 3) {
            if (j > size) break;
            out.push_back(j);
        }
    }
    return out;
}

/* Expands base cutoffs into a 1-based insertion schedule of length total-1. */
static void buildSchedule(std::vector<int>& sched, int total) {
    std::vector<int> tmp;
    std::vector<char> used(total + 1, 0);
    for (size_t i = 0; i < sched.size(); ++i)
        for (int x = sched[i]; x > 1; --x)
            if (!used[x]) { tmp.push_back(x); used[x] = 1; }
    for (int x = total; static_cast<int>(tmp.size()) < total - 1 && x > 1; --x)
        if (!used[x]) { tmp.push_back(x); used[x] = 1; }
    sched.swap(tmp);
}

/*--------------------- Internal: bounded search ---------------------*/

/* Performs lower-bound search on [0..hiIncl], inclusive-high; counts one comparison per step. */
template <typename Cont>
static int boundedLower(const Cont& a, unsigned int key, int hiIncl) {
    if (a.empty()) return 0;
    if (hiIncl >= static_cast<int>(a.size())) hiIncl = static_cast<int>(a.size()) - 1;
    int lo = 0, hi = hiIncl;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        ++g_comparisons;
        if (a[mid] == key) return mid;
        else if (a[mid] > key) { if (mid == 0) break; hi = mid - 1; }
        else lo = mid + 1;
    }
    if (lo < static_cast<int>(a.size()) && key < a[lo]) return lo;
    return static_cast<int>(a.size());
}

/*--------------------- Internal: pair phase ---------------------*/

/* Splits input into winners/losers with exactly one comparison per pair; returns odd flag/value. */
template <typename Cont>
static void makePairs(const Cont& in, Cont& winners, Cont& losers, bool& hasOdd, unsigned int& oddVal) {
    winners.clear(); losers.clear();
    hasOdd = ((in.size() & 1u) != 0u);
    for (size_t i = 0; i + 1 < in.size(); i += 2) {
        ++g_comparisons;
        if (in[i] > in[i + 1]) { winners.push_back(in[i]); losers.push_back(in[i + 1]); }
        else { losers.push_back(in[i]); winners.push_back(in[i + 1]); }
    }
    if (hasOdd) oddVal = in.back();
}

/*------------------------ Internal: alignment ------------------------*/

/* Aligns losers to the order of the sorted winners (inputs are unique). */
template <typename Cont>
static std::vector<unsigned int> alignLosers(const Cont& winnersOrig,
                                             const Cont& losersOrig,
                                             const Cont& winnersSorted)
{
    std::vector<unsigned int> aligned(winnersSorted.size());
    for (size_t i = 0; i < winnersSorted.size(); ++i) {
        for (size_t j = 0; j < winnersOrig.size(); ++j) {
            if (winnersOrig[j] == winnersSorted[i]) { aligned[i] = losersOrig[j]; break; }
        }
    }
    return aligned;
}

/*------------------------ Internal: insertion steps ------------------------*/

/* Inserts the first pending loser at the front with no comparisons. */
template <typename Cont>
static void placeFirst(Cont& chain, const std::vector<unsigned int>& aligned) {
    if (!aligned.empty()) chain.insert(chain.begin(), aligned[0]);
}

/* Inserts remaining losers using the Jacobsthal schedule with a tight increasing cap. */
template <typename Cont>
static void placeRest(Cont& chain, const std::vector<unsigned int>& aligned) {
    if (aligned.size() <= 1) return;
    std::vector<int> sched = jBase(static_cast<int>(aligned.size()));
    buildSchedule(sched, static_cast<int>(aligned.size()));
    int cap = 3;
    for (size_t s = 0; s < sched.size(); ++s) {
        if (s > 0 && sched[s] > sched[s - 1]) cap = 2 * cap + 1;
        const int idx = sched[s] - 1;
        if (idx >= 1 && idx < static_cast<int>(aligned.size())) {
            const int pos = boundedLower(chain, aligned[idx], cap - 1);
            chain.insert(chain.begin() + pos, aligned[idx]);
        }
    }
}

/*------------------------ Internal: Ford–Johnson core ------------------------*/

/* Recursively sorts using the pair phase, recursion on winners, and Jacobsthal-guided insertions. */
template <typename Cont>
static Cont fordJohnsonCore(Cont& data) {
    if (data.empty())  return Cont();
    if (data.size() == 1) return data;
    if (data.size() == 2) {
        Cont r = data;
        ++g_comparisons;
        if (r[0] > r[1]) std::swap(r[0], r[1]);
        return r;
    }
    Cont winners, losers;
    bool hasOdd = false; unsigned int oddVal = 0;
    makePairs(data, winners, losers, hasOdd, oddVal);
    Cont sortedWinners = fordJohnsonCore(winners);
    std::vector<unsigned int> aligned = alignLosers(winners, losers, sortedWinners);
    if (hasOdd) aligned.push_back(oddVal);
    Cont chain = sortedWinners;
    placeFirst(chain, aligned);
    placeRest(chain, aligned);
    return chain;
}

/*------------------------ Parsing & Validation ------------------------*/

/* Parses argv, validates positives/uniqueness/size>=2, fills both containers or throws. */
void PmergeMe::parseInputs(int argc, char** argv,
                           std::vector<unsigned int>& outVec,
                           std::deque<unsigned int>&  outDeq)
{
    if (argc < 2) throw std::invalid_argument("Error");
    std::set<unsigned int> seen;
    int count = 0;
    for (int i = 1; i < argc; ++i) {
        std::stringstream ss(argv[i]);
        std::string tok;
        while (ss >> tok) {
            for (size_t j = 0; j < tok.size(); ++j) {
                unsigned char ch = static_cast<unsigned char>(tok[j]);
                if (!std::isdigit(ch)) throw std::invalid_argument("Error");
            }
            unsigned long v = std::strtoul(tok.c_str(), 0, 10);
            if (v == 0UL || v > UINT_MAX) throw std::invalid_argument("Error");
            if (!seen.insert(static_cast<unsigned int>(v)).second)
                throw std::invalid_argument("Error");
            outVec.push_back(static_cast<unsigned int>(v));
            outDeq.push_back(static_cast<unsigned int>(v));
            ++count;
        }
    }
    if (count < 2) throw std::invalid_argument("Error");
}

/*------------------------ Public Entrypoints ------------------------*/

/* Sorts vector<unsigned int> in place using Ford–Johnson. */
void PmergeMe::fordJohnsonVect(std::vector<unsigned int>& data) {
    data = fordJohnsonCore(data);
}

/* Sorts deque<unsigned int> in place using Ford–Johnson. */
void PmergeMe::fordJohnsonDeq(std::deque<unsigned int>& data) {
    data = fordJohnsonCore(data);
}
