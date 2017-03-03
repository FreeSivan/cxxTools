#include "phash.h"

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
    return first[i1]+second[i2]+third[i3];
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
    int key = lastKey + 1;
    int i1 = key >> 21;
    int i2 = key >> 6;
    int i3 = key;
    memset(first+i1, 0, sizeof(int)*(F_LENGTH-i1));
    memset(second+i2, 0, sizeof(int)*(S_LENGTH-i2));
    memset(third+i3, 0, sizeof(int)*(F_LENGTH-i3));
}

void phash::generate(unsigned int key) {
    if (lastKey >> 21 != key >> 21) {
        third[key>>21] += soffset;
        soffset = 0;
    }
    if (lastKey >> 6 != key >> 6) {
        second[k>>6] += foffset;
        foffset = 0;
    }
    first[key] = foffset;
}

}

