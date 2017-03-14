#include <stdio.h>
#include "ys_datapool.h"

using namespace ys;

int main() {
    dataPool<unsigned int> intpool(2, 3);
    //intpool.push_back(1);
    //intpool.push_back(2);
    //intpool.push_back(3);
    //intpool.push_back(4);
    //intpool.push_back(5);
    //intpool.push_back(6);
    //intpool.push_back(7);

    unsigned int arr[] = {5, 322, 445,23,664,23576,443,84,37, 1};

    intpool.push_back(arr, sizeof(arr)/sizeof(int));

    for (int i = 0; i < intpool.getCurLen(); ++i) {
        printf ("%u\n", intpool[i]);
    }
    printf ("===========================\n");
    intpool.sort();

    for (int i = 0; i < intpool.getCurLen(); ++i) {
        printf ("%u\n", intpool[i]);
    }
    printf ("===========================\n");
    intpool.reset();
    for (int i = 0; i < intpool.getCurLen(); ++i) {
        printf ("%u\n", intpool[i]);
    }

    //for (int i = 0; i < 10000000; ++i) {
    //    intpool.push_back(i);
    //}
    //printf ("size = %d\n", intpool.getCurLen());

    //for (int i = 0; i < intpool.getCurLen(); ++i) {
    //    printf ("%u\n", intpool[i]);
    //}

    return 0;
}

