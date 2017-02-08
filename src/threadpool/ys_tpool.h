#ifndef _YS_TPOOL_H_
#define _YS_TPOOL_H_
#include <pthread.h>

struct event {
public:
    event() {};
    virtual ~event() {};
public:
    virtual void* routine() = 0;
private:
    void* args;
    event* link;
private:
    event(const event&);
    event& operator(const event&);
};

struct threadPool {
public:
    threadPool(int thr_num = 10);
    ~threadPool();
public:
    bool tp_add_event(event* e);
    static void* thr_run(void* args);
private:
    event* head;
    int max_thr_num;
    pthread_t *thr_id;
    pthread_conf_t empty;
    pthread_mutux_t lock;
private:
    threadPool(const threadPool&);
    threadPool& operator=(const threadPool&);
};

#endif
