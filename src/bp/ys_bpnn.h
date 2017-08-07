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
private:
    int wNum_;      // weight num;
    double *w_;     // weight array;
    double b_;      // bias
    double s_;      // sum
    double o_;      // output
private:
    BPNode(const BPNode&);
    void operator=(const BPNode&);
};

class BPLayer {
public:
    BPLayer(int curNum, int preNum, int index);
    ~BPLayer();
    friend class BPNet;
public:
    void setPre(BPNode** pre, int num);
    void setPost(BPNode** post, int num);
public:
    void forward();
    virtual double activeFunc(double sum);
private:
    int curNum_;
    int preNum_;
    int postNum_;
    BPNode **cur_;
    BPNode **pre_;
    BPNode **post_;
    int layerIndex_;
private:
    BPLayer(const BPLayer&);
    void operator=(const BPLayer&);
};

class BPNet {
public:
    BPNet();
    ~BPNet();
private:
    int layNum_;
    BPLayer *lay_;    
    double lRate_;      // learning rate
    double rRate_;      // regularization rate
};

}

#endif  // _YS_BP_NODE_H_
