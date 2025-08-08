#ifndef EASYFIND_HPP
#define EASYFIND_HPP

#include <algorithm> // std::find

// Non-const container overload: returns iterator
template <typename T>
typename T::iterator easyfind(T& container, int value) {
    return std::find(container.begin(), container.end(), value);
}

// Const container overload: returns const_iterator
template <typename T>
typename T::const_iterator easyfind(const T& container, int value) {
    return std::find(container.begin(), container.end(), value);
}

#endif // EASYFIND_HPP
