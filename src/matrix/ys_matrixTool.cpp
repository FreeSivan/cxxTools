#include <stdio.h>
#include "ys_matrixTool.h"
#include "ys_matrix.h"

using namespace ys;

int main(void) {
    DMatrix<int> matrix1(2, 3);

    matrix1[0][0] = 7;
    matrix1[0][1] = 6;
    matrix1[0][2] = 2;
    matrix1[1][0] = 3;
    matrix1[1][1] = 2;
    matrix1[1][2] = 5;

    for (int i = 0; i < matrix1.getDimX(); ++i) {
    for (int j = 0; j < matrix1.getDimY(); ++j) {
        printf ("%d ", matrix1[i][j]);
    }
    printf ("\n");
    }
    printf ("----------------------------\n");

    DMatrix<int> matrix2(3, 2);
    matrix2[0][0] = 8;
    matrix2[0][1] = 9;
    matrix2[1][0] = 1;
    matrix2[1][1] = 6;
    matrix2[2][0] = 3;
    matrix2[2][1] = 8;

    for (int i = 0; i < matrix2.getDimX(); ++i) {
    for (int j = 0; j < matrix2.getDimY(); ++j) {
        printf ("%d ", matrix2[i][j]);
    }
    printf ("\n");
    }
    printf ("----------------------------\n");

    DMatrix<int>* matrix3 = Matrixs<int>::mul(matrix1, matrix2);

    for (int i = 0; i < matrix3->getDimX(); ++i) {
    for (int j = 0; j < matrix3->getDimY(); ++j) {
        printf ("%d ", (*matrix3)[i][j]);
    }
    printf ("\n");
    }

    delete matrix3;
    return 0;
}
