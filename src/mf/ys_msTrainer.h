#ifndef _YS_MS_TRAINER_H_
#define _YS_MS_TRAINER_H_

#include "ys_trainerBase.h" 
#include "../matrix/ys_matrix.h"

namespace ys {      // namespace for yue sivan

#define MAX_NAME_LEN 256

class MsTrainer : public TrainerBase {
public:
    MsTrainer();
    virtual ~MsTrainer() {};
public:
    virtual bool load(char *path);
    virtual bool save(char *path);
public:
    virtual void train();
public:
    void setErrThr(double ErrThr);
    void setLRate(double lRate);
    void setTrainThr(int trainThr);
    void setFactorNum(int factorNum);
    void setPFileName(char* pFileName);
    void setQFileName(char* qFileName);
    void setRFileName(char* rFileName);
protected:
    virtual bool trainImpl(int index) = 0;
protected:
    int trainThr_;          //default 100000
    int factorNum_;         //default 100
    double errThr_;         //default 0.01
    double regRate_;        //default 0.2
    double learnRate_;      //default 0.1
    DMatrix<double> r_;     //Training data
    DMatrix<double> r1_;    //Matrix p*q
    DMatrix<double> p_;     //Result matrix p
    DMatrix<double> q_;     //Result matrix q
    char rfileName_[MAX_NAME_LEN];
    char pfileName_[MAX_NAME_LEN];
    char qfileName_[MAX_NAME_LEN];
};

class GDMsTrainer : public MsTrainer {
public:
    GDMsTrainer() {}
    ~GDMsTrainer() {}
public:
    virtual bool trainImpl(int index);
private:
    double errorFuncP(int row, int col);
    double errorFuncQ(int row, int col);
    double regularFuncP(int row, int col);
    double regularFuncQ(int row, int col);
};

class SGDMsTrainer : public MsTrainer {
public:
    SGDMsTrainer() {}
    ~SGDMsTrainer() {}
public:
    virtual bool trainImpl(int index);
private: 
    double errorFuncP(int i, int m, int j);
    double errorFuncQ(int m, int j, int i);
    double regularFuncP(int i, int m);
    double regularFuncQ(int m, int j);
};

};

#endif // _YS_MS_TRAINER_H_
