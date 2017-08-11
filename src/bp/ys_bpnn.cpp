#include "ys_bpnn.h"

namespace ys {

/*****************************************************
 *Summary: 神经元base类析构函数
 *      
 *      申请weight_的内存，不申请input_
 *
 *      input_在layer层申请
 *
 *Parameters:
 *
 *      无
 *
 *Return : 无
 *
 ****************************************************/
BPNode::BPNode(int wNum) {
    wNum_ = wNum;
    weight_ = new double[wNum_];
    input_ = NULL;
    output_ = bias_ = sum_ = 0;
}

/*****************************************************
 *Summary: 神经元base类析构函数
 *      
 *      负责释放weight_数组，不释放input_
 *
 *      input_在layer层维护，神经元只读访问
 *
 *Parameters:
 *
 *      无
 *
 *Return : 无
 *
 ****************************************************/
BPNode::~BPNode() {
    delete[] weight_;
}

/*****************************************************
 *Summary: 加和函数
 *      
 *      计算神经元输入关于权重的加权和
 *
 *Parameters:
 *
 *      无
 *
 *Return : 输入*权重+偏置
 ****************************************************/
double BPNode::additiveFunc() {
    if (!weight_ || !input_) {
        return -1;
    }
    sum_ = 0;
    for (int i = 0; i < wNum_; ++i) {
        sum_ += weight_[i] * input_[i];
    }
    sum_ += bias_;
    return sum_;
}

/*****************************************************
 *Summary: 激活函数
 *      
 *      将神经元的输出经过激活函数
 *
 *Parameters:
 *
 *      无
 *
 *Return : 经过激活函数计算神经元最后输出值
 *****************************************************/
double BPNode::activationFunc() {
    
}

/*****************************************************
 *Summary: 加和函数关于输入的偏导函数
 *      
 *      计算第pos个输入关于加和函数的偏导值
 *
 *Parameters:
 *
 *      int pos : 输入的编号
 *
 *Return : 第pos个输入关于加和函数的偏导值
 *
 *****************************************************/
double BPNode::derivativeI(int pos) {
    if (!weight_) {
        return -1;
    }
    return weight_[pos];
}

/*****************************************************
 *Summary: 加和函数关于权重的偏导函数
 *      
 *      计算第pos个权重关于加和函数的偏导值
 *
 *Parameters:
 *
 *      int pos : 权重的编号
 *
 *Return : 第pos个权重关于加和函数的偏导值
 *
 *****************************************************/
double BPNode::derivativeW(int pos) {
    if (!input_) {
        return -1;
    }
    return input_[pos];
}

/*****************************************************
 *Summary: 获取神经元的第index个权重
 *
 *Parameters:
 *
 *      int index : 带获取的权重的编号
 *
 *Return : 神经元第index个权重值
 *
 *****************************************************/
double BPNode::getWeight(int index) {
    if (index >= weight_ || !weight_) {
        return -1;
    }
    return weight_[index];
}

 /*****************************************************
  *Summary: 获取神经元的第index个权重
  *
  *Parameters:
  *
  *      int index : 带获取的权重的编号
  *
  *Return : 神经元第index个权重值
  *
  *****************************************************/
BPLayer::BPLayer(int iNum, int oNum, int index) {
    inputNum_ = iNum;
    outputNum_ = oNum;
    layerIndex_ = index;
    lLink_ = rLink_ = NULL;
    node_ = new BPNode*[oNum];
    for (int i = 0; i < oNum; ++i) {
        BPNode[i] = NULL;
    }
}

/*****************************************************
 *Summary: 获取神经元的第index个权重
 *
 *Parameters:
 *
 *      int index : 带获取的权重的编号
 *
 *Return : 神经元第index个权重值
 *
 *****************************************************/
BPLayer::~BPLayer() {
    // TODO
}

/*****************************************************
 *Summary: 获取本层第index个神经元的输出
 *
 *Parameters:
 *
 *      int index : 神经元编号
 *
 *Return : 编号为index的神经元的输出
 *
 *****************************************************/
double BPLayer::getOutput(int index) const {
    if (index >= outputNum_ || !node_) {
        return -1;
    }
    if (node_[index]) {
        return -1;
    }
    return node_[index]->output_;
}

/*****************************************************
 *Summary: 获取本层编号为index的神经元的加和函数结果
 *
 *Parameters:
 *
 *      int index : 神经元编号
 *
 *Return : 编号为index的神经元的加和函数结果
 *
 *****************************************************/
double BPLayer::getAdditive(int index) const {
    if (index >= outputNum_ || !node_) {
        return -1;
    }
    if (node_[index]) {
        return -1;
    }
    return node_[index]->sum_;
}

/*****************************************************
 *Summary: 获取编号为oIndex的神经元的第wIndex个权重
 *
 *Parameters:
 *
 *      int oIndex : 神经元编号
 *
 *      int wIndex : 权重编号
 *
 *Return : 编号为oIndex的神经元的第wIndex个权重
 *
 *****************************************************/
double BPLayer::getWeight(int oIndex, int wIndex) {
    if (oIndex >= outputNum_ || !node_) {
        return -1;
    }
    if (!node_[oIndex]) {
        return -1;
    }
    return node_[oIndex]->getWeight(wIndex);
}

/*****************************************************
 *Summary: 获取本层在神经网络中的编号
 *
 *Parameters:
 *
 *      无
 *
 *Return : 本层在神经网络中的编号
 *
 *****************************************************/
double BPLayer::getLayerIndex() const {
    return layerIndex_;
}

/*****************************************************
 *Summary: 前向函数，是本层的所有神经元进行前向计算
 *
 *Parameters:
 *
 *      无
 *
 *Return : 无
 *
 *****************************************************/
bool BPLayer::forward() {
    if (!input_ || !node_) {
        return false;
    }
    for (int i = 0; i < inputNum_; ++i) {
        input_[i] = lLink_->getOutput(i);
    }
    for (int i = 0; i < outputNum_; ++i) {        
        node_[i]->additiveFunc()
        node_[i]->activationFunc();
    }
}

}
