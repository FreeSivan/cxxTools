#ifndef _PERFACT_HASH_H_
#define _PERFACT_HASH_H_

namespace ys {

#define T_LENGTH ((unsigned long)(1 << 10))
#define S_LENGTH ((unsigned long)(1 << 25))
#define F_LENGTH ((unsigned long)((unsigned int)1 << 31))

class phash {
public:
    phash();
    ~phash();
public:
    void addOver();
    long getpHash(unsigned int key);
    void addphKey(unsigned int key, int length);
private:
    unsigned short *first;
    unsigned int *second;
    unsigned long *third;
    unsigned short foffset;
    unsigned int soffset;
    unsigned long toffset;
    unsigned int lastKey;
private:
    phash(const phash&);
    phash& operator=(const phash&);
    void generate(unsigned int key);
};

}

#endif

