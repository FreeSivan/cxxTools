/**
 * auther : xiwen.yxw_104792
 */

#ifndef _TPQUEUE_H_
#define _TPQUEUE_H_

#include <pthread.h>

#define DEFAULT_SIZE 100

template <typename T>
struct node {
    T m_data;
    node<T> *m_next;
};

template <typename T>
struct mtqueue {
public:
    mtqueue();
    ~mtqueue();
public:
    void setflag(int flag);
    void setsize(int size);
    void put(node<T> *val);
    void releaseFree();
    void resetqueue();
    node<T>* get();
private:
    node<T> *m_head;
    node<T> **m_tail;
    int m_cursize;
    int m_maxsize;
    int m_overflag;
    int m_fullwait;
    int m_freewait;
    pthread_cond_t m_free;
    pthread_cond_t m_full;
    pthread_mutex_t m_lock;
private:
    mtqueue(const mtqueue<T>&);
    mtqueue<T>& operator=(const mtqueue<T>&);
};

template <typename T>
inline mtqueue<T>::mtqueue() {
    m_head = 0;
    m_tail = &m_head;
    m_cursize = 0;
    m_maxsize = DEFAULT_SIZE;
    m_overflag = 0;
    m_freewait = 0;
    m_fullwait = 0;
    pthread_cond_init(&m_free, 0);
    pthread_cond_init(&m_full, 0);
    pthread_mutex_init(&m_lock, 0);
}

template <typename T>
inline mtqueue<T>::~mtqueue() {
    node<T> *p, *q;
    pthread_mutex_lock(&m_lock);
    for (p = m_head; p ; p = q) {
        q = p->m_next;
        delete p;
    }
    pthread_mutex_unlock(&m_lock);
    pthread_mutex_destroy(&m_lock);
    pthread_cond_destroy(&m_full);
    pthread_cond_destroy(&m_free);
}

template <typename T>
inline void mtqueue<T>::resetqueue() {
    node<T> *p, *q;
    pthread_mutex_lock(&m_lock);
    for (p = m_head; p; p = q) {
        q = p->m_next;
        delete p;
    }
    m_head = 0;
    m_tail = &m_head;
    m_cursize = 0;
    m_maxsize = DEFAULT_SIZE;
    m_overflag = 0;
    m_freewait = 0;
    m_fullwait = 0;
    pthread_mutex_unlock(&m_lock);
}

template <typename T>
inline void mtqueue<T>::setsize(int size) {
    pthread_mutex_lock(&m_lock);
    if (size > m_cursize) {
        m_maxsize = size;
    }
    pthread_mutex_unlock(&m_lock);
}

template <typename T>
inline void mtqueue<T>::setflag(int flag) {
    pthread_mutex_lock(&m_lock);
    m_overflag = flag;
    pthread_mutex_unlock(&m_lock);
}

template <typename T>
inline void mtqueue<T>::releaseFree() {
    pthread_mutex_lock(&m_lock);
    pthread_cond_broadcast(&m_free);
    pthread_mutex_unlock(&m_lock);
}

template <typename T>
inline node<T>* mtqueue<T>::get() {
    node<T> *node;
    pthread_mutex_lock(&m_lock);
    while (!m_cursize) {
        if (m_overflag) {
            pthread_mutex_unlock(&m_lock);
            return 0;
        }
        m_freewait++;
        pthread_cond_wait(&m_free, &m_lock);
    }
    if (m_overflag) {
        pthread_mutex_unlock(&m_lock);
        return 0;
    }
    node = m_head;
    m_head = node->m_next;
    if (!m_head) {
        m_tail = &m_head;
    }
    m_cursize--;
    if (m_fullwait > 0) {
        pthread_cond_signal(&m_full);
        m_fullwait--;
    }
    pthread_mutex_unlock(&m_lock);
    return node;
}

template <typename T>
inline void mtqueue<T>::put(node<T> *val) {
    if (!val) return;
    val->m_next = 0;
    pthread_mutex_lock(&m_lock);
    while (m_cursize >= m_maxsize) {
        m_fullwait++;
        pthread_cond_wait(&m_full, &m_lock);
    }
    m_cursize++;
    *m_tail = val;
    m_tail = &(val->m_next);
    if (m_freewait > 0) {
        pthread_cond_signal(&m_free);
        m_freewait--;
    }
    pthread_mutex_unlock(&m_lock);
}

#endif

