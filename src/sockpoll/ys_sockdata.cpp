#include "ys_sockdata.h"

namespace ys {

connect_meta::connect_meta() {
    pthread_init_mutex(&lock);
}

connect_meta::~connect_meta() {
    pthread_destroy_mutex(&lock);
}

void connect_meta::setState(int stat) {
    pthread_mutex_lock(lock);
    state = stat;
    pthread_mutex_unlock(lock);
}

int connect_meta::getState()const {
    pthread_mutex_lock(lock);
    int stat = state 
    pthread_mutex_unlock(lock);
    return stat;
}

void* readEvent::routine() {
	connect_meta* p = (connect_meta*)args;
	// p->sock;
	cout<<""<<endl;
	p->setState(4);
}

void* writeEvent::routine() {
	connect_meta* p = (connect_meta*)args;
	// p->sock;
	cout<<""<<endl;
	p->setState(0);
}

}


