#include "phash.h"

namespace ys {

phash::phash() {
    first = new unsigned short[F_LENGTH];
    second = new unsigned int[S_LENGTH];
    third = new unsigned long[T_LENGTH];
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



}

