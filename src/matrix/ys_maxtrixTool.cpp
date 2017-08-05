#include <stdio.h>
#include "ys_matrixTool.h"
#include "ys_matrix.h"

using namespace ys;

int main(void) {
    Matrix<int> matrix1;
    matrix1[0][0] = 7;
    matrix1[0][1] = 6;
    matrix1[0][2] = 2;
    matrix1[1][0] = 3;
    matrix1[1][1] = 2;
    matrix1[1][2] = 5;

    Matrix<int> matrix2;
    matrix1[0][0] = 8;
    matrix1[0][1] = 9;
    matrix1[1][0] = 1;
    matrix1[1][1] = 6;
    matrix1[2][0] = 3;
    matrix1[2][1] = 8;

    Matrix<int>* matrix3 = Matrixs::mul(matrix1, matrix2);

    for (int i = 0; i < matrix3.getDimX(); ++i) {
    for (int j = 0; j < matrix3.getDimY(); ++j) {
        printf ("%d ", matrix3[i][j]);
    }
    printf ("\n");
    }
    return 0;
}
