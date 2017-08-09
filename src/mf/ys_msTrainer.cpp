#include <stdio.h>
#include <string.h>
#include "ys_msTrainer.h"
#include "../matrix/ys_matrixTool.h"

double ran_uniform();
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
    char fullpath[1024] = {0};
    sprintf (fullpath, "%s/%s", path, rfileName_);
    r_.load(fullpath);
    p_.setSize(r_.getDimX(), factorNum_);
    for (int i = 0; i < r_.getDimX(); ++i) {
        for (int j = 0; j < factorNum_; ++j) {
            p_[i][j] = ran_uniform();    
        }
    }
    q_.setSize(factorNum_, r_.getDimY());
    for (int i = 0; i < factorNum_; ++i) {
        for (int j = 0; j < r_.getDimY(); ++j) {
            q_[i][j] = ran_uniform();
        }
    }
    return true;
}

bool MsTrainer::save(char *path) {
    char fullpath[1024] = {0};
    sprintf (fullpath, "%s/%s", path, pfileName_);
    bool ret1 = p_.save(fullpath);
    sprintf (fullpath, "%s/%s", path, qfileName_);
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

double MsTrainer::deviationFunction() {
    if (r_.getDimX() != r1_.getDimX() || 
        r_.getDimY() != r1_.getDimY()) {
        return -1;
    }
    double sum = 0;
    int count = 0;
    for (int i = 0; i < r_.getDimX(); ++i) {
    for (int j = 0; j < r_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        sum += pow((r_[i][j]-r1_[i][j]), 2);
        count ++;
    }
    }
    return sqrt(sum / count);
}

/*
 *Summary: Calculate the gradient value of p[i][m] 
 *         about the error function 
 *
 *Parameters:
 *
 *   int i : dimX of matrix p
 *   int m : dimY of matrix p
 *
 *Return : the gradient value of p[i][m] about the 
 *         error function 
 */
double GDMsTrainer::lossFuncP(int i, int m) {
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
 *   int m : dimX of matrix q
 *   int j : dimY of matrix q
 *
 *Return : the gradient value of q[m][j] about the 
 *         error function 
 */
double GDMsTrainer::lossFuncQ(int m, int j) {
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
 *Summary: Calculate the gradient value of p[i][m] 
 *         about the regular function 
 *
 *Parameters:
 *
 *   int i : dimX of matrix p
 *   int m : dimY of matrix p
 *
 *Return : the gradient value of p[i][m] about the 
 *         regular function 
 */
double GDMsTrainer::regularFuncP(int i, int m) {
    return 0;
}

/*
 *Summary: Calculate the gradient value of q[i][m] 
 *         about the regular function 
 *
 *Parameters:
 *
 *   int m : dimX of matrix q
 *   int j : dimY of matrix q
 *
 *Return : the gradient value of q[i][m] about the 
 *         error function 
 */
double GDMsTrainer::regularFuncQ(int m, int j) {
    return 0;
}

/*
 *Summary: use standard gradient descent to train model
 *
 *Parameters:
 *
 *   int index : the current round of training
 *
 *Return : true : training over; false : training continue
 */
bool GDMsTrainer::trainImpl(int index) {
    for (int i = 0; i < r_.getDimX(); ++i) {
    for (int j = 0; j < r_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        r1_[i][j] = Matrixs<double>::mulLine(p_, q_, i, j);
    }
    }
    if (!(index % 100)) {
        if (deviationFunction() < errThr_) {
            return true;
        }
    }
    for (int m = 0; m < factorNum_; ++m) {
        for (int i = 0; i < p_.getDimX(); ++i) {
            double lossP = lossFuncP(i, m);
            double regP = regularFuncP(i, m);
            p_[i][m] -= learnRate_*(lossP+regRate_*regP);    
        }
        for (int j = 0; j < q_.getDimY(); ++j) {
            double lossQ = lossFuncQ(m, j);
            double regQ = regularFuncQ(m, j);
            q_[m][j] -= learnRate_*(lossQ+regRate_*regQ);
        }
    }
    return false;
}

/*
 *Summary: Calculate the gradient value of p[i][m] 
 *         about the SDT error function 
 *
 *Parameters:
 *
 *   int i : dimX of matrix p
 *   int m : dimY of matrix p
 *   int j : dimY of matrix r
 *
 *Return : the gradient value of p[i][m] about the 
 *         error function 
 */
double SGDMsTrainer::lossFuncP(int i, int m, int j) {
    return (r_[i][j]-r1_[i][j])*(-q_[m][j]);
}

/*
 *Summary: Calculate the gradient value of q[m][j] 
 *         about the SDT error function 
 *
 *Parameters:
 *
 *   int m : dimX of matrix q
 *   int j : dimY of matrix q
 *   int i : dimX of matrix r
 *
 *Return : the gradient value of q[m][j] about the 
 *         error function 
 */
double SGDMsTrainer::lossFuncQ(int m, int j, int i) {
    return (r_[i][j]-r1_[i][j])*(-p_[i][m]);
}

/*
 *Summary: Calculate the gradient value of p[i][m] 
 *         about the SDT regular function 
 *
 *Parameters:
 *
 *   int i : dimX of matrix p
 *   int m : dimY of matrix p
 *
 *Return : the gradient value of p[i][m] about the 
 *         regular function 
 */
double SGDMsTrainer::regularFuncP(int i, int m) {
    return 0;
}

/*
 *Summary: Calculate the gradient value of q[m][j] 
 *         about the SDT regular function 
 *
 *Parameters:
 *
 *   int m : dimX of matrix q
 *   int j : dimY of matrix q
 *
 *Return : the gradient value of q[m][j] about the 
 *         regular function 
 */
double SGDMsTrainer::regularFuncQ(int m, int j) {
    return 0;
}

/*
 *Summary: use Stochastic gradient descent to train model
 *
 *Parameters:
 *
 *   int index : the current round of training
 *
 *Return : true : training over; false : training continue
 */
bool SGDMsTrainer::trainImpl(int index) {
    for (int i = 0; i < p_.getDimX(); ++i) {
    for (int j = 0; j < q_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        r1_[i][j] = Matrixs<double>::mulLine(p_, q_, i, j);
        printf ("r1_[i][j] = %lf\n", r1_[i][j]);
        for (int m = 0; m < factorNum_; ++m) {
            double lossP = lossFuncP(i, m, j);
            double lossQ = lossFuncQ(m, j, i);
            double regP = regularFuncP(i, m);
            double regQ = regularFuncQ(m, j);
            p_[i][m] -= learnRate_*(lossP+regRate_*regP);
            q_[m][j] -= learnRate_*(lossQ+regRate_*regQ);
        }
    }
    }
    if (!(index % 100)) {
        double deviation = deviationFunction();
        printf ("count = %d, deviation = %lf\n", index, deviation);
        if (deviation < errThr_) {
            return true;
        }
    }
    return false;
}

};

#include <iostream>
#include <stdlib.h>

using namespace ys;

double ran_uniform() {
    return rand()/((double)RAND_MAX + 1);
}

int main() {
    DMatrix<double> tmpR;
    tmpR.setSize(1000, 10000);
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 10000; ++j) {
            if (rand()%100 < 90) {
                tmpR[i][j] = 0;
            }
            else {
                tmpR[i][j] = ran_uniform();                
            }
        }
    }
    tmpR.save("r.matrix");
    DMatrix<double> tmpR1;
    tmpR1.load("r.matrix");
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 10000; ++j) {
            if (tmpR[i][j] != tmpR1[i][j]) {
                printf ("error\n");
            }
        }
    }
    tmpR1.save("r1.matrix");
    SGDMsTrainer sgd;
    sgd.setRFileName("r.matrix");
    sgd.load(".");   
    sgd.train();
    return 0;
}
