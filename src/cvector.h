#ifndef _cvectorh
#define _cvectorh

/*
    compile with:
    gcc-o cvector.o -c cvector.c -Wall
*/

#include <stdbool.h>
#include <memory.h>
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
struct cvector newCVector(size_t elementSize); // initalizes CVector struct
bool reserveCVector(struct cvector* cvec, size_t numElements); // reserves extra space if needed, O(1)+
bool resizeCVector(struct cvector* cvec, size_t numElements); // resizes the vector if needed, O(1)+
bool pushBackCVector(struct cvector* cvec, const void* newElement); // pushes element onto the end of the vector, O(1)+
bool popBackCVector(struct cvector* cvec, void* poppedElement); // pops last element off of the vector, O(1)
bool fitCVector(struct cvector* cvec); // fits the allocation to the data, O(1)+ (depends on realloc implementation when it comes to shrinking allocations)
void freeCVector(struct cvector* cvec); // frees allocated data

bool deepFreeCVector(struct cvector* cvec); // does a deep free on allocated data, deepFree function can be set to free all elements
bool deepCopyCVector(struct cvector* dest, const struct cvector* src); // deepcopies the vector, deepcopy function can be set

#endif