#include "ys_bpnn.h"

namespace ys {

BPNode::BPNode(int wNum) {
    wNum_ = wNum;
    w_ = new double[wNum_];
    b_ = 0;
    s_ = 0;
    o_ = 0;
}

BPNode::~BPNode() {
    delete[] w_;
}

BPLayer::BPLayer(int cNum, int pNum, int index) {
    curNum_ = cNum;
   cur_ = new BPNode*[curNum_];
    for (int i = 0; i < curNum_; ++i) {
        cur_[i] = new BPNode(pNum);
    }
    pre_ = 0;
    post_ = 0;
    layerIndex_ = index;
}

BPLayer::~BPLayer() {
    for (int i = 0; i < curNum_; ++i) {
        delete cur_[i];
    }
    delete[] cur_;
}

void BPLayer::setPre(BPNode** pre, int pNum) {
    pre_ = pre;
    preNum_ = pNum;
}

void BPLayer::setPost(BPNode** post, int pNum) {
    post_ = post;
    postNum_ = pNum;
}

void BPLayer::forward() {
    for (int i = 0; i < curNum_; ++i) {
        double sum = 0;
        for (int j = 0; j < preNum_; ++j) {
            sum += cur_[i]->w_[j]*pre_[j]->o_;
        }
        cur_[i]->s_ = sum;
        double out = activeFunc(sum);
        cur_[i]->o_ = out;
    }
}

double BPLayer::activeFunc(double sum) {
    return sum; 
}

}
