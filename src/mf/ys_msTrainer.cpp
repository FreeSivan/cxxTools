#include "ys_msTrainer.h"

namespace ys {

MsTrainer::MsTrainer() {}

MsTrainer::~MsTrainer() {}

bool MsTrainer::load(char *path) {
    return r_.load(path)
}

bool MsTrainer::save(char *path) {
    bool ret1 = p_.save(path);
    bool ret2 = q_.save(path);
    return (ret && ret);
}

bool MsTrainer::train() {
    
}

bool MsTrainer::trainDT() {
    DMatrix<double> r(r_.getDimX(), r_.getDimY());
    int trainNum = 0;
    while (trainNum < maxTrainNum_) {
        for (int i = 0; i < dimX; ++i) {
        for (int j = 0; j < dimY; ++j) {
            if (!r_[i][j]) {
                continue;
            }
            r[i][j] = Matrixs<double>::mulLine(p_, q_, i, j);
        }
        }
        if (!(trainNum % 100)) {
            double err = Matrixs<double>::RMSE_A(r_, r);
            if (err < minError_) {
                break;
            }
        }
        trainDTImpl(r);
        trainNum ++;
    }
}

bool MsTrainer::trainDTImpl(DMatrix<double>& r) {
    int dimX = r_.getDimX();
    int dimY = r_.getDimY();
    int dimK = factorNum_;
    double thita = 0;
    for (int i = 0; i < dimX; ++i) {
        for (int k = 0; k < dimK; ++k) {
            for (int j = 0; j < dimY; ++j) {
                if (!r_[i][j]) {
                    continue;
                }
                thita = (r_[i][j]-r[i][j])*(-q_[k][j]);
            }
        }
        p_[i][k] -= learnStep_ * thita;
    }
    thita = 0;
    for (int j = 0; j < dimY; ++j) {
        for (int k = 0; k < dimK; ++k) {
            for (int i = 0; i < dimX; ++i) {
                if (!r_[i][j]) {
                    continue;
                }
                thita = (r_[i][j]-r[i][j])*(-p[i][k]);
            }
        }
        q_[k][j] -= learnStep_ * thita;
    }
}

bool MsTrainer::trainSDT() {
    DMatrix<double> r(r_.getDimX(), r_.getDimY());
    int trainNum = 0;
    while (trainNum < maxTrainNum_) {
        trainSDTImpl(r);
        if (!(trainNum % 100)) {
            double err = Matrixs<double>::RMSE_A(r_, r);
            if (err < minError_) {
                break;
            }
        } 
        trainNum ++;
    }
}

bool MsTrainer::trainSDTImpl(DMatrix<double>& r) {
    int dimX = r_.getDimX();
    int dimY = r_.getDimY();
    int dimK = factorNum_;
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        if (!r_[i][j]) {
            continue;
        }
        r[i][j] = Matrixs::mulLine(p_, q_, i, j);
        for (int k = 0; k < dimK; ++k) {
            double ptheta = (r_[i][j]-r[i][j])*(-q_[k][j]);
            double qtheta = (r_[i][j]-r[i][j])*(-p_[i][k]);
            p_[i][k] -= learnStep_ * ptheta;
            q_[k][j] -= learnStep_ * qtheta;
        }
    }
    }
    
}

};

