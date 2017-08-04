#include <stdio.h>

namespace ys {     // namespace for yue sivan

typedef unsigned int uint;

template <typename T> class DMatrix {
public:
    DMatrix();
    ~DMatrix();
    DMatrix(uint x, uint y);
    DMatrix(const DMatrix<T>& v);
public:
    void init(T v);
    int getDimX() const;
    int getDimY() const;
    T get(uint x, uint y) const;
    bool set(uint x, uint y, T v);
    void setSize(uint x, uint y);
    void assign(const DMatrix<T>& v);
public:
    bool save(char* filename);
    bool load(char* filename);
public:
    DMatrix<T>& operator=(const DMatrix<T>& v);
    T& operator() (uint x, uint y);
    T  operator() (uint x, uint y) const;
    T* operator() (uint x);
    T* operator[] (uint x); 
private:
    T** value_;
    uint dimX_;
    uint dimY_;
};

template <typename T>
inline DMatrix<T>::DMatrix() {
    dimX_ = 0;
    dimY_ = 0;
    value_ = 0;
}

template <typename T>
inline DMatrix<T>::DMatrix(uint x, uint y) {
    dimX_ = 0;
    dimY_ = 0;
    value_ = 0;
    setSize(x, y);
}

template <typename T>
inline DMatrix<T>::DMatrix(const DMatrix<T>& v) {
    if (dimX_ != v.getDimX() || dimY_ != v.DimY()) {
        setSize(v.getDimX(), v.getDimY());
    }
    for (int i = 0; i < dimX_; ++i) {
    for (int j = 0; j < dimY_; ++j) {
        value_[i][j] = v[i][j];
    }
    }
}

template <typename T>
inline DMatrix<T>::~DMatrix() {
    if (value_) {
        delete[] value_[0];
        delete[] value_;
    }
}

template <typename T>
inline void DMatrix<T>::init(T v) {
    for (int i = 0; i < dimX_; ++i) {
    for (int j = 0; j < dimY_; ++j) {
        value_[i][j] = v;
    }
    }
}

template <typename T>
inline int DMatrix<T>::getDimX() const {
    return dimX_;
}

template <typename T>
inline int DMatrix<T>::getDimY() const {
    return dimY_;
}

template <typename T>
inline T DMatrix<T>::get(uint x, uint y) const {
    if (x > dimX_ || y > dimY_) {
        goto Error;
    }
    return value_[x][y];
Error:
    throw x;
}

template <typename T>
inline bool DMatrix<T>::set(uint x, uint y, T v) {
    if (x > dimX_ || y > dimY_) {
        goto Error;
    }
    value_[x][y] = v;
Finish:
    return 1;
Error:
    return 0;
}

template <typename T>
inline void DMatrix<T>::setSize(uint x, uint y) {
    if ((dimX_ == x) && (dimY_ == y)) {
        return;
    }
    dimX_ = x;
    dimY_ = y;
    if (value_) {
        delete[] value_[0];
        delete[] value_;
    }
    value_ = new T*[dimX_];
    value_[0] = new T[dimX_ * dimY_];
    for (uint i = 0; i < dimX_; ++i) {
        value_[i] = value_[0] + i * dimY_;
    }
}

template <typename T>
inline void DMatrix<T>::assign(const DMatrix<T>& v) {
    if (v.getDimX() != dimX_ || v.getDimY() != dimY_) {
        setSize(v.getDimX(), v.getDimY());
    }
    for (int i = 0; i < dimX_; ++i) {
    for (int j = 0; j < dimY_; ++j) {
        value_[i][j] = v[i][j];
    }
    }
}

template <typename T>
inline DMatrix<T>& DMatrix<T>::operator=(const DMatrix<T>& v) {
    assign(v);
    return *this;
}

template <typename T>
inline T& DMatrix<T>::operator() (uint x, uint y) {
    return value_[x][y];
}

template <typename T>
inline T  DMatrix<T>::operator() (uint x, uint y) const{
    return value_[x][y];
}

template <typename T>
inline T* DMatrix<T>::operator() (uint x) {
    return value_[x];
}

template <typename T>
inline T* DMatrix<T>::operator[] (uint x) {
    return value_[x];
}

template <typename T>
inline bool DMatrix<T>::save(char* filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        goto Error;
    }
    fwrite(&dimX_, sizeof(uint), 1, fp);
    fwrite(&dimY_, sizeof(uint), 1, fp);
    for (int i = 0; i < dimX_; ++i) {
        fwrite(value_[i], sizeof(T), dimY_, fp);
    }
    fclose(fp);

Finish:
    return 1;
Error:
    return 0;
}

template <typename T>
inline bool DMatrix<T>::load(char* filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        goto Error;
    }
    int dimx, dimy;
    fread(&dimx, sizeof(uint), 1, fp);
    fread(&dimy, sizeof(uint), 1, fp);
    if (dimx != dimX_ || dimy != dimY_) {
        setSize(dimx, dimy);
    }
    for (int i = 0; i < dimX_; ++i) {
        fread(value_[i], sizeof(T), dimY_, fp);
    }
    fclose(fp);
    
Finish:
    return 1;
Error:
    return 0;
}

};
