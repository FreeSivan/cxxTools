#include "ys_tpool.h"
#include <iostream>
#include <unistd.h>
using namespace std;
using namespace ys;
// abstract event
event::event() { link = 0; }
event::~event() {}
// test event
void* testEvent::routine() {
    pthread_t val = (pthread_t)args;
    cout<<"tid = "<<val<<endl;    
}
// threadPool
threadPool::threadPool(int thr_num) {
    head = 0;
    tail = &head;
    over_flag = 0;
    max_thr_num = thr_num;
    pthread_mutex_init(&lock, 0);
    pthread_cond_init(&empty, 0);
    thr_id = new pthread_t[max_thr_num];
    for (int i = 0; i < max_thr_num; ++i) {
        pthread_create(&thr_id[i], 0, threadPool::thrRun, (void*)this);
    }
}

threadPool::~threadPool() {
    setOverFlag(1);
    pthread_mutex_lock(&lock);
    pthread_cond_broadcast(&empty);
    pthread_mutex_unlock(&lock);
    while (head) {
        event *tmp = head;
        head = head->link;
        delete tmp;
    }
    delete[] thr_id;
    pthread_cond_destroy(&empty);
    pthread_mutex_destroy(&lock);
}

void threadPool::setOverFlag(int flag) {
    pthread_mutex_lock(&lock);
    over_flag = flag;
    pthread_mutex_unlock(&lock);
}

bool threadPool::tpAddEvent(event* e) {
    if (over_flag) {
        return false;
    } 
    pthread_mutex_lock(&lock);
    *tail = e;
    tail = &(e->link);
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&lock);
    return true;
}

void* threadPool::thrRun(void *args) {
    threadPool* p = (threadPool*)args;
    while (1) {
        pthread_mutex_lock(&(p->lock));
        if (p->over_flag) {
            pthread_mutex_unlock(&p->lock);
            break;
        }
        while (*(p->tail) == p->head) {
            if (p->over_flag) {
                return 0;
            }
            pthread_cond_wait(&p->empty, &p->lock);
        }
        event* work = p->head;
        p->head = work->link;
        if (!p->head) {
            p->tail = &(p->head);
        }
        pthread_mutex_unlock(&p->lock);
        //for test
        work->args = (void*)pthread_self();
        work->routine();
        delete work;
    }
}

// test code
int main() {
    threadPool tpool;
    while (1) {
        sleep(1);
        event* e = new testEvent();
        e->args = 0;
        tpool.tpAddEvent(e);
    }
}

