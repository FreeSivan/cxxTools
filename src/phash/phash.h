#ifndef _PERFACT_HASH_H_
#define _PERFACT_HASH_H_

namespace ys {

#define T_LENGTH (1 << 10)
#define S_LENGTH (1 << 25)
#define F_LENGTH (1 << 31)

class phash {
public:
    phash();
    ~phash();
public:
    long getpHash(unsigned int key);
    void addphKey(unsigned int key, int length);
private:
    unsigned short *first;
    unsigned int *second;
    unsigned long *third;
private:
    phash(const phash&);
    phash& operator=(const phash&);
};

}

#endif

