/**
 * author : yue sivan
 */
#ifndef _YS_TPOOL_H_
#define _YS_TPOOL_H_
#include <pthread.h>
#include "../mtqueue/ys_mtqueue.h"

namespace ys {

struct event {
public:
    event();
    virtual ~event();
public:
    virtual void* routine() = 0;
public:
    void* args;
private:
    event(const event&);
    event& operator=(const event&);
};

struct testEvent : public event {
public:
    virtual void* routine();
};

struct threadPool {
public:
    threadPool(int tNum = 10, int eNum = 100);
    ~threadPool();
public:
    void setOverFlag();
    bool tpAddEvent(event* e);
    static void* thrRun(void* args);
private:
    mt_queue<event*> queue;
    int max_thr_num;
    pthread_t *thr_id;
private:
    threadPool(const threadPool&);
    threadPool& operator=(const threadPool&);
};

}

#endif
