#ifndef _SKIPLIST_H_
#define _SKIPLIST_H_

#define COUNT_SIZE(_K, _V, _L) (sizeof(_K)+sizeof(_V)+(_L)*sizeof(int*))
#define DEFAULT_LEVEL 5

#include <malloc.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

template <class K, class V>
struct skNode {
    K key;
    V val;
    skNode* link[0];
};

template <class K, class V>
struct skList {
public:
    skList(int level = DEFAULT_LEVEL);
    ~skList();
public:
    bool getValue(K key, V& val) const;
    bool addTerm(K key, V val);
    bool delTerm(K key);
    void display();
private:
    skNode<K, V>* m_head;
    int m_length;
    int m_level;
private:
    skList(const skList&);
    skList& operator=(const skList&);
    int random_level();
};

template <class K, class V>
inline skList<K, V>::skList(int level){
    m_length = 0;
    m_level = level;
    m_head = (skNode<K, V>*)malloc(COUNT_SIZE(K, V, level));
    for (int i = 0; i < m_level; ++i) {
        m_head->link[i] = 0;
    }
}

template <class K, class V>
inline skList<K, V>::~skList() {
    skNode<K, V> *tmp = m_head;
    while (m_head) {
        tmp = m_head;
        m_head = m_head->link[0];
        free(tmp);
    }
}

template <class K, class V>
inline bool skList<K, V>::getValue(K key, V& val) const {
    skNode<K, V>* start = m_head;
    for (int i = m_level-1; i >= 0; --i) {
        skNode<K, V>* p = start;
        while (p->link[i]) {
            if (p->link[i]->key == key) {
                val = p->link[i]->val;
                return true;
            }
            if (p->link[i]->key > key) {
                break;
            }
            p = p->link[i];
        }
        start = p;
    }
    return false;
}

template <class K, class V>
inline bool skList<K, V>::addTerm(K key, V val) {
    V tmpV;
    if (getValue(key, tmpV)) {
        return false;
    }
    int level = random_level();
    skNode<K, V> *tmp = (skNode<K, V>*)malloc(COUNT_SIZE(K, V, level));
    tmp->key = key;
    tmp->val = val;
    for (int i = 0; i < level; ++i) {
        tmp->link[i] = 0;
    }
    skNode<K, V>* begin = m_head;
    for (int i = level-1; i >= 0; --i) {
        skNode<K, V>* p = begin;
        while (p->link[i]) {
            if (p->link[i]->key > key) {
                break;
            }
            p = p->link[i];
        }
        begin = p;
        tmp->link[i] = p->link[i];
        p->link[i] = tmp;
    }
    m_length++;
    return true;
}

template <class K, class V>
inline bool skList<K, V>::delTerm(K key) {
    skNode<K, V>* start = m_head;
    skNode<K, V>* tmp;
    for (int i = m_level-1; i >= 0; --i) {
        skNode<K, V>* p = start;
        while (p->link[i]) {
            if (p->link[i]->key == key) {
                tmp = p->link[i];
                p->link[i] = tmp->link[i];
                break;
            }
            if (p->link[i]->key > key) {
                break;
            }
            p = p->link[i];
        }
        start = p;
    }
    return true;
}

template <class K, class V>
inline void skList<K, V>::display() {
    skNode<K, V>* p;
    for (int i = m_level-1; i >= 0; --i) {
        p = m_head->link[i];
        while (p) {
            cout<<p->key<<"|"<<p->val<<" ";
            p = p->link[i];
        }
        cout<<endl;
    }
}

template <class K, class V>
inline int skList<K, V>::random_level() {
    int level = 1;
    while (level < m_level && random()%2) {
        level ++;
    }
    return level;
}

#endif

