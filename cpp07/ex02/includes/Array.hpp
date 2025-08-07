#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <exception>

template <typename T>
class Array {
private:
    T*               data_;
    unsigned int     size_;

public:
    // default: empty array
    Array() 
        : data_(NULL), size_(0) 
    {}

    // n elements, default‐initialized
    Array(unsigned int n) 
        : data_(NULL), size_(n) 
    {
        if (size_ > 0)
            data_ = new T[size_];
    }

    // copy constructor (deep copy)
    Array(Array const& other) 
        : data_(NULL), size_(other.size_) 
    {
        if (size_ > 0) {
            data_ = new T[size_];
            for (unsigned int i = 0; i < size_; ++i)
                data_[i] = other.data_[i];
        }
    }

    // assignment operator (deep copy)
    Array& operator=(Array const& other) {
        if (this != &other) {
            delete [] data_;
            size_ = other.size_;
            if (size_ > 0) {
                data_ = new T[size_];
                for (unsigned int i = 0; i < size_; ++i)
                    data_[i] = other.data_[i];
            } else {
                data_ = NULL;
            }
        }
        return *this;
    }

    // destructor
    ~Array() {
        delete [] data_;
    }

    // subscript operator, throws if idx out of [0..size_-1]
    T& operator[](int idx) {
        if (idx < 0 || static_cast<unsigned int>(idx) >= size_)
            throw std::exception();
        return data_[idx];
    }

    // const‐version of operator[]
    T const& operator[](int idx) const {
        if (idx < 0 || static_cast<unsigned int>(idx) >= size_)
            throw std::exception();
        return data_[idx];
    }

    // number of elements
    unsigned int size() const {
        return size_;
    }
};

#endif  // ARRAY_HPP
