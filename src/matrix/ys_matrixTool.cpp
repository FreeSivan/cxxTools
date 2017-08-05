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

    DMatrix<int> matrix2(3, 4);
    matrix1[0][0] = 8;
        printf ("1\n");
    matrix1[0][1] = 9;
    printf ("2\n");
    matrix1[1][0] = 1;
    printf ("3\n");
    matrix1[1][1] = 6;
    printf ("4\n");
    matrix1[2][0] = 3;
    printf ("5\n");
    matrix1[2][1] = 8;
    printf ("6\n");

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
        printf ("%d ", matrix3[i][j]);
    }
    printf ("\n");
    }

    delete matrix3;
    return 0;
}
