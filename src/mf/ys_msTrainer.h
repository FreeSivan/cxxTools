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
    double deviationFunction();
protected:
    virtual bool trainImpl(int index) = 0;
protected:
    int trainThr_;					// 训练最大次数
    int factorNum_;					// 隐参数数目
    double errThr_;					// 错误率阈值
    double regRate_;				// 正则率
    double learnRate_;				// 学习率
    DMatrix<double> r_;				// 初始矩阵
    DMatrix<double> r1_;			// p_*q_结果
    DMatrix<double> p_;				// 结果矩阵p
    DMatrix<double> q_;				// 结果矩阵q
    char rfileName_[MAX_NAME_LEN];	// r_矩阵数据文件名
    char pfileName_[MAX_NAME_LEN];	// p_矩阵数据文件名
    char qfileName_[MAX_NAME_LEN];	// q_矩阵数据文件名
};

class GDMsTrainer : public MsTrainer {
public:
    GDMsTrainer() {}
    ~GDMsTrainer() {}
public:
    virtual bool trainImpl(int index);
private:
    double lossFuncP(int row, int col);
    double lossFuncQ(int row, int col);
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
    double lossFuncP(int i, int m, int j);
    double lossFuncQ(int m, int j, int i);
    double regularFuncP(int i, int m);
    double regularFuncQ(int m, int j);
};

};

#endif // _YS_MS_TRAINER_H_
