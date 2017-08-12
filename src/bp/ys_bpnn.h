#ifndef _YS_BP_NODE_H_
#define _YS_BP_NODE_H_

namespace ys {

class BPLayer;
class BPNet;

class BPNode {
public:
    BPNode(int wNum);
    virtual ~BPNode();
public:
    bool forward();
    bool backward();
public:
    double rsumW(int pos);
    double rsumI(int pos);
public:
    void setInput(double *input);
    void setDersum(double dersum);
    void setLRate(double lRate);
    double getSum() const;
private:
    int wNum_;          // 权值参数维度
    double *weight_;    // 指向权值数组的指针
    double *input_;     // 指向输入数组的指针
    double bias_;       // 神经单元偏置项
    double sum_;        // 输入的加权和
    double dersum_;     // 损失函数对加和的偏导
    double lRate_;      // 神经网络的学习率
private:
    BPNode(const BPNode&);
    void operator=(const BPNode&);
};

class BPLayer {
public:
    BPLayer(int iNum, int oNum);
    virtual ~BPLayer();
public:
    bool forward();
    bool backward();
public:
    void setInput(double *input);
    void setROutput(double *routput);
    double* getOutput() const;
    double* getRInput() const;
protected:
    virtual double active(double value);
    virtual double ractive(double value);
private:
    int inputNum_;      // 输入维度==本层权重维度
    int outputNum_;     // 输出维度==本层节点个数
    double *input_;     // 输入指针，指向本层输入
    double *output_;    // 输出数组，存放本层输出
    double *rinput_;    // 损失函数对本层输入的偏导值
    double *routput_;   // 损失函数对本层输出的偏导值
    BPNode **node_;     // 本层节点数组
private:
    BPLayer(const BPLayer&);
    void operator=(const BPLayer&);
};

class BPNet {
public:
    BPNet(int layNum, int iNum, int oNum);
    virtual ~BPNet();
public:
    bool addLayer();
    bool forward();
    bool backward();
public:
    virtual double errorFunc();
    virtual double lossFunc(double input);
    virtual double regularFunc(double input);
private:
    int layNum_;        // 神经网络层数
    int inputNum_;      // 神经网络输入维度
    int outputNum_;     // 神经网络输出维度
    double *input_;     // 神经网络的输入
    double *output_;    // 神经网络的输出
    double *routput_;   // 损失函数对输出的偏导值
    BPLayer **lay_;     // 节点层的数组
    double lRate_;      // 神经网络学习率
    double rRate_;      // 神经网络正则率
};

}

#endif  // _YS_BP_NODE_H_
