#include "ys_tpool.h"

threadPool::threadPool(int thr_num) {
    head = 0;
    max_thr_num = thr_num;
    pthread_mutex_init(&lock);
    pthread_cond_init(&empty);
    thr_id = new pthread_t();
    for (int i = 0; i < thr_num; ++i) {
        pthread_create(&thr_id, 0, threadPool::thr_run, 0);
    }
}

threadPool::~threadPool() {
    pthread_mutex_lock(&lock);
    pthread_conf_broadcast(&empty);
    pthread_mutex_unlock(&lock);

}

