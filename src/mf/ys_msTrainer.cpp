#include <string.h>
#include "ys_msTrainer.h"
#include "../matrix/ys_matrixTool.h"

namespace ys {

MsTrainer::MsTrainer() {
    trainThr_ = 100000;
    factorNum_ = 100;
    errThr_ = 0.01;
    regRate_ = 0.2;
    learnRate_ = 0.1
    memcpy(rfilename_, "rfile", sizeof("rfile")); 
    rfilename_[sizeof("rfile")] = 0;
    memcpy(pfilename_, "pfile", sizeof("pfile")); 
    pfileName_[sizeof("pfile")] = 0;
    memcpy(pfilename_, "qfile", sizeof("qfile")); 
    qfileName_[sizeof("qfile")] = 0;
}

bool MsTrainer::load(char *path) {
    return r_.load(path);
}

bool MsTrainer::save(char *path) {
    bool ret1 = p_.save(path);
    bool ret2 = q_.save(path);
    return (ret1 && ret2);
}

void MsTrainer::setErrThr(double errThr) {
    errThr_ = errThr;
}

void MsTrainer::setLearnRate(double learnRate) {
    learnRate_ = learnRate;
}

void MsTrainer::setTrainThr(int trainThr) {
    trainThr_ = trainThr;
}

void MsTrainer::setFactorNum(int factorNum) {
    factorNum_ = factorNum;
}

void MsTrainer::setTrainFlag(int trainFlag) {
    trainFlag_ = trainFlag;
}

void MsTrainer::setPFileName(char *pfileName) {
    int len = strlen(pfileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(pfileName_, pfileName, len);
    pfileName_[len] = 0;
}

void MsTrainer::setQFileName(char *qfileName) {
    int len = strlen(qfileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(qfileName_, qfileName, len);
    qfileName_[len] = 0;
}

void MsTrainer::setRFileName(char* rFileName) {
    int len = strlen(rFileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(rfileName_, rfileName, len);
    rfileName_[len] = 0;
}

void DTMsTrainer::train() {
    r1_.reset(r_.getDimX(), r_.getDimY());
    int trainNum = 0;
    while (trainNum < trainThr_) {
        if (trainImpl(trainNum)) {
            break;
        }
        trainNum ++;
    }
}

double DTMsTrainer::errorFuncP(int row, int col) {
    double thita = 0;
    int count = 0;
    for (int j = 0; j < r_.getDimY(); ++j) {
        if (!r_[row][j]) {
            continue;
        }
        count ++;
        thita += (r_[row][j]-r1_[row][j])*(-q_[col][j]);
    }
    if (!count) {
        return 0;
    }
    return thita / count;
}

double DTMsTrainer::errorFuncQ(int row, int col) {
    double thita = 0;
    int count = 0;
    for (int i = 0; i < r_.getDimX(); ++i) {
        if (!r_[i][col]) {
            continue;
        }
        count ++;
        thita += (r_[i][col]-r1[i][col])*(-p_[i][row]);
    }
    if (!count) {
        return 0;
    }
    return thita / count;
}

double DTMsTrainer::regularFuncP(int row, int col) {
    return 0;
}

double DTMsTrainer::regularFuncQ(int row, int col) {
    return 0;
}

void DTMsTrainer::trainImpl(int index) {
    for (int i = 0; i < r_.getDimX(); ++i) {
    for (int j = 0; j < r_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        r1_[i][j] = Matrixs<double>::mulLine(p_, q_, i, j);
    }
    }
    if (!(index % 100)) {
        dobule err = Matrixs<double>::RMSE_A(*r_, *r1_);
        if (err < errThr_) {
            return true;
        }
    }
    for (int m = 0; m < factorNum_; ++m) {
        for (int i = 0; i < p_.getDimX(); ++i) {
            double errP = errorFuncP(i, m);
            double regP = regularFuncP(i, m);
            p_[i][m] -= learnRate*(errP+regRate*regP);    
        }
        for (int j = 0; j < q_.getDimY(); ++j) {
            double errQ = errorFuncQ(m, j);
            double regQ = regularFuncQ(m, j);
            q_[m][j] -= learnRate*(errQ+regRate*regQ);
        }
    }
    return false;
}

double SDTMsTrainer::errorFuncP(int i, int m, int j) {
    return (r_[i][j]-r1_[i][j])*(-q_[m][j]);
}

double SDTMsTrainer::errorFuncQ(int m, int j, int i) {
    return (r_[i][j]-r1_[i][j])*(-p_[i][m]);
}

double SDTMsTrainer::regularFuncP(int i, int m) {
    return 0;
}

double SDTMsTrainer::regularFuncQ(int m, int j) {
    return 0;
}

bool SDTMsTrainer::trainImpl(int index) {
    for (int i = 0; i < p_.getDimX(); ++i) {
    for (int j = 0; j < q_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        for (int m = 0; m < factorNum_; ++m) {
            errP = errorFuncP(i, m, j);
            errQ = errorFuncQ(m, j, i);
            regP = regularFuncP(i, m);
            regQ = regularFuncQ(m, j);
            p_[i][m] -= learnRate*(errP+regRate*regP);
            q_[m][j] -= learnRate*(errQ+regRate*regQ);
        }
    }
    }
    if (!(index % 100)) {
        dobule err = Matrixs<double>::RMSE_A(*r_, *r1_);
        if (err < errThr_) {
            return true;
        }
    }
    return false;
}

};

