#ifndef MUTANTSTACK_HPP
#define MUTANTSTACK_HPP

#include <stack>
#include <deque>

// Inherit from std::stack to expose its protected container
template <typename T, typename Container = std::deque<T> >
class MutantStack : public std::stack<T, Container> {
public:
    MutantStack() {}
    MutantStack(const MutantStack& o) : std::stack<T, Container>(o) {}
    MutantStack& operator=(const MutantStack& o) {
        std::stack<T, Container>::operator=(o);
        return *this;
    }
    ~MutantStack() {}

    // iterator types
    typedef typename Container::iterator                  iterator;
    typedef typename Container::const_iterator            const_iterator;
    typedef typename Container::reverse_iterator          reverse_iterator;
    typedef typename Container::const_reverse_iterator    const_reverse_iterator;

    // begin/end
    iterator               begin()               { return this->c.begin(); }
    iterator               end()                 { return this->c.end(); }
    const_iterator         begin() const         { return this->c.begin(); }
    const_iterator         end()   const         { return this->c.end(); }

    // rbegin/rend
    reverse_iterator       rbegin()              { return this->c.rbegin(); }
    reverse_iterator       rend()                { return this->c.rend(); }
    const_reverse_iterator rbegin() const        { return this->c.rbegin(); }
    const_reverse_iterator rend()   const        { return this->c.rend(); }
};

#endif // MUTANTSTACK_HPP
