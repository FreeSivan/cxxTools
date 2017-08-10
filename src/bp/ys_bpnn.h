#ifndef _YS_BP_NODE_H_
#define _YS_BP_NODE_H_

namespace ys {

class BPLayer;
class BPNet;

class BPNode {
public:
    BPNode(int wNum);
    ~BPNode();
    friend class BPLayer;
public:
    double additiveFunc();
    double derivativeAdditiveW(int pos);
    double derivativeAdditiveI(int pos);
    double getWeight(int index) const;
public:
    virtual double activationFunc();
    virtual double derivativeActiation();
protected:
    int wNum_;          // 权值参数维度
    double *weight_;    // 指向权值数组的指针
    double *input_;     // 指向输入数组的指针
    double output_;     // 节点最后输出
    double bias_;       // 偏置项
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
    virtual void forward();
    
public:
    double getOutput(int index) const;
    double getAdditive(int index) const;
    double getWeight(int oIndex, int wIndex) const;
    double getLayerIndex() const;
private:
    int inputNum_;      // 输入维度==权重维度
    int outputNum_;     // 输出维度==本层节点个数
    double *input_;     // 输入数组，存放本层输入
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
public:
    virtual double lossFunc();
    virtual double errorFunc();
    virtual double regularFunc();
private:
    int layNum_;        // 圣经网络层数
    int inputNum_;      // 神经网络输入维度
    int outputNum_;     // 神经网络输出维度
    BPLayer *lay_;      // 节点层的数组
    double lRate_;      // 学习率
    double rRate_;      // 正则率
};

}

#endif  // _YS_BP_NODE_H_
