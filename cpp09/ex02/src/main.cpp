#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <sstream>
#include <limits>
#include <cctype>
#include <sys/time.h> // gettimeofday

// -------- timing --------
static long long nowMicros() {
    struct timeval tv; gettimeofday(&tv, 0);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

// -------- pretty print --------
template <typename Cont>
static void printLine(const char* prefix, const Cont& c) {
    std::cout << prefix;
    for (typename Cont::const_iterator it = c.begin(); it != c.end(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << std::endl;
}

// -------- tiny parsing helpers (local to main) --------
static bool parseOneToken(const std::string& tok, unsigned int& value) {
    if (tok.empty()) return false;
    for (std::string::size_type i = 0; i < tok.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(tok[i]);
        if (!std::isdigit(ch)) return false;
    }
    std::istringstream iss(tok);
    unsigned long ul = 0;
    iss >> ul;
    if (!iss || !iss.eof()) return false;
    if (ul == 0 || ul > std::numeric_limits<unsigned int>::max()) return false;
    value = static_cast<unsigned int>(ul);
    return true;
}

static bool parseArgsToBoth(int argc, char** argv,
                            std::vector<unsigned int>& outVec,
                            std::deque<unsigned int>&  outDeq)
{
    for (int i = 1; i < argc; ++i) {
        std::istringstream ss(argv[i]);  // allow "1 2 3" inside one arg
        std::string tok;
        while (ss >> tok) {
            unsigned int v = 0;
            if (!parseOneToken(tok, v)) return false;
            outVec.push_back(v);
            outDeq.push_back(v);
        }
    }
    return !outVec.empty();
}

// -------- main --------
int main(int argc, char** argv) {
    std::vector<unsigned int> inputV;
    std::deque<unsigned int>  inputD;

    if (!parseArgsToBoth(argc, argv, inputV, inputD)) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    printLine("Before:", inputV);

    // copies for sorting
    std::vector<unsigned int> v = inputV;
    std::deque<unsigned int>  d = inputD;

    // Note: we NEVER reset PmergeMe::comparisons.
    // We compute per-run counts using deltas.
    const int startTotal = PmergeMe::comparisons;

    // vector run
    const int startVecCmp = PmergeMe::comparisons;
    long long t0 = nowMicros();
    PmergeMe::fordJohnsonSort(v);
    long long t1 = nowMicros();
    const int vecComparisons = PmergeMe::comparisons - startVecCmp;

    // deque run
    const int startDeqCmp = PmergeMe::comparisons;
    long long t2 = nowMicros();
    PmergeMe::fordJohnsonSort(d);
    long long t3 = nowMicros();
    const int deqComparisons = PmergeMe::comparisons - startDeqCmp;

    // totals since program start (never reset)
    const int totalComparisons = PmergeMe::comparisons - startTotal;

    printLine("After: ", v);

    const std::size_t N = inputV.size();
    std::cout << "Time to process a range of " << N
              << " elements with std::vector : " << (t1 - t0) << " us\n";
    std::cout << "Comparisons (vector): " << vecComparisons << "\n";

    std::cout << "Time to process a range of " << N
              << " elements with std::deque  : " << (t3 - t2) << " us\n";
    std::cout << "Comparisons (deque): " << deqComparisons << "\n";

    std::cout << "Comparisons (total since start): " << totalComparisons << std::endl;

    // (optional simple check)
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i-1] > v[i]) { std::cerr << "ERROR: vector not sorted\n"; return 1; }
    }
    for (std::size_t i = 1; i < d.size(); ++i) {
        if (d[i-1] > d[i]) { std::cerr << "ERROR: deque not sorted\n"; return 1; }
    }

    return 0;
}
