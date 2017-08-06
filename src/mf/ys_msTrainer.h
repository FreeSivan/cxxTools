#ifndef _YS_MS_TRAINER_H_
#define _YS_MS_TRAINER_H_

#include "ys_trainerBase.h" 
#include "../matrix/ys_matrix.h"

namespace ys {      // namespace for yue sivan

#define MS_DT 0
#define MS_SDT 1

class MsTrainer : public TrainerBase {
public:
    MsTrainer();
    ~MsTrainer() {};
public:
    virtual bool load(char *path);
    virtual bool save(char *path);
    virtual void train();
private:
    void trainDT();
    void trainDTImpl(DMatrix<double>& r);
    void trainSDT();
    void trainSDTImpl(DMatrix<double>& r);
public:
    void setMinError(double minError);
    void setLearnStep(double minLearnStep);
    void setMaxTrainNum(int maxTrainNum);
    void setFactorNum(int factorNum);
    void setTrainFlag(int trainFlag);
    void setPFileName(char* pFileName);
    void setQFileName(char* qFileName);
private:
    DMatrix<double> r_;
    DMatrix<double> p_;
    DMatrix<double> q_;
    double minError_;
    double learnStep_;
    int maxTrainNum_;
    int factorNum_;
    int trainFlag_;
    char pfileName_[256];
    char qfileName_[256];
};

};

#endif // _YS_MS_TRAINER_H_
