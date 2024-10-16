#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/cppvector.h"

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
    class cppvector<char> cvec;
    for(size_t i=0; str[i]; i++) {
        if(cvec.size() != i) return false;
        cvec.pushBack(str[i]);
        if(cvec.size() != i+1) return false;
    }
    cvec.pushBack('\0');
    class cppvector<char> cvecCopy = cvec;
    if(cvecCopy!=cvec) return false;
    if(!(cvecCopy==cvec)) return false;
    if(cvec.size() != strlen(str)+1) return false;
    if(memcmp(&cvec[0], str, strlen(str)+1)) return false;
    cvec.fit();
    if(cvec.size() != cvec.allocationSize()) return false;
    if(cvec.popBack()!='\0') return false;
    for(size_t i=0; cvec.size() > 0; i++) {
        reversed[i] = cvec.popBack();
    }
    reversed[strlen(str)] = '\0';
    reverseString(reversed);
    if(memcmp(reversed, str, strlen(str)+1)) return false;
    return true;
}

bool testCase1() {
    const char*const str = "Hello, World! This is one heck of a long string for testing purposes. If it gets all of this correct then that would be great!";
    class cppvector<char> cvec;
    for(size_t i=0; str[i]; i++) {
        if(cvec.size() != i) return false;
        cvec.pushBack(str[i]);
        if(cvec.size() != i+1) return false;
    }
    class cppvector<char> dest = cvec;
    if(dest.size()!=cvec.size()) return false;
    if(&dest[0]==&cvec[0]) return false;
    if(memcmp(&dest[0], &cvec[0], sizeof(char)*cvec.size())) return false;
    cvec.fit();
    dest.fit();
    if(dest.size()!=cvec.size()) return false;
    if(dest.allocationSize()!=cvec.allocationSize()) return false;
    if(dest.allocationSize()!=dest.size()) return false;
    if(dest!=cvec) return false;
    if(!(dest==cvec)) return false;
    class cppvector<char> emptyVec;
    if((emptyVec+cvec) != dest) return false;
    emptyVec+=cvec;
    if(emptyVec!=cvec) return false;
    return true;
}

bool testCase2() {
    const char*const str = "Hello, World! This is one heck of a long string for testing purposes. If it gets all of this correct then that would be great!";
    class cppvector<char> cvec;
    for(size_t i=0; str[i]; i++) {
        if(cvec.size() != i) return false;
        cvec.pushBack(str[i]);
        if(cvec.size() != i+1) return false;
    }
    for(const char& c : cvec) {
        size_t index = (&c - cvec.begin());
        if(c != str[index]) return false;
    }
    return true;
}

bool testCase3() {
    const char*const str = "Hello, World! This is one heck of a long string for testing purposes. If it gets all of this correct then that would be great!";
    class cppvector<char> cvec;
    for(size_t i=0; str[i]; i++) {
        if(cvec.size() != i) return false;
        cvec.pushBack(str[i]);
        if(cvec.size() != i+1) return false;
    }
    class cppvector<char> firstHalf;
    class cppvector<char> secondHalf;
    for(size_t i=0; i<(cvec.size()/2); i++) {
        firstHalf.pushBack(cvec[i]);
    }
    for(size_t i=cvec.size()/2; i<cvec.size(); i++) {
        secondHalf.pushBack(cvec[i]);
    }
    class cppvector<char> combined = firstHalf + secondHalf;
    if(combined!=cvec) return false;
    return true;
}

int main(int argc, char* argv[]) {
    testCase testCases[] = {
        &testCase0,
        &testCase1,
        &testCase2,
        &testCase3,
    };
    size_t numTestCases = sizeof(testCases)/sizeof(testCase);
    for(size_t i=0; i<numTestCases; i++) {
        printf("test case #%d %s\n", (int)i, testCases[i]()?"Passed":"Failed");
    }
    return 0;
}