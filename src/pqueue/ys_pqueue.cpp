#include "ys_pqueue.h"

using namespace ys;

int main() {
    maxHeap<int> mheap;
    mheap.push(31);
    mheap.push(8);
    mheap.push(5);
    mheap.push(9);
    mheap.push(13);
    mheap.push(2);
    mheap.push(345);
    mheap.push(34);
    mheap.push(56);
    while (mheap.getCurlength()) {
        int tmp;
        mheap.pop(tmp);
        printf ("%d\n", tmp);
    }

    return 0;
}
