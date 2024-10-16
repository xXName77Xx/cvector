#ifndef _cppvectorh
#define _cppvectorh

// NOTE: no iterators because there would be more overhead using the iterator than using pointers

#include <cstddef>
#include <utility>
#include <stdexcept>

#include <algorithm>
#include <iterator>

template<typename type> class cppvector;

template<typename type> bool operator==(const class cppvector<type>&, const class cppvector<type>&);
template<typename type> bool operator!=(const class cppvector<type>&, const class cppvector<type>&);
template<typename type> class cppvector<type> operator+(const class cppvector<type>&, const class cppvector<type>&);


template<typename type> class cppvector {
    public:
    type* begin(void);
    type* end(void);
    const type* cbegin(void) const;
    const type* cend(void) const;
    cppvector(void);
    cppvector(const class cppvector&);
    cppvector(class cppvector&&);
    ~cppvector(void);
    
    void reserve(const size_t);
    void resize(const size_t);
    void pushBack(const type);
    void pushBack(const class cppvector<type>);
    type popBack(void);
    bool fit(void);

    void clear(void);

    size_t size(void) const;
    size_t allocationSize(void) const;

    type& back(void);
    type& front(void);

    const type& operator[](size_t) const;
    type&       operator[](size_t);

    const type& operator()(size_t) const;
    type&       operator()(size_t);

    private:
    void createInitalize(void);
    void copyInitalize(const class cppvector&);
    void moveInitalize(class cppvector<type>&&);
    type* data;
    size_t numElements;
    size_t allocatedSize;
    void realloc(size_t);
};

#include "cppvector.cpp"

#endif