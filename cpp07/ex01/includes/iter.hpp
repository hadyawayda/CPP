#ifndef ITER_HPP
#define ITER_HPP

#include <cstddef>

// Non-const array version: allows functions taking T&.
template <typename T, typename Func>
void iter(T* array, std::size_t length, Func f) {
    for (std::size_t i = 0; i < length; ++i)
        f(array[i]);
}

// Const array version: allows functions taking T const&.
template <typename T, typename Func>
void iter(T const* array, std::size_t length, Func f) {
    for (std::size_t i = 0; i < length; ++i)
        f(array[i]);
}

#endif // ITER_HPP
