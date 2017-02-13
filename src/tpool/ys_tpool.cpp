#include "ys_tpool.h"
#include <iostream>
#include <unistd.h>
using namespace std;

namespace ys {

// abstract event
event::event() {}
event::~event() {}
// test event
void* testEvent::routine() {
    pthread_t val = (pthread_t)args;
    cout<<"tid = "<<val<<endl;    
}
// threadPool
threadPool::threadPool(int tNum, int eNum) {
    queue.setsize(eNum);
    max_thr_num = tNum;
    thr_id = new pthread_t[max_thr_num];
    for (int i = 0; i < max_thr_num; ++i) {
        pthread_create(&thr_id[i], 0, threadPool::thrRun, (void*)this);
    }
}

threadPool::~threadPool() {
    delete[] thr_id;
}

void threadPool::setOverFlag() {
    queue.setflag(1);
    queue.releaseFree();
    queue.releaseFull();
}

bool threadPool::tpAddEvent(event* e) {
    mt_node<event*>* val = new mt_node<event*>();
    val->m_next = 0;
    val->m_data = e;
    return queue.put(val);
}

void* threadPool::thrRun(void *args) {
    threadPool* p = (threadPool*)args;
    while (1) {
        mt_node<event*>* work = p->queue.get();
        if (!work) {
            break;
        }
        //for test
        work->m_data->args = (void*)pthread_self();
        work->m_data->routine();
        delete work->m_data;
        delete work;
    }
}

}

using namespace ys;
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

