#ifndef _cppvectorcpp
#define _cppvectorcpp
#include "cppvector.h"

// public functions
template<typename type> cppvector<type>::cppvector(void) {
    this->data = new type[0];
    this->numElements = 0;
    this->allocatedSize = 0;
}

template<typename type> type* cppvector<type>::begin(void) const {
    return &(this->data[0]);
};
template<typename type> type* cppvector<type>::end(void) const {
    return &(this->data[this->size() - 1]);
};

template<typename type> bool cppvector<type>::realloc(size_t newSize) {
    type* newptr = new type[newSize];
    if(newptr==NULL) return false; // allocation failed
    for(size_t i=0; i<newSize; i++) {
        newptr[i] = std::move((*this)[i]);
    }
    delete[] this->data;
    this->data = newptr;
    this->allocatedSize = newSize;
    return true;
}

template<typename type> bool cppvector<type>::reserve(const size_t newSize) {
    if(newSize <= this->allocatedSize) return true; // if reserving less bytes than allocated, there is no need to do anything.
    const size_t newAllocatedSize = newSize*((size_t)2); // allocate extra memory ahead of time to achieve O(1)+ time complexity for pushback operations
    return this->realloc(newAllocatedSize); // reservation success
}

template<typename type> bool cppvector<type>::resize(const size_t newSize) {
    if(!this->reserve(newSize)) return false; // if needed. allocate extra memory, pass error to caller as needed
    // destruct elements that no longer exist
    for(size_t i = newSize; i<this->size(); i++) {
        (*this)[i].~type();
    }
    // construct new elements
    for(size_t i = this->size(); i<newSize; i++) {
        (*this)[i] = type();
    }
    this->numElements = newSize;
    return true; // resize success
}

template<typename type> bool cppvector<type>::pushBack(const type newElement) {
    if(!this->resize(this->size() + 1)) return false; // resize error
    (*this)[this->size() - 1] = newElement;
    return true; // pushback succcess
}

template<typename type> type cppvector<type>::popBack(void) {
    if(this->size()<=0) return type(); // can't pop elements off of empty vector
    const size_t popElementIndex = this->size() - 1;
    const type poppedElement = std::move((*this)[popElementIndex]);
    this->resize(this->size() - 1);
    return poppedElement;
}

template<typename type> bool cppvector<type>::fit(void) {
    if(this->allocatedSize == this->size()) return true; // allocation aready fits the data
    else if(this->allocatedSize < this->size()) return false; // error, more data than the allocation has size for
    // otherwise it is valid for fitting the allocation to the data
    return this->realloc(this->size()); // reservation success
}

template<typename type> cppvector<type>::cppvector(const class cppvector<type>& src) {
    this->numElements = src.size();
    this->allocatedSize = this->size();
    this->data = new type[this->size()];
    // NOTE: SHOULD THROW ERROR ON FAILED ALLOCATION

    for(size_t i=0; i<this->size(); i++) {
        (*this)[i] = src[i];
    }
}

template<typename type> cppvector<type>::cppvector(class cppvector<type>&& src) {
    this->numElements = src.size();
    this->allocatedSize = src.allocationSize();
    this->data = &src[0];
}

template<typename type> type cppvector<type>::operator[](size_t index) const {
    return this->data[index];
}

template<typename type> type& cppvector<type>::operator[](size_t index) {
    return this->data[index];
}

template<typename type> size_t cppvector<type>::size(void) const {
    return this->numElements;
}

template<typename type> size_t cppvector<type>::allocationSize(void) const {
    return this->allocatedSize;
}

template<typename type> cppvector<type>::~cppvector(void) {
    for(size_t i=0; i<this->size(); i++) {
        (*this)[i].~type();
    }
    delete[] this->data;
}
#endif