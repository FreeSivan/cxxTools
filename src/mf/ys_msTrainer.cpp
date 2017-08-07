#include <string.h>
#include "ys_msTrainer.h"
#include "../matrix/ys_matrixTool.h"

namespace ys {

MsTrainer::MsTrainer() {
    minError_ = 0.01;
    learnStep_ = 0.1;
    maxTrainNum_= 65536;
    factorNum_ = 100;
}

bool MsTrainer::load(char *path) {
    return r_.load(path);
}

bool MsTrainer::save(char *path) {
    bool ret1 = p_.save(path);
    bool ret2 = q_.save(path);
    return (ret1 && ret2);
}

void MsTrainer::setMinError(double minError) {
    minError_ = minError;
}

void MsTrainer::setLearnStep(double minLearnStep) {
    learnStep_ = minLearnStep;
}

void MsTrainer::setMaxTrainNum(int maxTrainNum) {
    maxTrainNum_ = maxTrainNum;
}

void MsTrainer::setFactorNum(int factorNum) {
    factorNum_ = factorNum;
}

void MsTrainer::setTrainFlag(int trainFlag) {
    trainFlag_ = trainFlag;
}

void MsTrainer::setPFileName(char *pfileName) {
    int len = strlen(pfileName);
    if (len >= 256) {
        return;
    }
    memcpy(pfileName_, pfileName, len);
    pfileName_[len] = 0;
}

void MsTrainer::setQFileName(char *qfileName) {
    int len = strlen(qfileName);
    if (len >= 256) {
        return;
    }
    memcpy(qfileName_, qfileName, len);
    qfileName_[len] = 0;
}

void MsTrainer::train() {
    switch (trainFlag_) {
        case MS_DT: {
            trainDT();
            break;
        }
        case MS_SDT: {
            trainSDT();
            break;
        }
        default :{
            break; 
        }
    }
}

double MsTrianer::dtp(const DMatrix<double>& r, int row, int col) {
    double thita = 0;
    for (int j = 0; j < r_.getDimY(); ++j) {
        if (!r_[row][j]) {
            continue;    
        }
        thita += (r[row][j]-r_[row][j])*(-q_[col][j]);
    }
    return thita;
}

double MsTrainer::dtsp(const DMatrix<double>& r, int row, int col) {
    double thita = 0;
}

double MsTrainer::dtq(const DMatrix<double>& r, int row, int col) {
    double thita = 0;
    for (int i = 0; i < r_.getDimX(); ++i) {
        if (!r_[i][row]) {
            continue;    
        }
        thita += (r_[i][row]-r[i][row])*(-p[i][col]);
    }
    return thita;
}

double MsTrainer::dtsq(const DMatrix<double>& r, int row, int col) {

}

void MsTrainer::train() {
    int trainNum = 0;
    while (trainNum < trainNum_) {
        for (int m = 0; m < factorNum_; ++m) {
            for (int i = 0; i < r_.getDimX(); ++i) {
                double err = deviateP(i, m);
                double reg = regularP(i, m);
                p_[i][m] -= lRate_*(err+rRate_*reg);
            }
            for (int j = 0; j < r_.getDimY(); ++j) {
                double err = deviateQ(m, j);
                double reg = regularQ(m, j);
                q_[m][j] = lRate_*(err+rRate_*reg);
            }
        }
        
        trainNum ++;
    }
}

void MsTrainer::trainDT() {
    DMatrix<double> r(r_.getDimX(), r_.getDimY());
    int trainNum = 0;
    int dimX = r_.getDimX();
    int dimY = r_.getDimY();
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

void MsTrainer::trainDTImpl(DMatrix<double>& r) {
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
            p_[i][k] -= learnStep_ * thita;
        }
    }
    thita = 0;
    for (int j = 0; j < dimY; ++j) {
        for (int k = 0; k < dimK; ++k) {
            for (int i = 0; i < dimX; ++i) {
                if (!r_[i][j]) {
                    continue;
                }
                thita = (r_[i][j]-r[i][j])*(-p_[i][k]);
            }
            q_[k][j] -= learnStep_ * thita;
        }
    }
}

void MsTrainer::trainSDT() {
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

void MsTrainer::trainSDTImpl(DMatrix<double>& r) {
    int dimX = r_.getDimX();
    int dimY = r_.getDimY();
    int dimK = factorNum_;
    for (int i = 0; i < dimX; ++i) {
    for (int j = 0; j < dimY; ++j) {
        if (!r_[i][j]) {
            continue;
        }
        r[i][j] = Matrixs<double>::mulLine(p_, q_, i, j);
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

