#ifndef _YS_BP_NODE_H_
#define _YS_BP_NODE_H_

namespace ys {

class BPLayer;
class BPNet;

class BPNode {
public:
    BPNode(int wNum);
    virtual ~BPNode();
    friend class BPLayer;
protected:
    void additiveFunc();
    void countWeight(double der);
    void derivativeAdditiveW(int pos);
    void derivativeAdditiveI(int pos);
protected:
    int wNum_;          // 权值参数维度
    double *weight_;    // 指向权值数组的指针
    double *input_;     // 指向输入数组的指针
    double bias_;       // 神经单元偏置项
    double sum_;        // 输入的加权和
private:
    BPNode(const BPNode&);
    void operator=(const BPNode&);
};

class BPLayer {
public:
    BPLayer(int iNum, int oNum, int index);
    virtual ~BPLayer();
    friend class BPNet;
public:
    bool forward();
    void backward();
public:
    double getOutput(int index) const;
    double getAdditive(int index) const;
    double getWeight(int oIndex, int wIndex) const;
    double getLayerIndex() const;
protected:
    virtual double activationFunc(double value);
    virtual double derivativeActiation();
private:
    int inputNum_;      // 输入维度==权重维度
    int outputNum_;     // 输出维度==本层节点个数
    double *input_;     // 输入指针，指向本层输入
    double *output_;    // 输出数组，存放本层输出
    double *rinput_;    // 损失函数对本层输入的偏导值
    double *routput_;   // 损失函数对本层输出的偏导值
    BPNode **node_;     // 本层节点数组
    BPLayer *lLink_;    // 指向前一层的指针
    BPLayer *rLink_;    // 指向后一层的指针
    int layerIndex_;    // 当前层次的编号
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
