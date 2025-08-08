#ifndef SPAN_HPP
#define SPAN_HPP

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator> // std::distance
#include <limits>

class Span {
public:
    // Ctor / Canonical
    explicit Span(unsigned int n);
    Span(const Span& other);
    Span& operator=(const Span& other);
    ~Span();

    // Single add
    void addNumber(int value);

    // Bulk add via iterator range (templated → must live in header)
    template <typename InputIt>
    void addRange(InputIt first, InputIt last) {
        // pre-check size to avoid partial insert
        unsigned int count = static_cast<unsigned int>(std::distance(first, last));
        if (count == 0) return;
        if (_data.size() + count > _capacity)
            throw std::runtime_error("Span: capacity exceeded in addRange");
        for (; first != last; ++first)
            _data.push_back(*first);
    }

    // Queries
    unsigned int size() const { return static_cast<unsigned int>(_data.size()); }
    unsigned int capacity() const { return _capacity; }

    // Span computations
    unsigned int shortestSpan() const;
    unsigned int longestSpan() const;

private:
    unsigned int   _capacity;
    std::vector<int> _data;
};

#endif // SPAN_HPP
