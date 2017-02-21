/**
 *
 * author : yue sivan
 */
#ifndef _SOCK_DATA_H_
#define _SOCK_DATA_H_
#include "../tpool/ys_tpool.h"

namespace ys {

static const unsigned int YS_SOCK_CANREAD = 1 << 0;
static const unsigned int YS_SOCK_CANWRITE = 1 << 1;

struct listen_meta {
    int sock;
    int pos;
    int close;
};

struct connect_meta : public listen_meta {
public:
    connect_meta();
    ~connect_meta();
private:
    pthread_mutex_t lock;
    int state;
public:
    connect_meta* link;
public:
    void setState(int stat);
	int getState()const;
};

struct readEvent : public event {
public:
    virtual void* routine();
};

struct writeEvent : public event {
public:
    virtual void* routine();
};

}

#endif

