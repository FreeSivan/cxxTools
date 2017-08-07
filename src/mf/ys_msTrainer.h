#ifndef _YS_MS_TRAINER_H_
#define _YS_MS_TRAINER_H_

#include "ys_trainerBase.h" 
#include "../matrix/ys_matrix.h"

namespace ys {      // namespace for yue sivan

class MsTrainer : public TrainerBase {
public:
    MsTrainer();
    virtual ~MsTrainer() {};
public:
    virtual bool load(char *path);
    virtual bool save(char *path);
    virtual void train();
protected:
    virtual double deviateP(int row, int col) = 0;
    virtual double deviateQ(int row, int col) = 0;
    virtual double regularP(int row, int col) = 0;
    virtual double regularQ(int row, int col) = 0;
public:
    void setErrThr(double ErrThr);
    void setLRate(double lRate);
    void setTrainThr(int trainThr);
    void setFactorNum(int factorNum);
    void setPFileName(char* pFileName);
    void setQFileName(char* qFileName);
    void setRFileName(char* rFileName);
private:
    DMatrix<double> *r_;
    DMatrix<double> *r1_;
    DMatrix<double> *p_;
    DMatrix<double> *q_;
    double ErrThr_;
    double lRate_;
    double rRate_;
    int trainThr_;
    int factorNum_;
    char rfilename_[256];
    char pfileName_[256];
    char qfileName_[256];
};

class DTMsTrainer : public MsTrainer {
public:
    DTMsTrainer() {}
    ~DTMsTrainer() {}
protected:
    virtual double deviateP(int row, int col);
    virtual double deviateQ(int row, int col);
    virtual double regularP(int row, int col);
    virtual double regularQ(int row, int col);
};

class SDTMsTrainer : public MsTrainer {
public:
    SDTMsTrainer() {}
    ~SDTMsTrainer() {}
public:
    virtual void train(); 
};

};

#endif // _YS_MS_TRAINER_H_
