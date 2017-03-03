#include "ys_phash.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
namespace ys {

phash::phash() {
    first = new unsigned short[F_LENGTH];
    second = new unsigned int[S_LENGTH];
    third = new unsigned long[T_LENGTH];
    foffset = soffset = toffset = lastKey = 0;
}

phash::~phash() {
    delete[] first;
    delete[] second;
    delete[] third;
}

long phash::getpHash(unsigned int key) {
    int i1 = key >> 21;
    int i2 = key >> 6;
    int i3 = key;
    return first[i3]+second[i2]+third[i1];
}

void phash::addphKey(unsigned int key, int length) {
    if (lastKey+1 < key) {
        unsigned int i = lastKey+1;
        for (; i < key; ++i) {
            generate(i);
        }    
    }
    generate(key);
    foffset += length;
    lastKey = key;
}

void phash::addOver() {
    for (int i = lastKey+1; i < F_LENGTH; ++i) {
        addphKey(i, 0);
    }
    for (int i = 0; i < T_LENGTH; ++i) {
        printf ("%ld\n", third[i]);
    }
}

void phash::generate(unsigned int key) {
    if (lastKey >> 21 != key >> 21) {
        toffset += soffset;
        third[key>>21] = toffset;
        soffset = 0;
    }
    if (lastKey >> 6 != key >> 6) {
        soffset += foffset;
        second[key>>6] = soffset;
        foffset = 0;
    }
    first[key] = foffset;
}

}

using namespace ys;

int main() {
    phash hashtable;
    unsigned long length = (unsigned int)1 << 31;
    for (unsigned long i = 0; i < length; i+=4) {
        hashtable.addphKey(i, 50);
    }
    hashtable.addOver();
    for (unsigned long i = 0; i < length; i+=4) {
        long offset = hashtable.getpHash(i);
        assert (offset == (i/4)*50);
    }
    return 0;
}
