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
    return &(*this)[0];
};

template<typename type> const type* cppvector<type>::cend(void) const {
    return &(*this)[this->numElements];
};

template <typename type> type* cppvector<type>::begin(void) {
    return &(*this)[0];
};

template<typename type> type* cppvector<type>::end(void) {
    return &(*this)[this->numElements];
};

template<typename type> void cppvector<type>::realloc(size_t newSize) {
    if(newSize==0) { // clear vector 
        if(this->dataptr!=nullptr) delete[] this->dataptr;
        this->createEmpty(); // reinitalize vector
        return;
    }
    type* newptr = new type[newSize];
    if(this->numElements>0 && this->dataptr!=nullptr) std::move<const type*, type*>(&(*this)[0], &(*this)[newSize], newptr);
    if(this->dataptr!=nullptr) delete[] this->dataptr;
    this->dataptr          = newptr;
    this->allocatedSize = newSize;
}

template<typename type> void cppvector<type>::reserve(const size_t newSize) {
    if(newSize <= this->allocatedSize) return; // if reserving less bytes than allocated, there is no need to do anything.
    const size_t newAllocatedSize = newSize*((size_t)2); // allocate extra memory ahead of time to achieve O(1)+ time complexity for pushback operations
    this->realloc(newAllocatedSize); // reservation success
}

template<typename type> void cppvector<type>::resize(const size_t newSize) {
    if(newSize==0) {
        this->clear();
        return;
    }
    this->reserve(newSize); // allocate extra memory if needed
    this->numElements = newSize;
}

template<typename type> void cppvector<type>::clear(void) {
    if(this->dataptr!=nullptr) delete[] this->dataptr;
    this->createEmpty(); // reinitalize empty vector
}

template<typename type> void cppvector<type>::pushBack(const type newElement) {
    this->resize(this->numElements + 1);
    (*this)[this->numElements - 1] = newElement;
}

template<class type> void cppvector<type>::pushBack(const cppvector<type>& newElements) {
    if(newElements.numElements==0) {
        return;
    }
    size_t oldSize = this->numElements;
    this->resize(oldSize + newElements.numElements);
    std::copy_n<const type*, size_t, type*>((const type*)&newElements[0], newElements.numElements, &(*this)[oldSize]);
}

template<typename type> type cppvector<type>::popBack(void) {
    if(this->numElements==0) {
        throw std::domain_error("cppvector: can't pop back element with size zero!");
    }
    const type poppedElement = std::move((*this)[this->numElements-1]);
    this->resize(this->numElements - 1);
    return poppedElement;
}

template<typename type> type& cppvector<type>::back(void) {
    if(this->numElement==0) {
        throw std::domain_error("cppvector: can't access back element with size zero!");
    }
    return (*this)[this->numElements-1];
}

template<typename type> type& cppvector<type>::front(void) {
    if(this->numElements==0) {
        throw std::domain_error("cppvector: can't access front element with size zero!");
    }
    return (*this)[0];
}

template<typename type> void cppvector<type>::fit(void) {
    if(this->allocatedSize == this->numElements) return;
    this->realloc(this->numElements);
}

template<typename type> void cppvector<type>::createEmpty(void) {
    this->dataptr          = nullptr;
    this->numElements   = 0;
    this->allocatedSize = 0;
}

template<typename type> void cppvector<type>::copyInitalize(const class cppvector<type>& src) {
    if(src.numElements==0) { // don't copy empty vector
        this->createEmpty();
        return;
    }
    this->numElements   = src.numElements;
    this->allocatedSize = this->numElements;
    this->dataptr          = new type[this->allocatedSize];
    std::copy_n<const type*, size_t, type*>((const type*)&src[0], this->numElements, &(*this)[0]);
}

template<typename type> void cppvector<type>::transferOwnershipTo(class cppvector<type>& dest) {
    // give dest access to the dataptr
    dest.dataptr            = this->dataptr;
    dest.numElements     = this->numElements;
    dest.allocatedSize   = this->allocatedSize;
    // remove dataptr access from this object
    this->dataptr           = nullptr;
    this->numElements    = 0;
    this->allocatedSize = 0;
}

template<typename type> void cppvector<type>::moveInitalize(class cppvector<type>&& src) {
    // doesn't do a deep copy, instead changes ownership of the memory
    // the change in ownership makes it no longer possible for src to delete the allocation
    src.transferOwnershipTo(*this);
}

template<typename type> cppvector<type>::cppvector(void) {
    this->createEmpty();
}

template<typename type> cppvector<type>::cppvector(const class cppvector<type>& src) {
    this->copyInitalize(src);
}

template<typename type> cppvector<type>::cppvector(class cppvector<type>&& src) {
    this->moveInitalize(std::move(src)); // the std::move is required here
}

template<typename type> const type& cppvector<type>::operator[](size_t index) const {
    return this->dataptr[index];
}

template<typename type> type& cppvector<type>::operator[](size_t index) {
    return this->dataptr[index];
}

template<typename type> const type& cppvector<type>::operator()(size_t index) const {
    if(index<0 || index>=this->numElements) {
        throw std::domain_error("cppvector: index out of bounds");
    }
    return (*this)[index];
}

template<typename type> type& cppvector<type>::operator()(size_t index) {
    if(index<0 || index>=this->numElements) {
        throw std::domain_error("cppvector: index out of bounds");
    }
    return (*this)[index];
}

template<typename type> size_t cppvector<type>::size(void) const {
    return this->numElements;
}

template<typename type> size_t cppvector<type>::allocationSize(void) const {
    return this->allocatedSize;
}

template<typename type> cppvector<type>::~cppvector(void) {
    this->clear();
}
#endif