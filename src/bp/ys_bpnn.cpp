#include "ys_bpnn.h"

namespace ys {

/***********************************************************
 *
 *  Summary: 
 *             
 *      申请weight_的内存，保存本节点的权值
 *
 *      不申请input_的内存，直接指向BPLay的input_
 *
 *  Parameters:
 *
 *      int wNum : 本层权重维度，也是输入维度
 *
 **********************************************************/
BPNode::BPNode(int wNum) {
    wNum_ = wNum;
    weight_ = new double[wNum_];
    input_ = NULL;
    bias_ = sum_ = 0;
    dersum_ = 0;
    lRate_ = 0;
}

/***********************************************************
 *
 *  Summary: 
 *    
 *      负责释放weight_数组，不释放input_
 *
 *      input_在layer层维护，神经元只读访问
 *
 **********************************************************/
BPNode::~BPNode() {
    delete[] weight_;
}

/***********************************************************
 *
 *  Summary: 
 *
 *      前向计算，是一个加和函数
 *      
 *      计算神经元输入关于权重的加权和
 *
 *  Return : 
 *      
 *      true: 执行成功； false: 执行失败
 *
 **********************************************************/
bool BPNode::forward() {
    if (!weight_ || !input_) {
        return false;
    }
    sum_ = 0;
    for (int i = 0; i < wNum_; ++i) {
        sum_ += weight_[i] * input_[i];
    }
    sum_ += bias_;
    return true;
}

/***********************************************************
 *
 *  Summary: 
 *
 *      后向计算，在损失函数负梯度方向更新每一个权重
 *      
 *  Return : 
 *      
 *      true: 执行成功； false: 执行失败
 *
 **********************************************************/
bool BPNode::backward() {
    for (int i = 0; i < wNum_; ++i) {
        if (-1 == rsumW(i)) {
            return false;
        }
        weight_[i] -=  lRate_ * dersum_* rsumW(i);
    }
    return true;
}

/***********************************************************
 *
 *  Summary: 
 *
 *      加和函数关于输入的偏导函数
 *      
 *      计算第pos个输入关于加和函数的梯度值
 *
 *  Parameters:
 *
 *      int pos : 输入的编号
 *
 *  Return : 
 *      
 *      第pos个输入关于加和函数的偏导值
 *
 ***********************************************************/
double BPNode::rsumI(int pos) {
    if (!weight_ || pos >= wNum_) {
        return -1;
    }
    return weight_[pos];
}

/***********************************************************
 *
 *  Summary: 
 *
 *      加和函数关于权重的偏导函数
 *      
 *      计算第pos个权重关于加和函数的梯度值
 *
 *  Parameters:
 *
 *      int pos : 权重的编号
 *
 *  Return : 
 *
 *      第pos个权重关于加和函数的偏导值
 *
 ***********************************************************/
double BPNode::rsumW(int pos) {
    if (!input_ || pos >= wNum_) {
        return -1;
    }
    return input_[pos];
}

/***********************************************************
 *
 *  Summary: 设置输入数组
 *
 ***********************************************************/
void BPNode::setInput(double *input) {
    input_ = input;
}

/***********************************************************
 *
 *  Summary: 设置损失函数L对加和值sum_的偏导值
 *
 ***********************************************************/
void BPNode::setDersum(double dersum) {
    dersum_ = dersum;
}

/***********************************************************
 *
 *  Summary: 设置模型的学习率，用于迭代权重
 *
 ***********************************************************/
void BPNode::setLRate(double lRate) {
    lRate_ = lRate;
}

/***********************************************************
 *
 *  Summary: 获取神经元的加和值
 *
 ***********************************************************/
double BPNode::getSum() const {
    return sum_;
}

/***********************************************************
 *
 * Summary: 
 *      
 *      Layer节点分配node_，output_，rinput_的内存
 *
 *      其中node_保存本层所有node
 *
 *      output_保存本层所有的输出
 *
 *      rinput_保存损失函数对本层所有输入的偏导值
 *
 *      input_不分配内存，直接指向前一层的output_
 *
 *      routput_不分配内存，直接指向前一层的rinput_
 *
 * Parameters:
 *
 *      int iNum : 输入维度，也是本层节点权值维度 
 *
 *      int oNum : 输出维度，也是本层节点个数
 *
 ***********************************************************/
BPLayer::BPLayer(int iNum, int oNum) {
    inputNum_ = iNum;
    outputNum_ = oNum;
    input_ = NULL;
    routput_ = NULL;
    output_ = new double[outputNum_];
    rinput_ = new double[inputNum_];
    node_ = new node_*[outputNum_];
    for (int i = 0; i < outputNum_; ++i) {
        node_[i] = new BPNode(inputNum_);
    }
}

/***********************************************************
 *
 *  Summary: 
 *      
 *      释放每个node节点，释放node数组
 *
 *      释放output_数组，释放rinput_数组
 * 
 *      input_和routput_不是自己申请，不需要释放
 *
 ***********************************************************/
BPLayer::~BPLayer() {
    for (int i = 0; i < outputNum_; ++i) {
        delete node_[i];
    }
    delete[] node_;
    delete[] output_;
    delete[] rinput_;
}

/***********************************************************
 *
 *  Summary: 
 *  
 *      前向函数，本层的所有神经元进行前向计算
 *
 *  setp:
 *     
 *      1. 迭代本层每个节点node_[i]
 *      
 *      2. 为每个node设置输入input
 *      
 *      3. 调用每个node的forward
 *      
 *      4. 获取每个node的sum值保存在output
 *
 *  return:
 *      
 *      true: 执行成功； false: 执行失败
 *      
 ***********************************************************/
bool BPLayer::forward() {
    if (!input_ || !node_ || !output_) {
        return false;
    }
    for (int i = 0; i < outputNum_; ++i) {
        node_[i]->setInput(input_);
        node_[i]->forward();
        output_[i] = node_[i].getSum();
    }
    return true;
}

/***********************************************************
 *
 *  Summary: 
 *  
 *      后向函数，使本层的所有神经元进行后向计算，
 *
 *      routput_保存损失函数对本层每个节点输出的偏导，
 *
 *      通过routput_[i]计算损失函数对每个节点的sum值
 *      
 *      的偏导，然后在每个节点中根据sum值的偏导计算
 *      
 *      损失函数对每个权重的偏导，进而调整各维度权重
 *
 *  setp:
 *     
 *      1. 迭代当前层每一个神经元node[i]
 *      
 *      2. 计算激活函数对node[i]加和值sum的偏导ors
 *      
 *      3. 计算损失函数对每个神经元的加和值的偏导lrs
 *      
 *      4. 将lrs设置到对应神经元中
 *
 *      5. 对该神经元调用backward函数
 *
 *      6. 计算损失函数对本层输入的偏导保存于rinput_
 *
 *  return:
 *      
 *      true: 执行成功； false: 执行失败
 *      
 ***********************************************************/
bool BPLayer::backward() {
    if (!node_ || !routput_ || !rinput_) {
        return false;
    }
    for (int i = 0; i < outputNum_; ++i) {
        double ors = ractive(node_[i].getSum());
        double lrs = routput_[i] * ors;
        node_[i]->setDersum(lrs);
        if (!node_[i]->backward()) {
            return false;
        }
        for (int j = 0; j < inputNum_; ++j) {
            if (-1 == node_[i]->rsumI(i)) {
                return false;
            }
            rinput_[i] += lrs * node_[i]->rsumI(i);
        }
    }
    return true;
}

/***********************************************************
 *
 *  Summary: 设置当前网络层的输入input
 *
 ***********************************************************/
void BPLayer::setInput(double *input) {
    input_ = input;
}

/***********************************************************
 *
 *  Summary: 设置损失函数对当前网络层各个输出的偏导值数组
 *
 ***********************************************************/
void BPLayer::setROutput(double *routput) {
    routput_ = routput;
}

/***********************************************************
 *
 *  Summary: 获取当前网络层的输出数组
 *
 ***********************************************************/
double* BPLayer::getOutput() const {
    return output_;
}

/***********************************************************
 *
 *  Summary: 获得损失函数对当前层的输入的偏导数组
 *
 ***********************************************************/
double* BPLayer::getRInput() const {
    return rinput_;
}

/***********************************************************
 *
 *  Summary: 默认激活函数，线性函数
 *
 ***********************************************************/
double active(double value) {
    return value;
}

/***********************************************************
 *
 *  Summary: 默认激活函数的导函数，线性函数的导函数
 *
 ***********************************************************/
double ractive(double value) {
    return 1;
}

int layNum_;        // 神经网络层数
int inputNum_;      // 神经网络输入维度
int outputNum_;     // 神经网络输出维度
double *input_;     // 神经网络的输入
double *output_;    // 神经网络的输出
double *routput_;   // 损失函数对输出的偏导值
BPLayer **lay_;     // 节点层的数组
double lRate_;      // 神经网络学习率
double rRate_;      // 神经网络正则率

/***********************************************************
 *
 *  Summary: 
 *             
 *      申请input_内存用于存放神经网络的输入
 *      
 *      申请output_内存用于存放神经网络的输出
 *
 *      申请routput_内存存放损失函数对输出的偏导
 *
 *      申请lay_内存用于存放神经网络的各层
 *
 *  Parameters:
 *
 *      int layNum : 神经网络层数
 *      
 *      int iNum : 神经网络输入维度
 *
 *      int oNum : 神经网络输出维度
 *
 **********************************************************/
BPNet::BPNet(int layNum, int iNum, int oNum) {
    layNum_ = layNum;
    inputNum_ = iNum;
    outputNum_ = oNum;
    input_ = new double[iNum];
    output_ = new double[oNum];
    routput_ = new double[oNum];
    lay_ = new BPLayer*[layNum_];
}

/***********************************************************
 *
 *  Summary: 
 *      
 *      释放input_, output_, routput_;
 *
 *      释放每个lay，最后释放lay_
 * 
 ***********************************************************/
BPNet::~BPNet() {
    delete[] input_;
    delete[] output_;
    delete[] routput_;
    for (int i = 0; i < layNum_; ++i) {
        delete lay_[i];
    }
    delete[] lay_;
}

/***********************************************************
 *
 *  Summary: 
 *
 *      前向函数，使本网络的所有层次的依次forward
 *
 *  setp:
 *     
 *      1. 将第一层的输入设置为本网络的输入
 *      
 *      2. 执行第一层的forward
 *      
 *      3. 迭代后续各层，每一层的输入设置为前一层的输出
 *      
 *      4. 为每一层调用forward
 *
 *      5. 将最后一层的输出设置为网络的输出
 *
 *  return:
 *      
 *      true: 执行成功； false: 执行失败
 *      
 ***********************************************************/
bool BPNet::forward() {
    lay_[0]->setInput(input_);
    if (!lay_[0]->forward()) {
        return false;
    }
    for (int i = 1; i < layNum_; ++i) {
        lay_[i]->setInput(lay_[i-1]->getOutput());
        if (!lay_[i]->forward()) {
            return false;
        }
    }
    for (int i = 0; i < outputNum_; ++i) {
        output_[i] = lay_[layNum_-1]->getOutput();
    }
    return true;
}

/***********************************************************
 *
 *  Summary: 
 *
 *      后向函数，使本网络的所有层次的依次backward
 *
 *      计算损失函数对每一个输出的偏导值，保存于routput_
 *      
 *      然后逐层往前迭代，执行backward函数
 *
 *  setp:
 *     
 *      1. 计算损失函数对每个输出的偏导值，保存于routput_
 *      
 *      2. 将routput_设置到最后一层，并执行其backward
 *      
 *      3. 迭代各层，每一层的routput设置为前一层的rinput_
 *      
 *      4. 每一层都调用backward函数，直至第一层
 *
 *  return:
 *      
 *      true: 执行成功； false: 执行失败
 *      
 ***********************************************************/
bool BPNet::backward() {
    for (int i = 0 ; i< outputNum_; ++i) {
        routput_[i] = lossFunc(output_[i]);
    }
    lay_[layNum_-1].setROutput(routput_);
    if (!lay_[layNum_-1].backward()) {
        return false;
    }
    for (int i = layNum_-2; i >= 0; --i) {
        if (!lay_[i]->backward()) {
            return false;
        }
        lay_[i]->setROutput(lay_[i+1]->getRInput());
    }
    return true;
}

}
