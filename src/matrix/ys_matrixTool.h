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

/*******************************************************************************
 *
 *  Summary: 计算矩阵A和矩阵B的加和，矩阵A和矩阵B必须行数列数相同
 *
 *  Parameters:
 *
 *      const Matrix<T>& A : 矩阵A    
 *
 *      const Matrix<T>& B : 矩阵B
 *
 *  Return : 返回指向结果矩阵的指针，该指针由本函数申请，但必须有调用函数释放 
 *
 *******************************************************************************/
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

/*******************************************************************************
 *
 *  Summary: 计算矩阵A和矩阵B的加和，通过传出参数C传出
 *
 *  Parameters:
 *
 *      const Matrix<T>& A : 矩阵A   
 *
 *      const Matrix<T>& B : 矩阵B
 *
 *      DMatrix<T>& C : 矩阵C，传出A+B的结果
 *
 *  Return : 没有返回值，通过传出参数C传出
 *
 *******************************************************************************/
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

/*******************************************************************************
 *
 *  Summary: 计算矩阵A和矩阵B的乘机，矩阵A的列数必须等于矩阵B的行数
 *
 *  Parameters:
 *
 *      const Matrix<T>& A : 矩阵A   
 *
 *      const Matrix<T>& B : 矩阵B
 *
 *  Return : 返回指向结果矩阵的指针，该指针由本函数申请，但必须有调用函数释放
 *
 *******************************************************************************/
template <typename T>
DMatrix<T>* Matrixs<T>::mul(const DMatrix<T>& A, const DMatrix<T>& B) {
    if (A.getDimY() != B.getDimX()) {
        return 0;
    }
    DMatrix<T>* result = new DMatrix<T>(A.getDimX(), B.getDimY());
    for (int i = 0; i < A.getDimX(); ++i) {
        for (int j = 0; j < B.getDimY(); ++j) {
            (*result)[i][j] = 0;
            for (int k = 0; k < A.getDimY(); ++k) {
                (*result)[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

/*******************************************************************************
 *
 *  Summary: 矩阵A和矩阵B相乘，结果通过传出参数矩阵C传出
 *
 *  Parameters:
 *
 *      const Matrix<T>& A : 矩阵A    
 *
 *      const Matrix<T>& B : 矩阵B
 *
 *      DMatrix<T>& C : 传出参数矩阵C
 *
 *  Return : 没有返回值，通过传出参数C传出
 *
 *******************************************************************************/
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

/*******************************************************************************
 *
 *Summary: 计算矩阵A和矩阵B的差值，矩阵A和矩阵B必须行数列数相同
 *
 *Parameters:
 *
 *   const Matrix<T>& A : 矩阵A    
 *
 *   const Matrix<T>& B : 矩阵B
 *
 *Return : 返回指向结果矩阵的指针，该指针由本函数申请，但必须有调用函数释放
 *
 *******************************************************************************/
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

/*******************************************************************************
 *
 *Summary: 计算矩阵A和矩阵B的差值，矩阵A和矩阵B必须行数列数相同
 *
 *Parameters:
 *
 *   const Matrix<T>& A : 矩阵A   
 *
 *   const Matrix<T>& B : 矩阵B
 *
 *   DMatrix<T>& C : 传出参数矩阵C
 *
 *Return : 没有返回值，通过传出参数C传出
 *
 *******************************************************************************/
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

/*******************************************************************************
 *
 *Summary: 计算矩阵A的r行和矩阵B的c列的乘积
 *
 *Parameters:
 *
 *   const Matrix<T>& A : 矩阵A   
 *
 *   const Matrix<T>& B : 矩阵B
 *
 *Return : 矩阵A的r行和矩阵B的c列的乘积值
 *
 *******************************************************************************/
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

/*******************************************************************************
 *
 *Summary: 计算矩阵A和B的均方差
 *
 *Parameters:
 *
 *   const Matrix<T>& A : 矩阵A   
 *
 *   const Matrix<T>& B : 矩阵B
 *
 *Return : 均方差的值
 *
 *******************************************************************************/
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
