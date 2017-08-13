#include <stdio.h>
#include <string.h>
#include "ys_msTrainer.h"
#include "../matrix/ys_matrixTool.h"

double ran_uniform();
namespace ys {
/****************************************************************
 *
 *	Summary: 初始化默认参数，分配初始资源
 *
 ***************************************************************/
MsTrainer::MsTrainer() {
    trainThr_ = 100000;
    factorNum_ = 10;
    errThr_ = 0.001;
    regRate_ = 0.2;
    learnRate_ = 0.1;
    memcpy(rfileName_, "rfile", sizeof("rfile")); 
    rfileName_[sizeof("rfile")] = 0;
    memcpy(pfileName_, "pfile", sizeof("pfile")); 
    pfileName_[sizeof("pfile")] = 0;
    memcpy(qfileName_, "qfile", sizeof("qfile")); 
    qfileName_[sizeof("qfile")] = 0;
}

/****************************************************************
 *
 *	Summary: 从指定文件中加载r_的结果到内存
 *
 ***************************************************************/
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

/****************************************************************
 *
 *	Summary: 保存q_和p_的结果到指定文件中
 *
 ***************************************************************/
bool MsTrainer::save(char *path) {
    char fullpath[1024] = {0};
    sprintf (fullpath, "%s/%s", path, pfileName_);
    bool ret1 = p_.save(fullpath);
    sprintf (fullpath, "%s/%s", path, qfileName_);
    bool ret2 = q_.save(fullpath);
    return (ret1 && ret2);
}
/****************************************************************
 *
 *	Summary: 设置最小误差阈值，作为训练结束条件
 *
 ***************************************************************/
void MsTrainer::setErrThr(double errThr) {
    errThr_ = errThr;
}


/****************************************************************
 *
 *	Summary: 设置正则率，正则函数的权重
 *
 ***************************************************************/
void MsTrainer::setLRate(double learnRate) {
    learnRate_ = learnRate;
}

/****************************************************************
 *
 *	Summary: 设置训练最大次数
 *
 ***************************************************************/
void MsTrainer::setTrainThr(int trainThr) {
    trainThr_ = trainThr;
}

/****************************************************************
 *
 *	Summary: 设置隐藏参数数目
 *
 ***************************************************************/
void MsTrainer::setFactorNum(int factorNum) {
    factorNum_ = factorNum;
}

/****************************************************************
 *
 *	Summary: 设置p矩阵存储文件名
 *
 ***************************************************************/
void MsTrainer::setPFileName(char *pfileName) {
    int len = strlen(pfileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(pfileName_, pfileName, len);
    pfileName_[len] = 0;
}

/****************************************************************
 *
 *	Summary: 设置q矩阵存储文件名
 *
 ***************************************************************/
void MsTrainer::setQFileName(char *qfileName) {
    int len = strlen(qfileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(qfileName_, qfileName, len);
    qfileName_[len] = 0;
}

/****************************************************************
 *
 *	Summary: 设置r矩阵存储文件名
 *
 ***************************************************************/
void MsTrainer::setRFileName(char* rfileName) {
    int len = strlen(rfileName);
    if (len >= MAX_NAME_LEN) {
        return;
    }
    memcpy(rfileName_, rfileName, len);
    rfileName_[len] = 0;
}

/****************************************************************
 *
 *	Summary: 训练函数，循环调用trainImpl，直至结束
 *
 ***************************************************************/
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

/****************************************************************
 *
 *	Summary: 误差函数，计算两个矩阵的标准差
 *
 ***************************************************************/
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

/****************************************************************
 *
 *	Summary: 
 *
 *		标准梯度下降的损失函数对p_[i][m]的导函数，标准梯度下降的
 *	
 *		损失函数是所有样本在模型的输出和样本输出的差的平方和，以
 *
 *		这个函数对p[i][m]的导函数在该点的导数值
 *
 *	Parameters:
 *
 *		int i : 当前要更新的参数p_[i][m]的x下标
 *
 *		int m : 当前要更新的参数p_[i][m]的y下标
 *	
 *	return:
 *
 *		损失函数的导函数在p_[i][m]点处的导数值
 *
 ***************************************************************/
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

/****************************************************************
 *
 *	Summary: 
 *
 *		标准梯度下降的损失函数对q_[m][j]的导函数，标准梯度下降的
 *	
 *		损失函数是所有样本在模型的输出和样本输出的差的平方和，以
 *
 *		这个函数对q_[m][j]的导函数在该点的导数值
 *	
 *	Parameters:
 *
 *		int m : 当前要更新的参数q_[m][j]的x下标
 *
 *		int j : 当前要更新的参数q_[m][j]的y下标
 *	
 *	return:
 *
 *		损失函数的导函数在q_[m][j]点处的导数值
 *
 ***************************************************************/
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

/****************************************************************
 *
 *	Summary: 
 *
 *		正则函数对p_[i][m]的导函数，正则函数是为了防止过拟合而
 *	
 *		拟定的和损失函数一起对参数求梯度，和损失函数对参数的偏
 *
 *		导一样，一起参与参数的迭代更新
 *	
 *	Parameters:
 *
 *		int i : 当前要更新的参数p_[i][m]的x下标
 *
 *		int m : 当前要更新的参数p_[i][m]的y下标
 *	
 *	return:
 *
 *		损失函数的导函数在p_[i][m]点处的导数值
 *
 ***************************************************************/
double GDMsTrainer::regularFuncP(int i, int m) {
    return 0;
}

/****************************************************************
 *
 *	Summary: 
 *
 *		正则函数对q_[m][j]的导函数，正则函数是为了防止过拟合而
 *	
 *		拟定的和损失函数一起对参数求梯度，和损失函数对参数的偏
 *
 *		导一样，一起参与参数的迭代更新
 *	
 *	Parameters:
 *
 *		int m : 当前要更新的参数q_[m][j]的x下标
 *
 *		int j : 当前要更新的参数q_[m][j]的y下标
 *	
 *	return:
 *
 *		损失函数的导函数在q_[m][j]点处的导数值
 *
 ***************************************************************/
double GDMsTrainer::regularFuncQ(int m, int j) {
    return 0;
}

/****************************************************************
 *
 *	Summary: 
 *
 *		执行标准梯度下降一轮训练，迭代q_,p_矩阵中的值
 *	
 *	step:
 *
 *		1. 根据矩阵p_，q_当前值，计算当前轮次r1_的值
 *
 *		2. 判断r1_和r_的当前无差，若小于阈值则返回
 *
 *		3. 迭代每个隐性因子m，更新每个p[i][m]和q[m][j]
 *
 *		4. 对于每个p_[i][m]，根据损失梯度和正则梯度更新
 *
 *		5. 对于每个q_[m][m]，根据损失梯度和正则梯度更新
 *	
 *	Parameters:
 *
 *		int index : 当前训练轮次
 *
 *	return:
 *
 *		损失函数的导函数在q_[m][j]点处的导数值
 *
 ***************************************************************/
bool GDMsTrainer::trainImpl(int index) {
    for (int i = 0; i < r_.getDimX(); ++i) {
    for (int j = 0; j < r_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        r1_[i][j] = Matrixs<double>::mulLine(p_, q_, i, j);
    }
    }
    if (deviationFunction() < errThr_) {
		return true;
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

/****************************************************************
 *
 *	Summary: 
 *
 *		随机梯度下降的损失函数对p_[i][m]的导函数，随机梯度下降的
 *	
 *		损失函数是当前样本在模型的输出和样本输出的差的平方，以这
 *
 *		个函数对p[i][m]的导函数在该点的导数值
 *
 *	Parameters:
 *
 *		int i : 当前样本r_[i][j]的x维度下标
 *
 *		int m : 当前训练参数p_[i][m]的y维度下标
 *
 *		int j : 当前样本r_[i][j]的y维度下标
 *	
 *	return:
 *
 *		损失函数的导函数在p_[i][m]点处的导数值
 *
 ***************************************************************/
double SGDMsTrainer::lossFuncP(int i, int m, int j) {
    return (r_[i][j]-r1_[i][j])*(-q_[m][j]);
}

/****************************************************************
 *
 *	Summary: 
 *
 *		随机梯度下降的损失函数对q_[m][j]的导函数，随机梯度下降的
 *	
 *		损失函数是当前样本在模型的输出和样本输出的差的平方，以这
 *
 *		个函数对q[m][j]的导函数在该点的导数值
 *
 *	Parameters:
 *
 *		int m : 当前训练参数q_[i][m]的x维度下标
 *
 *		int j : 当前样本r_[i][j]的y维度下标
 *
 *		int i : 当前样本r_[i][j]的x维度下标
 *	
 *	return:
 *
 *		损失函数的导函数在p_[i][m]点处的导数值
 *
 ***************************************************************/
double SGDMsTrainer::lossFuncQ(int m, int j, int i) {
    return (r_[i][j]-r1_[i][j])*(-p_[i][m]);
}

/****************************************************************
 *
 *	Summary: 
 *
 *		正则函数对p_[i][m]的导函数，正则函数是为了防止过拟合而
 *	
 *		拟定的和损失函数一起对参数求梯度，和损失函数对参数的偏
 *
 *		导一样，一起参与参数的迭代更新
 *	
 *	Parameters:
 *
 *		int i : 当前要更新的参数p_[i][m]的x下标
 *
 *		int m : 当前要更新的参数p_[i][m]的y下标
 *	
 *	return:
 *
 *		损失函数的导函数在p_[i][m]点处的导数值
 *
 ***************************************************************/
double SGDMsTrainer::regularFuncP(int i, int m) {
    return 0;
}

/****************************************************************
 *
 *	Summary: 
 *
 *		正则函数对q_[m][j]的导函数，正则函数是为了防止过拟合而
 *	
 *		拟定的和损失函数一起对参数求梯度，和损失函数对参数的偏
 *
 *		导一样，一起参与参数的迭代更新
 *	
 *	Parameters:
 *
 *		int m : 当前要更新的参数q_[m][j]的x下标
 *
 *		int j : 当前要更新的参数q_[m][j]的y下标
 *	
 *	return:
 *
 *		损失函数的导函数在q_[m][j]点处的导数值
 *
 ***************************************************************/
double SGDMsTrainer::regularFuncQ(int m, int j) {
    return 0;
}

/****************************************************************
 *
 *	Summary: 
 *
 *		执行随机梯度下降一轮训练，迭代q_,p_矩阵中的值
 *	
 *	step:
 *
 *		1. 迭代r_的每个值r_[i][j]，如果r_[i][j]为0则跳过
 *
 *		2. 根据当前p_,q_矩阵的值计算r1_[i][j]
 *
 *		3. 迭代每个隐性因子m，更新每个p[i][m]和q[m][j]
 *
 *		4. 对于每个p_[i][m]，根据损失梯度和正则梯度更新
 *
 *		5. 对于每个q_[m][m]，根据损失梯度和正则梯度更新
 *	
 *	Parameters:
 *
 *		int index : 当前训练轮次
 *
 *	return:
 *
 *		损失函数的导函数在q_[m][j]点处的导数值
 *
 ***************************************************************/
bool SGDMsTrainer::trainImpl(int index) {
    printf ("index = %d\n", index);
    for (int i = 0; i < p_.getDimX(); ++i) {
    for (int j = 0; j < q_.getDimY(); ++j) {
        if (!r_[i][j]) {
            continue;
        }
        r1_[i][j] = Matrixs<double>::mulLine(p_, q_, i, j) / factorNum_;
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
    double deviation = deviationFunction();
    printf ("count = %d, deviation = %lf\n", index, deviation);
    if (deviation < errThr_) {
        return true;
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
    tmpR.setSize(100, 300);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 300; ++j) {
            if (rand()%100 < 90) {
                tmpR[i][j] = 0;
            }
            else {
                tmpR[i][j] = ran_uniform();                
            }
        }
    }
    tmpR.save("r.matrix");
    SGDMsTrainer sgd;
    sgd.setRFileName("r.matrix");
    sgd.load(".");   
    sgd.train();
    sgd.save(".");
    DMatrix<double> p;
    DMatrix<double> q;
    p.load("pfile");
    q.load("qfile");
    DMatrix<double> *r1 = Matrixs<double>::mul(p, q);
    for (int i = 0; i < 100; ++i) {
        for (int  j = 0; j < 300; ++j) {
            printf ("%d     %d      %lf     %lf\n", i, j, tmpR[i][j], (*r1)[i][j]/10);
        }
    }
    return 0;
}
