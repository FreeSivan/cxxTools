#ifndef _YS_TPOOL_H_
#define _YS_TPOOL_H_
#include <pthread.h>

namespace ys {

struct event {
public:
    event();
    virtual ~event();
public:
    virtual void* routine() = 0;
public:
    void* args;
    event* link;
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
    threadPool(int thr_num = 10);
    ~threadPool();
public:
    void setOverFlag(int flag);
    bool tpAddEvent(event* e);
    static void* thrRun(void* args);
private:
    event* head;
    event** tail;
    int over_flag;
    int max_thr_num;
    pthread_t *thr_id;
    pthread_cond_t empty;
    pthread_mutex_t lock;
private:
    threadPool(const threadPool&);
    threadPool& operator=(const threadPool&);
};

}

#endif
