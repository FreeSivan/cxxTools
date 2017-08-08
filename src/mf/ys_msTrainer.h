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
protected:
    virtual void trainImpl(int index) = 0;
public:
    void setErrThr(double ErrThr);
    void setLRate(double lRate);
    void setTrainThr(int trainThr);
    void setFactorNum(int factorNum);
    void setPFileName(char* pFileName);
    void setQFileName(char* qFileName);
    void setRFileName(char* rFileName);
private:
    int trainThr_;
    int factorNum_;
    double errThr_;
    double regRate_;
    double learnRate_;
    DMatrix<double> r_;
    DMatrix<double> r1_;
    DMatrix<double> p_;
    DMatrix<double> q_;
    char rfilename_[MAX_NAME_LEN];
    char pfileName_[MAX_NAME_LEN];
    char qfileName_[MAX_NAME_LEN];
};

class DTMsTrainer : public MsTrainer {
public:
    DTMsTrainer() {}
    ~DTMsTrainer() {}
public:
    virtual bool trainImpl();
private:
    double errorFuncP(int row, int col);
    double errorFuncQ(int row, int col);
    double regularFuncP(int row, int col);
    double regularFuncQ(int row, int col);
};

class SDTMsTrainer : public MsTrainer {
public:
    SDTMsTrainer() {}
    ~SDTMsTrainer() {}
public:
    virtual bool trainImpl();
private: 
    double errorFuncP(int i, int m, int j);
    double errorFuncQ(int m, int j, int i);
    double regularFuncP(int i, int m);
    double regularFuncQ(int m, int j);
};

};

#endif // _YS_MS_TRAINER_H_
