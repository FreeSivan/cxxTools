/**
 *
 * author : yue sivan
 */
#ifndef _SOCK_DATA_H_
#define _SOCK_DATA_H_
#include "../tpool/ys_tpool.h"

namespace ys {

enum {
    YS_CLOSE,
    YS_READ,
    YS_WRITE
}

struct listen_meta {
    int sock;
    int pos;
    int state;
};

struct connect_meta : public listen_meta {
    pthread_mutex_t lock;
    connect_meta* link;
};

struct readEvent : public event {
public:
    virtual void* routine();
};

struct writeEvent : public event {
    virtual void* routine();
};

}

#endif

