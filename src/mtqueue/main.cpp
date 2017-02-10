#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ys_mtqueue.h"

using namespace ys;

typedef struct _testData {
    long data;
} testData;

mt_queue<testData> pqueue;

void* thrfunc1(void *arg) {
    while (true) {
        sleep(1);
        mt_node<testData> *tmp = pqueue.get();
        printf ("data = %u\n", tmp->m_data.data);
        delete tmp;
    }    
}

void* thrfunc2(void *arg) {
    pthread_t pid = pthread_self();
    while (true) {
        mt_node<testData> *tmp = new mt_node<testData>();
        tmp->m_data.data = pid;
        pqueue.put(tmp);
        sleep(10);
    }
}

int main(void) {
    int i;
    void *ret;
    pthread_t tid1;
    pthread_t tidarr[10];
    pthread_create(&tid1, 0, thrfunc1, 0);
    for (i = 0; i < 10; ++i) {
        pthread_create(&tidarr[i], 0, thrfunc2, 0);    
    }
    pthread_join(tid1, &ret);
    return 0;
}

