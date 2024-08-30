#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/cvector.h"
typedef bool (*testCase)(void); 

void reverseString(char*const str) {
    for(size_t start = 0, end=strlen(str)-1; start < end; start++, end--) {
        const char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
    }
}

bool testCase0() {
    const char*const str = "Hello, World! This is one heck of a long string for testing purposes. If it gets all of this correct then that would be great!";
    char reversed[1024];
    char c = '.';
    struct cvector cvec = newCVector(sizeof(char));
    for(size_t i=0; str[i]; i++) {
        if(cvec.numElements != i) goto fail;
        if(!pushBackCVector(&cvec, &str[i])) goto fail;
        if(cvec.numElements != i+1) goto fail;
    }
    if(!pushBackCVector(&cvec, "")) goto fail;
    if(cvec.numElements != strlen(str)+1) goto fail;
    if(memcmp(cvec.data, str, strlen(str)+1)) goto fail;
    if(!fitCVector(&cvec)) goto fail;
    if(cvec.elementSize*cvec.numElements != cvec.allocatedSize) goto fail;
    if(!popBackCVector(&cvec, &c)) goto fail;
    if(c!='\0') goto fail;
    for(size_t i=0; cvec.numElements > 0; i++) {
        if(!popBackCVector(&cvec, &reversed[i])) goto fail;
    }
    reversed[strlen(str)] = '\0';
    reverseString(reversed);
    if(memcmp(reversed, str, strlen(str)+1)) goto fail;
    deepFreeCVector(&cvec);
    return true;
fail:
    deepFreeCVector(&cvec);
    return false;
}

bool testCase1() {
    const char*const str = "Hello, World! This is one heck of a long string for testing purposes. If it gets all of this correct then that would be great!";
    struct cvector cvec = newCVector(sizeof(char));
    struct cvector dest = newCVector(sizeof(char));
    for(size_t i=0; str[i]; i++) {
        if(cvec.numElements != i) goto fail;
        if(!pushBackCVector(&cvec, &str[i])) goto fail;
        if(cvec.numElements != i+1) goto fail;
    }
    if(!deepCopyCVector(&dest, &cvec)) goto fail;
    if(dest.numElements!=cvec.numElements) goto fail;
    if(dest.elementSize!=cvec.elementSize) goto fail;
    if(dest.deepCopyFunc!=cvec.deepCopyFunc) goto fail;
    if(dest.data==cvec.data) goto fail;
    if(memcmp(dest.data, cvec.data, cvec.elementSize*cvec.numElements)) goto fail;
    if(!fitCVector(&cvec)) goto fail;
    if(!fitCVector(&dest)) goto fail;
    if(dest.numElements!=cvec.numElements) goto fail;
    if(dest.elementSize!=cvec.elementSize) goto fail;
    if(dest.deepCopyFunc!=cvec.deepCopyFunc) goto fail;
    if(dest.allocatedSize!=cvec.allocatedSize) goto fail;
    deepFreeCVector(&cvec);
    deepFreeCVector(&dest);
    return true;
fail:
    freeCVector(&cvec);
    freeCVector(&dest);
    return false;
}

int main(int argc, char* argv[]) {
    testCase testCases[] = {
        &testCase0,
        &testCase1,
    };
    size_t numTestCases = sizeof(testCases)/sizeof(testCase);
    for(size_t i=0; i<numTestCases; i++) {
        printf("test case #%d %s\n", (int)i, testCases[i]()?"Passed":"Failed");
    }
    return 0;
}