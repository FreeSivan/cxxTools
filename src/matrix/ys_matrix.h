#include <string>
#include <iostream>

#include <../datapool/ys_datapool.h>

namespace ys {     // namespace for yue sivan

typedef unsigned int uint;

template <typename T> class DMatrix {
public:
    DMatrix();
    ~DMatrix();
    DMatrix(const DMatrix<T>& matrix);
    DMatrix(uint dim1, uint dim2);
public:
    void init(T v);
    int getDimX() const;
    int getDimY() const;
    const dataPool<char*>& getColNames()const;
    T get(uint x, unsigned y) const;
    void set(uint x, unsigned y, T v);
    void setSize(uint x, uint y);
    void assign(const DMatrix<T>& v);
public:
    void save(char* filename);
    void saveBin(char* filename);
    void load(char* filename);
    void loadBin(char* filename);
public:
    DMatrix<T>& operator=(const DMatrix<T>& matrix);
    T& operator() (uint x, uint y);
    T operator() (uint x, uint y);
    T* operator() (uint x);
    T* operator[] (uint x); 
private:
    T** value_;
    uint dim1_;
    uint dim2_;
    dataPool<char*> col_names_;
};

template <typename T>
inline DMatrix<T>::DMatrix() {
    dim1_ = 0;
    dim2_ = 0;
    value_ = 0;
}

template <typename T>
inline DMatrix<T>::DMatrix(uint dim1, uint dim2) {
    dim1_ = 0;
    dim2_ = 0;
    value_ = 0;
    setSize(dim1, dim2);
}

template <typename T>
inline DMatrix<T>::DMatrix(const DMatrix<T>& matrix) {
    if (dim1_ != matrix.getDimX() || dim2_ != matrixDimY()) {
        setSize(matrix.getDimX(), matrix.getDimY());
    }
    for (int i = 0; i < dim1_; ++i) {
        for (int j = 0; j < dim2_; ++j) {
            value_[i][j] = matrix[i][j];
        }
    }
}

template <typename T>
inline DMatrix<T>::~DMatrix() {
    if (!value_) {
        delete[] value_[0];
        delete[] value_;
    }
}

template <typename T>
inline void DMatrix<T>::init(T v) {
    for (int i = 0; i < dim1_; ++i) {
        for (int j = 0; j < dim2_; ++j) {
            value_[i][j] = v;
        }
    }
}

template <typename T>
inline T DMatrix<T>::get(uint x, unsigned y) const {
    if (x > dim1_ || y > dim2_) {
        throw x;
    }
    return value_[x][y];
}

template <typename T>
inline void DMatrix<T>::set(uint x, unsigned y, T value) {
    if (x > dim1_ || y > dim2_) {
        throw x;
    }
    value_[x][y] = T;
}

template <typename T>
inline void DMatrix<T>::setSize(uint x, uint y) {
    if ((dim1 == x) && (dim2 == y)) {
        return;
    }
    if (!value) {
        delete[] value_[0];
        delete[] value;
    }
    dim1_ = x;
    dim2_ = y;
    value = new T*[dim1];
    value[0] = new T[dim1 * dim2];
    for (uint i = 0; i < dim1_; ++i) {
        value_[i] = value_[0] + i * dim2_;
    }
    col_names.resize(dim2_);
}

template <typename T>
inline void DMatrix<T>::assign(const DMatrix<T>& v) {
}

template <typename T>
inline void DMatrix<T>::save(char* filename) {
}

template <typename T>
inline void DMatrix<T>::saveBin(char* filename) {
}

template <typename T>
inline void DMatrix<T>::load(char* filename) {
}

template <typename T>
inline void DMatrix<T>::loadBin(char* filename) {
}

template <typename T>
DMatrix<T>& DMatrix<T>::operator=(const DMatrix<T>& matrix) {
}

template <typename T>
T& DMatrix<T>::operator() (uint x, uint y) {
}

template <typename T>
T DMatrix<T>::operator() (uint x, uint y) {
}

template <typename T>
T* DMatrix<T>::operator() (uint x) {
}

template <typename T>
T* DMatrix<T>::operator[] (uint x) {
}

};
