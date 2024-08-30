#include "cvector.h"

// private functions
static void cvector_memcpy(void*const dest, const void*const src, const size_t size) {
    for(size_t i=0; i<size; i++) {
        ((unsigned char*)dest)[i] = ((unsigned char*)src)[i];
    }
}

static size_t cvector_getSizeOfCVectorinBytes(const struct cvector*const cvec) {
    return cvec->numElements*cvec->elementSize;
}

// default functions (only used internally)
static bool cvector_defaultDeepCopy(void* dest, const void* src, size_t elemSize) {
    cvector_memcpy(dest, src, elemSize);
    return true;
}

static bool cvector_defaultDeepFree(void* ptr) {
    return true;
}

// public functions
struct cvector newCVector(const size_t elementSize) {
    struct cvector cvec;
    cvec.data = (void*)cvec_malloc(0); // get valid allocation, does not check for NULL values as malloc sometimes returns those as a valid response for a zero byte allocation
    cvec.numElements = 0;
    cvec.elementSize = (elementSize<=0)?1:elementSize;
    cvec.allocatedSize = 0;
    // set up default functions
    cvec.deepCopyFunc = &cvector_defaultDeepCopy;
    cvec.deepFreeFunc = &cvector_defaultDeepFree;
    return cvec;
}

bool reserveCVector(struct cvector*const cvec, const size_t numElements) {
    const size_t desiredSize = numElements * cvec->elementSize;
    if(desiredSize <= cvec->allocatedSize) return true; // if reserving less bytes than allocated, there is no need to do anything.
    const size_t newAllocatedSize = desiredSize*2; // allocate extra memory ahead of time to achieve O(1)+ time complexity for pushback operations
    void*const newData = (void*)cvec_realloc(cvec->data, newAllocatedSize);
    if(newData == NULL) return false; // error allocating data, reservation stays as-is
    cvec->allocatedSize = newAllocatedSize;
    cvec->data = newData;
    return true; // reservation success
}

bool resizeCVector(struct cvector*const cvec, const size_t numElements) {
    if(!reserveCVector(cvec, numElements)) return false; // if needed allocate extra data, pass error to caller as needed
    cvec->numElements = numElements;
    return true; // resize success
}

bool pushBackCVector(struct cvector*const cvec, const void*const newElement) {
    if(!resizeCVector(cvec, cvec->numElements + 1)) return false; // resize error
    const size_t newElementIndex = (cvec->numElements-1)*cvec->elementSize;
    unsigned char*const newElementLocation = &((unsigned char*)cvec->data)[newElementIndex];
    cvector_memcpy(newElementLocation, newElement, cvec->elementSize);
    return true; // pushback succcess
}

bool popBackCVector(struct cvector*const cvec, void*const poppedElement) {
    if(cvec->numElements<=0) return false; // can't pop elements off of empty vector
    const size_t popElementIndex = (cvec->numElements-1)*(cvec->elementSize);
    const void*const popElementLocation = (const void*)&(((const unsigned char*)(cvec->data))[popElementIndex]);
    cvector_memcpy(poppedElement, popElementLocation, cvec->elementSize);
    // may result in lower performance but it works
    return resizeCVector(cvec, cvec->numElements - 1); // pass errors to caller as needed
}

bool fitCVector(struct cvector*const cvec) {
    const size_t numBytes = cvector_getSizeOfCVectorinBytes(cvec);
    if(cvec->allocatedSize == numBytes) return true; // no need to fit the allocation to the data
    else if(cvec->allocatedSize < numBytes) return false; // error, more data than the allocation has size for
    // otherwise it is valid for fitting the allocation to the data
    const size_t desiredSize = cvec->numElements * cvec->elementSize;
    void*const newData = (void*)cvec_realloc(cvec->data, desiredSize);
    if(newData == NULL) return false; // error fitting data, reservation stays as-is. idk if this should return true to allow the program to continue properly
    cvec->allocatedSize = desiredSize;
    cvec->data = newData;
    return true; // reservation success
}

bool deepCopyCVector(struct cvector*const dest, const struct cvector*const src) {
    dest->elementSize = src->elementSize; // needs to come before vector resizing so resize allocates the appropriate amount of memory
    if(!resizeCVector(dest, src->numElements)) return false; // resize failed
    dest->deepCopyFunc = src->deepCopyFunc;
    for(size_t i=0; i<src->numElements; i++) {
        const size_t copyIndex = src->elementSize*i;
        void*const      destptr = &((unsigned char*)(dest->data))[copyIndex];
        const void*const srcptr = &((unsigned char*)( src->data))[copyIndex];
        if(!(src->deepCopyFunc(destptr, srcptr, src->elementSize))) return false; // error in copying
    }
    return true;
}

void freeCVector(struct cvector*const cvec) {
    cvec_free(cvec->data);
    cvec->data = NULL; // prevent double free
    cvec->allocatedSize = 0;
    cvec->numElements = 0;
}

bool deepFreeCVector(struct cvector*const cvec) {
    for(size_t i=0; i<cvec->numElements; i++) {
        const size_t freeIndex = cvec->elementSize*i;
        void* const dataptr = &((unsigned char*)(cvec->data))[freeIndex];
        if(!(cvec->deepFreeFunc(dataptr))) return false; // error in freeing
    }
    freeCVector(cvec); // free the vector
    return true;
}