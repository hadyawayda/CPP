/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <set>
#include <sstream>
#include <climits>
#include <algorithm>

/** File-local: Jacobsthal number J(n). */
static int fj_jnum(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    int a = 0, b = 1, c = 0;
    for (int i = 2; i <= n; ++i) { c = b + 2 * a; a = b; b = c; }
    return c;
}

/** File-local: Jacobsthal cutoffs ≤ size, starting from J(3). */
static std::vector<int> fj_jbase(int size) {
    std::vector<int> out;
    for (int k = 3;; ++k) {
        int j = fj_jnum(k);
        if (j > size) break;
        out.push_back(j);
    }
    return out;
}

/** File-local: expand base cutoffs to a full 1-based insertion schedule of length total-1. */
static void fj_expandSchedule(std::vector<int>& sched, int total) {
    std::vector<int> tmp;
    std::vector<char> used(total + 1, 0);
    for (size_t i = 0; i < sched.size(); ++i)
        for (int x = sched[i]; x > 1; --x)
            if (!used[x]) { tmp.push_back(x); used[x] = 1; }
    for (int x = total; static_cast<int>(tmp.size()) < total - 1 && x > 1; --x)
        if (!used[x]) { tmp.push_back(x); used[x] = 1; }
    sched.swap(tmp);
}

/** File-local: bounded binary search with inclusive high bound; bumps cmp per iteration. */
template <typename Cont>
static int fj_boundSearch(const Cont& main, unsigned int key, int hiIncl, int& cmp) {
    if (main.empty()) return 0;
    if (hiIncl >= static_cast<int>(main.size())) hiIncl = static_cast<int>(main.size()) - 1;
    int lo = 0, hi = hiIncl;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        ++cmp;
        if (main[mid] == key) return mid;
        else if (main[mid] > key) { if (mid == 0) break; hi = mid - 1; }
        else lo = mid + 1;
    }
    if (lo < static_cast<int>(main.size()) && key < main[lo]) return lo;
    return static_cast<int>(main.size());
}

/** File-local: Ford–Johnson core over a generic contiguous/indexable container of unsigned int. */
template <typename Cont>
static Cont fondJohnsonCore(Cont& data, int& cmpCounter) {
    if (data.empty()) return Cont();
    if (data.size() == 1) return data;
    if (data.size() == 2) {
        Cont r = data;
        ++cmpCounter;
        if (r[0] > r[1]) std::swap(r[0], r[1]);
        return r;
    }

    Cont winners, losers, res;
    const bool odd = (data.size() & 1u);
    for (size_t i = 0; i + 1 < data.size(); i += 2) {
        ++cmpCounter;
        if (data[i] > data[i + 1]) { winners.push_back(data[i]); losers.push_back(data[i + 1]); }
        else { losers.push_back(data[i]); winners.push_back(data[i + 1]); }
    }
    if (odd) losers.push_back(data.back());

    Cont sortedWinners = fondJohnsonCore(winners, cmpCounter);

    std::vector<unsigned int> alignedLosers(sortedWinners.size());
    for (size_t i = 0; i < sortedWinners.size(); ++i) {
        for (size_t j = 0; j < winners.size(); ++j) {
            if (winners[j] == sortedWinners[i]) { alignedLosers[i] = losers[j]; break; }
        }
    }
    if (odd) alignedLosers.push_back(losers.back());

    res = sortedWinners;

    std::vector<int> sched = fj_jbase(static_cast<int>(alignedLosers.size()));
    fj_expandSchedule(sched, static_cast<int>(alignedLosers.size()));

    res.insert(res.begin(), alignedLosers[0]);

    int cap = 3;
    for (size_t s = 0; s < sched.size(); ++s) {
        if (s > 0 && sched[s] > sched[s - 1]) cap = 2 * cap + 1;
        if (sched[s] <= static_cast<int>(alignedLosers.size()) && sched[s] != 1) {
            const int idx = sched[s] - 1;
            const int pos = fj_boundSearch(res, alignedLosers[idx], cap - 1, cmpCounter);
            if (pos >= 0) res.insert(res.begin() + pos, alignedLosers[idx]);
        }
    }
    return res;
}

/** Canonical constructor. */
PmergeMe::PmergeMe() : _cmpVec(0), _cmpDeq(0) {}
/** Canonical copy. */
PmergeMe::PmergeMe(const PmergeMe& other) { *this = other; }
/** Canonical assignment. */
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) { _cmpVec = other._cmpVec; _cmpDeq = other._cmpDeq; }
    return *this;
}
/** Canonical destructor. */
PmergeMe::~PmergeMe() {}

/** Validates argv: positive integers, unique, at least two. */
void PmergeMe::validateArgs(char **av) {
    if (!av[1]) throw std::invalid_argument("Error");
    std::set<unsigned int> seen;
    int count = 0;
    for (int i = 1; av[i]; ++i) {
        std::stringstream ss(av[i]);
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
            ++count;
        }
    }
    if (count < 2) throw std::invalid_argument("Error");
}

/** Sorts vector<unsigned int> in place using Ford–Johnson. */
void PmergeMe::fordJohnsonVect(std::vector<unsigned int>& data) {
    _cmpVec = 0;
    data = fondJohnsonCore(data, _cmpVec);
}

/** Sorts deque<unsigned int> in place using Ford–Johnson. */
void PmergeMe::fordJohnsonDeq(std::deque<unsigned int>& data) {
    _cmpDeq = 0;
    data = fondJohnsonCore(data, _cmpDeq);
}

/** Comparison count from last vector run. */
int PmergeMe::vectorComparisons() const { return _cmpVec; }

/** Comparison count from last deque run. */
int PmergeMe::dequeComparisons() const { return _cmpDeq; }
