#ifndef _cvectorh
#define _cvectorh

/*
    compile with:
    gcc-o cvector.o -c cvector.c -Wall
*/


// NOTE: no iterators because there would be more overhead using the iterator than using pointers

#include <stdbool.h>
#include <malloc.h>

struct cvector {
    void* data;
    size_t numElements;
    size_t elementSize;
    size_t allocatedSize;
    bool (*deepCopyFunc)(void*, const void*, size_t);
    bool (*deepFreeFunc)(void*);
};

// functions that return a bool returns false on failure
struct cvector newCVector(const size_t elementSize); // initalizes CVector struct
bool reserveCVector(struct cvector*const cvec, const size_t numElements); // reserves extra space if needed, O(1)+
bool resizeCVector(struct cvector*const cvec, const size_t numElements); // resizes the vector if needed, O(1)+
bool pushBackCVector(struct cvector*const cvec, const void*const newElement); // pushes element onto the end of the vector, O(1)+
bool popBackCVector(struct cvector*const cvec, void*const poppedElement); // pops last element off of the vector, O(1)
bool fitCVector(struct cvector*const cvec); // fits the allocation to the data, O(1)+ (depends on realloc implementation when it comes to shrinking allocations)
void freeCVector(struct cvector*const cvec); // frees allocated data

bool deepFreeCVector(struct cvector*const cvec); // does a deep free on allocated data, deepFree function can be set to free all elements
bool deepCopyCVector(struct cvector*const dest, const struct cvector*const src); // deepcopies the vector, deepcopy function can be set

#endif