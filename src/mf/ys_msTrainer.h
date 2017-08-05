#ifndef _YS_MS_TRAINER_H_
#define _YS_MS_TRAINER_H_

namespace ys {      // namespace for yue sivan

class MsTrainer : public TrainerBase {
public:
    MsTrainer();
    ~MSTrainer();
public:
    virtual bool load(char *path);
    virtual bool save(char *path);
    virtual bool train();
private:
    bool trainDT();
    bool trainDTImpl(DMatrix<double>& r);
    bool trainSDT();
    bool trainSDTImpl(DMatrix<double>& r);
private:
    DMatrix<double> r_;
    DMatrix<double> p_;
    DMatrix<double> q_;
    double minError_;
    double learnStep_;
    int maxTrainNum_;
    int factorNum_;
    char pfileNmae_[256];
    char qfileName_[256];
};

};

#endif // _YS_MS_TRAINER_H_