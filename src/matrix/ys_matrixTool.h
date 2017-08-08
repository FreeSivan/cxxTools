#ifndef _YS_MATRIX_TOOL_H_
#define _YS_MATRIX_TOOL_H_

#include <math.h>
#include <stdio.h>
#include "ys_matrix.h"

namespace ys {      // namespace for yue sivan

template <typename T> class Matrixs {
public:
    static DMatrix<T>* add(const DMatrix<T>& A, const DMatrix<T>& B);
    static DMatrix<T>* mul(const DMatrix<T>& A, const DMatrix<T>& B);
    static DMatrix<T>* sub(const DMatrix<T>& A, const DMatrix<T>& B);
    static void add(const DMatrix<T>& A, const DMatrix<T>& B, DMatrix<T>& C);
    static void mul(const DMatrix<T>& A, const DMatrix<T>& B, DMatrix<T>& C);
    static void sub(const DMatrix<T>& A, const DMatrix<T>& B, DMatrix<T>& C);
    static T mulLine(const DMatrix<T>& A, const DMatrix<T>& B, int r, int c);
    static T RMSE(const DMatrix<T>& A, const DMatrix<T>& B);
};

/*
 *Summary: sum matrix A and matrix B
 *
 *Parameters:
 *
 *   const Matrix<T>& A : matrix A    
 *
 *   const Matrix<T>& B : matrix B
 *
 *Return : The pointer of the result matrix, a new object is allocated in 
 *
 *      this function, it must be release by caller!!!!!
 *
 */
template <typename T>
DMatrix<T>* Matrixs<T>::add(const DMatrix<T>& A, const DMatrix<T>& B) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        return 0;
    }
    DMatrix<T>* result = new DMatrix<T>(A.getDimY(), A.getDimX());
    for (int i = 0; i < A.getDimX(); ++i) {
        for (int j = 0; j < A.getDimY(); ++j) {
            (*result)[i][j] = A[i][j] + B[i][j];
        }
    }
    return result;
}
template <typename T>
void Matrixs<T>::add(const DMatrix<T>& A, const DMatrix<T>& B, DMatrix<T>& C) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        return 0;
    }
    C.reset(A.getDimX(), A.getDimY());
    for (int i = 0; i < A.getDimX(); ++i) {
        for (int j = 0; j < A.getDimY(); ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

/*
 *Summary: multiply matrix A and matrix B
 *
 *Parameters:
 *
 *   const Matrix<T>& A : matrix A    
 *
 *   const Matrix<T>& B : matrix B
 *
 *Return : The pointer of the result matrix, a new object is allocated in 

 *      this function, it must be release by caller!!!!!
 */
template <typename T>
DMatrix<T>* Matrixs<T>::mul(const DMatrix<T>& A, const DMatrix<T>& B) {
    if (A.getDimY() != B.getDimX()) {
        return 0;
    }
    DMatrix<T>* result = new DMatrix<T>(dimX, dimY);
    for (int i = 0; i < A.getDimY(); ++i) {
        for (int j = 0; j < B.getDimX(); ++j) {
            (*result)[i][j] = 0;
            for (int k = 0; k < A.getDimX(); ++k) {
                (*result)[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}
template <typename T>
void Matrixs<T>::mul(const DMatrix<T>& A, const DMatrix<T>& B, DMatrix<T>& C) {
    if (A.getDimY() != B.getDimX()) {
        return;
    }
    C.reset(A.getDimX(), B.getDimY());
    for (int i = 0; i < A.getDimX(); ++i) {
        for (int j = 0; j < B.getDimY(); ++j) {
            C[i][j] = 0;
            for (int k = 0; k < A.getDimY(); ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/*
 *Summary: matrix A minus matrix B
 *
 *Parameters:
 *
 *   const Matrix<T>& A : matrix A    
 *
 *   const Matrix<T>& B : matrix B
 *
 *Return : The pointer of the result matrix, a new object is allocated in 

 *      this function, it must be release by caller!!!!!
 */
template <typename T>
DMatrix<T>* Matrixs<T>::sub(const DMatrix<T>& A, const DMatrix<T>& B) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        return 0;
    }
    DMatrix<T>* result = new DMatrix<T>(A.getDimX(), B.getDimY());
    for (int i = 0; i < A.getDimX(); ++i) {
        for (int j = 0; j < A.getDimY(); ++j) {
            (*result)[i][j] = A[i][j] - B[i][j];
        }
    }
    return result;
}
template <typename T>
void Matrixs<T>::sub(const DMatrix<T>& A, const DMatrix<T>& B, DMatrix<T>& C) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        return 0;
    }
    C.reset(A.getDimY(), A.getDimY());
    for (int i = 0; i < A.getDimX(); ++i) {
        for (int j = 0; j < A.getDimY(); ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

/*
 *Summary: 计算矩阵A的r行和矩阵B的c列的乘积
 *
 *Parameters:
 *
 *   const Matrix<T>& A : 矩阵A   
 *
 *   const Matrix<T>& B : 矩阵B
 *
 *Return : 矩阵A的r行和矩阵B的c列的乘积值
 */
template <typename T>
T Matrixs<T>::mulLine(const DMatrix<T>& A, const DMatrix<T>& B, int r, int c) {
    if (A.getDimY() != B.getDimX()) {
        throw r;
    }
    int dimK = A.getDimY();
    T result = 0;
    for (int i = 0; i < dimK; ++i) {
        result += A[r][i] * B[i][c];
    }
    return result;
}

/*
 *Summary: 计算矩阵A和B的均方差
 *
 *Parameters:
 *
 *   const Matrix<T>& A : 矩阵A   
 *
 *   const Matrix<T>& B : 矩阵B
 *
 *Return : 均方差的值
 */
template <typename T>
T Matrixs<T>::RMSE(const DMatrix<T>& A, const DMatrix<T>& B) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        return (T)-1;
    }
    int dimX = A.getDimX();
    int dimY = B.getDimY();
    T sum = 0;
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        sum += pow((A[i][j]-B[i][j]), 2);
    }
    }
    return sqrt(sum / (dimX*dimY));
}

};

#endif //_YS_MATRIX_TOOL_H_
