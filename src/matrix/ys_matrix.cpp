#include <cmath>
#include <stdlib.h>

#include "ys_matrix.h"

double ran_uniform() {
    return rand()/((double)RAND_MAX + 1);
}

using namespace ys;

int main(void) {
    DMatrix<double> dmatrix(100, 1000);
    for (int i = 0; i < dmatrix.getDimX(); ++i) {
    for (int j = 0; j < dmatrix.getDimY(); ++j) {
        dmatrix(i, j) = ran_uniform();
    }
    }
    dmatrix.save("tmpfile");
    return 0;
}
