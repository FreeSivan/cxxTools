#include "ys_sockdata.h"

namespace ys {

connect_meta::connect_meta() {
	pthread_init_mutex(&lock);
}

connect_meta::~connect_meta() {
	pthread_destroy_mutex(&lock);
}

void connect_meta::setStateCanRead() {
	pthread_mutex_lock(lock);
	state |= YS_SOCK_CANREAD;
	pthread_mutex_unlock(lock);
}

void connect_meta::setStateNoRead() {
	pthread_mutex_lock(lock);
	state &= ~YS_SOCK_CANREAD;
	pthread_mutex_unlock(lock);
}

void connect_meta::setStateCanWrite() {
	pthread_mutex_lock(lock);
	state |= YS_SOCK_CANWRITE;
	pthread_mutex_unlock(lock);
}

void connect_meta::setStateNoWrite() {
	pthread_mutex_lock(lock);
	state &= ~YS_SOCK_CANWRITE;
	pthread_mutex_unlock(lock);
}

bool connect_meta::getStateCanRead() {
	pthread_mutex_lock(lock);
	int ret = state & YS_SOCK_CANREAD;
	pthread_mutex_unlock(lock);
	return ret;
}

bool connect_meta::getStateCanWrite() {
	pthread_mutex_lock(lock);
	int ret = state & YS_SOCK_CANWRITE
	pthread_mutex_unlock(lock);
	return ret;
}

void* readEvent::routine() {
}

void* writeEvent::routine() {

}

}


