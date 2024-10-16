#ifndef _cppvectorcpp
#define _cppvectorcpp
#include "cppvector.h"

// public functions
template<typename type> bool operator==(const class cppvector<type>& a, const class cppvector<type>& b) {
    if(a.size()!=b.size()) return false;
    return std::equal<const type*, const type*>((const type*)&a[0], (const type*)&a[a.size()], (const type*)&b[0]);
}

template<typename type> bool operator!=(const class cppvector<type>& a, const class cppvector<type>& b) {
    return !(a==b);
}

template<typename type> class cppvector<type> operator+(const class cppvector<type>& a, const class cppvector<type>& b) {
    class cppvector<type> output = a;
    output+=b;
    return output;
}

template<typename type> class cppvector<type> operator+=(class cppvector<type>& a, const class cppvector<type>& b) {
    a.pushBack(b);
    return a;
}

template <typename type> const type* cppvector<type>::cbegin(void) const {
    return &this->data[0];
};

template<typename type> const type* cppvector<type>::cend(void) const {
    return &this->data[this->numElements];
};

template <typename type> type* cppvector<type>::begin(void) {
    return &this->data[0];
};

template<typename type> type* cppvector<type>::end(void) {
    return &this->data[this->numElements];
};

template<typename type> void cppvector<type>::realloc(size_t newSize) {
    type* newptr = new type[newSize];
    if(this->numElements>0) std::move<const type*, type*>(&(*this)[0], &(*this)[newSize], newptr);
    delete[] this->data;
    this->data = newptr;
    this->allocatedSize = newSize;
}

template<typename type> void cppvector<type>::reserve(const size_t newSize) {
    if(newSize <= this->allocatedSize) return; // if reserving less bytes than allocated, there is no need to do anything.
    const size_t newAllocatedSize = newSize*((size_t)2); // allocate extra memory ahead of time to achieve O(1)+ time complexity for pushback operations
    this->realloc(newAllocatedSize); // reservation success
}

template<typename type> void cppvector<type>::resize(const size_t newSize) {
    this->reserve(newSize); // allocate extra memory if needed
    this->numElements = newSize;
    if(this->numElements==0) {
        this->fit(); // this can free up a lot of memory when the vector is cleared, and doesn't waste time with copy operations
    }
}

template<typename type> void cppvector<type>::clear(void) {
    this->resize(0);
    this->fit();
}

template<typename type> void cppvector<type>::pushBack(const type newElement) {
    this->resize(this->numElements + 1);
    (*this)[this->numElements-1] = newElement;
}

template<class type> void cppvector<type>::pushBack(const cppvector<type> newElements) {
    size_t oldSize = this->numElements;
    size_t addSize = newElements.size();
    this->resize(oldSize + addSize);
    std::copy_n<const type*, size_t, type*>((const type*)&newElements[0], addSize, &(*this)[oldSize]);
}

template<typename type> type cppvector<type>::popBack(void) {
    if(this->numElements<=0) return type(); // can't pop elements off of empty vector
    const size_t newSize = this->numElements - 1;
    const type poppedElement = std::move(this->back());
    this->resize(newSize);
    return poppedElement;
}

template<typename type> type& cppvector<type>::back(void) {
    if(this->numElements<=0) {
        throw std::length_error("cppvector can't access back element with size zero!");
    }
    return (*this)[this->numElements-1];
}

template<typename type> type& cppvector<type>::front(void) {
    if(this->numElements<=0) {
        throw std::length_error("cppvector can't access front element with size zero!");
    }
    return (*this)[0];
}

template<typename type> void cppvector<type>::fit(void) {
    if(this->allocatedSize != this->numElements) this->realloc(this->numElements);
}

template<typename type> void cppvector<type>::createInitalize(void) {
    this->data = new type[0];
    this->numElements = 0;
    this->allocatedSize = 0;
}

template<typename type> void cppvector<type>::copyInitalize(const class cppvector<type>& src) {
    this->numElements   = src.size();
    this->allocatedSize = this->numElements;
    this->data          = new type[this->allocatedSize];
    std::copy_n<const type*, size_t, type*>((const type*)&src[0], this->numElements, &(*this)[0]);
}

template<typename type> void cppvector<type>::moveInitalize(class cppvector<type>&& src) {
    this->numElements   = src.size();
    this->allocatedSize = src.allocationSize();
    this->data          = &src[0];
}

template<typename type> cppvector<type>::cppvector(void) {
    this->createInitalize();
}

template<typename type> cppvector<type>::cppvector(const class cppvector<type>& src) {
    this->copyInitalize(src);
}

template<typename type> cppvector<type>::cppvector(class cppvector<type>&& src) {
    this->moveInitalize(std::move(src));
}

template<typename type> const type& cppvector<type>::operator[](size_t index) const {
    return this->data[index];
}

template<typename type> type& cppvector<type>::operator[](size_t index) {
    return this->data[index];
}

template<typename type> const type& cppvector<type>::operator()(size_t index) const {
    return (*this)[index];
}

template<typename type> type& cppvector<type>::operator()(size_t index) {
    return (*this)[index];
}

template<typename type> size_t cppvector<type>::size(void) const {
    return this->numElements;
}

template<typename type> size_t cppvector<type>::allocationSize(void) const {
    return this->allocatedSize;
}

template<typename type> cppvector<type>::~cppvector(void) {
    std::destroy<type*>(&(*this)[0], &(*this)[this->numElements]);
    delete[] this->data;
}
#endif