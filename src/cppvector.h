#ifndef _cppvectorh
#define _cppvectorh

/*
    compile with:
    g++ -o cppvector.o -c cppvector.cpp -Wall
*/


// NOTE: no iterators because there would be more overhead using the iterator than using pointers

#include <cstddef>
#include <utility>

template<typename type> class cppvector {
    public:
    type* begin(void) const;
    type* end(void) const;
    cppvector(void);
    cppvector(const class cppvector&);
    cppvector(class cppvector&&);
    ~cppvector(void);
    
    bool reserve(const size_t);
    bool resize(const size_t);
    bool pushBack(const type);
    type popBack(void);
    bool fit(void);

    size_t size(void) const;
    size_t allocationSize(void) const;

    type  operator[](size_t) const;
    type& operator[](size_t);

    private:
    type* data;
    size_t numElements;
    size_t allocatedSize;
    bool realloc(size_t);
};

#include "cppvector.cpp"

#endif