#include "Span.hpp"

Span::Span(unsigned int n)
: _capacity(n), _data() {
    _data.reserve(n); // not required, but avoids reallocs
}

Span::Span(const Span& other)
: _capacity(other._capacity), _data(other._data) {}

Span& Span::operator=(const Span& other) {
    if (this != &other) {
        _capacity = other._capacity;
        _data = other._data;
    }
    return *this;
}

Span::~Span() {}

void Span::addNumber(int value) {
    if (_data.size() >= _capacity)
        throw std::runtime_error("Span: capacity exceeded in addNumber");
    _data.push_back(value);
}

unsigned int Span::shortestSpan() const {
    if (_data.size() < 2)
        throw std::runtime_error("Span: not enough numbers for a span");
    std::vector<int> v(_data);
    std::sort(v.begin(), v.end());
    // compute min adjacent difference
    long long best = static_cast<long long>(std::numeric_limits<unsigned int>::max());
    for (std::size_t i = 1; i < v.size(); ++i) {
        long long diff = static_cast<long long>(v[i]) - static_cast<long long>(v[i-1]);
        if (diff < best) best = diff;
    }
    return static_cast<unsigned int>(best);
}

unsigned int Span::longestSpan() const {
    if (_data.size() < 2)
        throw std::runtime_error("Span: not enough numbers for a span");
    std::vector<int>::const_iterator mn = std::min_element(_data.begin(), _data.end());
    std::vector<int>::const_iterator mx = std::max_element(_data.begin(), _data.end());
    long long diff = static_cast<long long>(*mx) - static_cast<long long>(*mn);
    return static_cast<unsigned int>(diff);
}
