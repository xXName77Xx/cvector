#include "cvector.h"

// private functions
static size_t cvector_getSizeOfCVectorinBytes(struct cvector* cvec) {
    return cvec->numElements*cvec->elementSize;
}

// default functions
static bool cvector_defaultDeepCopy(void* dest, const void* src, size_t elemSize) {
    memcpy(dest, src, elemSize);
    return true;
}

static bool cvector_defaultDeepFree(void* ptr) {
    return true;
}

// public functions
struct cvector newCVector(size_t elementSize) {
    if(elementSize<=0) elementSize = 1;
    struct cvector cvec;
    cvec.data = (void*)malloc(0); // get valid allocation, does not check for NULL values as malloc sometimes returns those as a valid response for a zero byte allocation
    cvec.numElements = 0;
    cvec.elementSize = elementSize;
    cvec.allocatedSize = 0;
    cvec.deepCopyFunc = &cvector_defaultDeepCopy;
    cvec.deepFreeFunc = &cvector_defaultDeepFree;
    return cvec;
}

bool reserveCVector(struct cvector* cvec, size_t numElements) {
    size_t desiredSize = numElements * cvec->elementSize;
    if(desiredSize <= cvec->allocatedSize) return true; // if reserving less bytes than allocated, there is no need to do anything.
    size_t newAllocatedSize = desiredSize*2; // allocate extra memory ahead of time to achieve O(1)+ time complexity for pushback operations
    void* newData = (void*)realloc(cvec->data, newAllocatedSize);
    if(newData == NULL) return false; // error allocating data, reservation stays as-is
    cvec->allocatedSize = newAllocatedSize;
    cvec->data = newData;
    return true; // reservation success
}

bool resizeCVector(struct cvector* cvec, size_t numElements) {
    if(!reserveCVector(cvec, numElements)) return false; // if needed allocate extra data, pass error to caller as needed
    cvec->numElements = numElements;
    return true; // resize success
}

bool pushBackCVector(struct cvector* cvec, const void* newElement) {
    if(!resizeCVector(cvec, cvec->numElements + 1)) return false; // resize error
    size_t newElementIndex = (cvec->numElements-1)*cvec->elementSize;
    unsigned char* newElementLocation = &((unsigned char*)cvec->data)[newElementIndex];
    memcpy(newElementLocation, newElement, cvec->elementSize);
    return true; // pushback succcess
}

bool popBackCVector(struct cvector* cvec, void* poppedElement) {
    if(cvec->numElements<=0) return false; // can't pop elements off of empty vector
    size_t popElementIndex = (cvec->numElements-1)*(cvec->elementSize);
    const void* popElementLocation = (const void*)&(((const unsigned char*)(cvec->data))[popElementIndex]);
    memcpy(poppedElement, popElementLocation, cvec->elementSize);
    cvec->numElements--;
    return true;
    // resize call added so later iterations can free unused memory if the program is tight on memory
    //return resizeCVector(cvec, cvec->numElements - 1); // pass errors to caller as needed
}

bool fitCVector(struct cvector* cvec) {
    size_t numBytes = cvector_getSizeOfCVectorinBytes(cvec);
    if(cvec->allocatedSize == numBytes) return true; // no need to fit the allocation to the data
    if(cvec->allocatedSize < numBytes) return false; // error, more data than the allocation has size for
    // otherwise it is valid for fitting the allocation to the data
    size_t desiredSize = cvec->numElements * cvec->elementSize;
    void* newData = (void*)realloc(cvec->data, desiredSize);
    if(newData == NULL) return false; // error fitting data, reservation stays as-is. idk if this should return true to allow the program to continue properly
    cvec->allocatedSize = desiredSize;
    cvec->data = newData;
    return true; // reservation success
}

bool deepCopyCVector(struct cvector* dest, const struct cvector* src) {
    dest->elementSize = src->elementSize; // needs to come before vector resizing so resize allocates the appropriate amount of memory
    if(!resizeCVector(dest, src->numElements)) return false; // resize failed
    dest->deepCopyFunc = src->deepCopyFunc;
    for(int i=0; i<src->numElements; i++) {
        size_t copyIndex = src->elementSize*i;
        void* destptr =      &((unsigned char*)(dest->data))[copyIndex];
        const void* srcptr = &((unsigned char*)( src->data))[copyIndex];
        if(!(src->deepCopyFunc(destptr, srcptr, src->elementSize))) return false; // error in copying
    }
    return true;
}

void freeCVector(struct cvector* cvec) {
    free(cvec->data);
    cvec->data = NULL; // prevent double free
    cvec->allocatedSize = 0;
    cvec->numElements = 0;
}

bool deepFreeCVector(struct cvector* cvec) {
    for(int i=0; i<cvec->numElements; i++) {
        size_t freeIndex = cvec->elementSize*i;
        void* dataptr = &((unsigned char*)(cvec->data))[freeIndex];
        if(!(cvec->deepFreeFunc(dataptr))) return false; // error in freeing
    }
    freeCVector(cvec);
    return true;
}