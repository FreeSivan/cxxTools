#ifndef _YS_TRAINER_BASE_H_
#define _YS_TRAINER_BASE_H_

namespace ys {      // namespace for ys

class TrainerBase {
public:
    TrainerBase() {}
    virtual ~TrainerBase() {}
public:
    virtual bool load(char *path) = 0;
    virtual bool save(char *path) = 0;
    virtual bool train() = 0;
private:
    TrainerBase(const TrainerBase&);
    void operator=(const TrainerBase&);
};

};

#endif