#ifndef _YS_MATRIX_TOOL_H_
#define _YS_MATRIX_TOOL_H_

#include "ys_matrix.h"

namespace ys {      // namespace for yue sivan

template <typename T> class Matrixs {
public:
    static DMatrix<T>* add(const DMatrix<T>& A, const DMatrix<T>& B);
    static DMatrix<T>* mul(const DMatrix<T>& A, const DMatrix<T>& B);
    static DMatrix<T>* sub(const DMatrix<T>& A, const DMatrix<T>& B);
    static T mulLine(const DMatrix<T>& A, const DMatrix<T>& B, int r, int c);
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
inline DMatrix<T>* Matrixs<T>::add(const DMatrix<T>& A, const DMatrix<T>& B) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        return 0;
    }
    int dimX = A.getDimX();
    int dimY = A.getDimY();
    DMatrix<T>* result = new DMatrix<T>(dimX, dimY);
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        (*result)[i][j] = A[i][j] + B[i][j];
    }
    }
    return result;
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
inline DMatrix<T>* Matrixs<T>::mul(const DMatrix<T>& A, const DMatrix<T>& B) {
    if (A.getDimY() != B.getDimX()) {
        return 0;
    }
    int dimX = A.getDimX();
    int dimY = B.getDimY();
    int dimK = A.getDimY();
    DMatrix<T>* result = new DMatrix<T>(dimX, dimY);
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        T sum = 0;
        for (int k = 0; k < dimK; ++k) {
            sum += A[i][k] * B[k][j];
        }
        (*result)[i][j] = sum;
    }
    }
    return result;
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
inline DMatrix<T>* Matrixs<T>::sub(const DMatrix<T>& A, const DMatrix<T>& B) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        return 0;
    }
    int dimX = A.getDimX();
    int dimY = A.getDimY();
    DMatrix<T>* result = new DMatrix<T>(dimX, dimY);
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        (*result)[i][j] = A[i][j] - B[i][j];
    }
    }
    return result;
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
inline T Matrixs<T>::mulLine(const DMatrix<T>& A, const DMatrix<T>& B, int r, int c) {
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

};

#endif //_YS_MATRIX_TOOL_H_