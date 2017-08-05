#ifndef _YS_MATRIX_TOOL_H_
#define _YS_MATRIX_TOOL_H_

#include "ys_matrix.h"

namespace ys {      // namespace for yue sivan

template <typename T> class Matrixs {
public:
    Matrix<T>* add(const Matrix<T>& A, const Matrix<T>& B);
    Matrix<T>* mul(const Matrix<T>& A, const Matrix<T>& B);
    Matrix<T>* sub(const Matrix<T>& A, const Matrix<T>& B);
    T mulLine(const Matrix<T>& A, const Matrix<T>& B, int r, int c);
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
inline Matrix<T>* Matrixs<T>::add(const Matrix<T>& A, const Matrix<T>& B) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        goto Error;
    }
    int dimX = A.getDimX();
    int dimY = A.getDimY();
    Matrix<T>* result = new Matrix<T>(dimX, dimY);
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        (*result)[i][j] = A[i][j] + B[i][j];
    }
    }
Finish:
    return result;
Error:
    return 0;
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
inline Matrix<T>* Matrixs<T>::mul(const Matrix<T>& A, const Matrix<T>& B) {
    if (A.getDimY() != B.getDimX()) {
        goto Error;
    }
    int dimX = A.getDimX();
    int dimY = B.getDimY();
    int dimK = A.getDimY();
    Matrix<T>* result = new Matrix<T>(dimX, dimY);
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        int sum = 0;
        for (int k = 0; k < dimK; ++k) {
            sum += A[i][k] * B[k][j];
        }
        result[i][j] = sum;
    }
    }
Finish:
    return result;
Error:
    return 0;
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
inline Matrix<T>* Matrixs<T>::sub(const Matrix<T>& A, const Matrix<T>& B) {
    if (A.getDimX() != B.getDimX() || A.getDimY() != B.getDimY()) {
        goto Error;
    }
    int dimX = A.getDimX();
    int dimY = A.getDimY();
    Matrix<T>* result = new Matrix<T>(dimX, dimY);
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        (*result)[i][j] = A[i][j] - B[i][j];
    }
    }
Finish:
    return result;
Error:
    return 0;
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
inline T Matrixs<T>::mulLine(const Matrix<T>& A, const Matrix<T>& B, int r, int c) {
    if (A.getDimY() != B.getDimX()) {
        goto Error;
    }
    int dimK = A.getDimY();
    int result = 0;
    for (int i = 0; i < dimK; ++i) {
        result += A[r][i] * B[i][c];
    }
Finish:
    return result;
Error:
    throw r;
}

};

#endif //_YS_MATRIX_TOOL_H_