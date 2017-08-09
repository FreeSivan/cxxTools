#include <string.h>
#include "ys_msTrainer.h"
#include "../matrix/ys_matrixTool.h"

namespace ys {

MsTrainer::MsTrainer() {
    trainThr_ = 100000;
    factorNum_ = 100;
    errThr_ = 0.01;
    regRate_ = 0.2;
    learnRate_ = 0.1;
    memcpy(rfileName_, "rfile", sizeof("rfile")); 
    rfileName_[sizeof("rfile")] = 0;
    memcpy(pfileName_, "pfile", sizeof("pfile")); 
    pfileName_[sizeof("pfile")] = 0;
    memcpy(pfileName_, "qfile", sizeof("qfile")); 
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
/*
 *Summary: Sets the min error threshold for the 
 *         end of training 
 *
 *Parameters:
 *
 *   double errThr : min error threshold
 *
 *Return : no return 
 */
void MsTrainer::setErrThr(double errThr) {
    errThr_ = errThr;
}


/*
 *Summary: Sets the learning rate of the model 
 *
 *Parameters:
 *
 *   double learnRate : learing rate of the model
 *
 *Return : no return 
 */
void MsTrainer::setLRate(double learnRate) {
    learnRate_ = learnRate;
}

/*
 *Summary: Set the maximum training times of the model 
 *
 *Parameters:
 *
 *   int trainThr : the maximum training times
 *
 *Return : no return 
 */
void MsTrainer::setTrainThr(int trainThr) {
    trainThr_ = trainThr;
}

/*
 *Summary: Sets the number of hidden factors 
 *
 *Parameters:
 *
 *   int factorNum : the number of hidden factors
 *
 *Return : no return 
 */
void MsTrainer::setFactorNum(int factorNum) {
    factorNum_ = factorNum;
}

/*
 *Summary: set the file to save p matrix 
 *
 *Parameters:
 *
 *   char *pfileName : file name 
 *
 *Return : no return 
 */
void MsTrainer::setPFileName(char *pfileName) {
    int len = strlen(pfileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(pfileName_, pfileName, len);
    pfileName_[len] = 0;
}

/*
 *Summary: set the file to save q matrix 
 *
 *Parameters:
 *
 *   char *qfileName : file name 
 *
 *Return : no return 
 */
void MsTrainer::setQFileName(char *qfileName) {
    int len = strlen(qfileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(qfileName_, qfileName, len);
    qfileName_[len] = 0;
}

/*
 *Summary: set the file to save r matrix 
 *
 *Parameters:
 *
 *   char *rfileName : file name 
 *
 *Return : no return 
 */
void MsTrainer::setRFileName(char* rfileName) {
    int len = strlen(rfileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(rfileName_, rfileName, len);
    rfileName_[len] = 0;
}

/*
 *Summary: set the file to save r matrix 
 *
 *Parameters:
 *
 *   char *rfileName : file name 
 *
 *Return : no return 
 */
void MsTrainer::train() {
    r1_.setSize(r_.getDimX(), r_.getDimY());
    int trainNum = 0;
    while (trainNum < trainThr_) {
        if (trainImpl(trainNum)) {
            break;
        }
        trainNum ++;
    }
}

/*
 *Summary: Calculate the gradient value of p[i][m] 
 *         about the error function 
 *
 *Parameters:
 *
 *   int i : 
 *   int m : 
 *
 *Return : the gradient value of p[i][m] about the 
 *         error function 
 */
double DTMsTrainer::errorFuncP(int i, int m) {
    double thita = 0;
    int count = 0;
    for (int j = 0; j < r_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        count ++;
        thita += (r_[i][j]-r1_[i][j])*(-q_[m][j]);
    }
    if (!count) {
        return 0;
    }
    return thita / count;
}

/*
 *Summary: Calculate the gradient value of q[m][j] 
 *         about the error function 
 *
 *Parameters:
 *
 *   int m : 
 *   int j : 
 *
 *Return : the gradient value of q[m][j] about the 
 *         error function 
 */
double DTMsTrainer::errorFuncQ(int m, int j) {
    double thita = 0;
    int count = 0;
    for (int i = 0; i < r_.getDimX(); ++i) {
        if (!r_[i][j]) {
            continue;
        }
        count ++;
        thita += (r_[i][j]-r1_[i][j])*(-p_[i][m]);
    }
    if (!count) {
        return 0;
    }
    return thita / count;
}

/*
 *Summary: Calculate the gradient value of q[i][m] 
 *         about the regular function 
 *
 *Parameters:
 *
 *   int i : 
 *   int m : 
 *
 *Return : the gradient value of p[i][m] about the 
 *         error function 
 */
double DTMsTrainer::regularFuncP(int row, int col) {
    return 0;
}

double DTMsTrainer::regularFuncQ(int row, int col) {
    return 0;
}

bool DTMsTrainer::trainImpl(int index) {
    for (int i = 0; i < r_.getDimX(); ++i) {
    for (int j = 0; j < r_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        r1_[i][j] = Matrixs<double>::mulLine(p_, q_, i, j);
    }
    }
    if (!(index % 100)) {
        double err = Matrixs<double>::RMSE(r_, r1_);
        if (err < errThr_) {
            return true;
        }
    }
    for (int m = 0; m < factorNum_; ++m) {
        for (int i = 0; i < p_.getDimX(); ++i) {
            double errP = errorFuncP(i, m);
            double regP = regularFuncP(i, m);
            p_[i][m] -= learnRate_*(errP+regRate_*regP);    
        }
        for (int j = 0; j < q_.getDimY(); ++j) {
            double errQ = errorFuncQ(m, j);
            double regQ = regularFuncQ(m, j);
            q_[m][j] -= learnRate_*(errQ+regRate_*regQ);
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
            double errP = errorFuncP(i, m, j);
            double errQ = errorFuncQ(m, j, i);
            double regP = regularFuncP(i, m);
            double regQ = regularFuncQ(m, j);
            p_[i][m] -= learnRate_*(errP+regRate_*regP);
            q_[m][j] -= learnRate_*(errQ+regRate_*regQ);
        }
    }
    }
    if (!(index % 100)) {
        double err = Matrixs<double>::RMSE(r_, r1_);
        if (err < errThr_) {
            return true;
        }
    }
    return false;
}

};

