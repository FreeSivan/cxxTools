#include "ys_sockdata.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>

sock_module::sock_module(char* host, int port) {
    struct sockaddr_in local;
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    inet_pton(AF_INET, host, &local.sin_addr);
    if ((listen.sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    if (-1 == bind(lsock, (struct sockaddr*)&local, sizeof(local))) {
        return -1;
    }
    if (-1 == listen(lsock, LISTEN_NUM)) {
        return -1;
    }
    listen.sock = lsock;
    listen.pos = -1;
    listen.close = 0;
    connList = 0;
}

sock_module::~sock_module() {

}

void sock_module::prepare(struct pollfd *psock, int& nsock) {
    connect_meta* p = connList;
    int pos = nsock;
    if (!listen.close) {
        psock[pos].fd = listen.sock;
        listen.pos = pos;
        psock[pos++].events = POLLIN;
    }
    while (p) {
        pthread_mutex_lock(&p->lock);
        if (!p->close) {
            psock[pos].fd = p->sock;
            p->pos = pos;
            psock[pos++].events = POLLIN | POLLOUT | POLLERR | POLLHUP;
        }
        pthread_mutex_unlock(&p->lock);
        p = p->link;
    }
    nsock = pos;
}

void sock_module::process(struct pollfd *psock) {
    if (listen.pos >= 0 && psock[listen.pos] & POLLIN) {
        int dsock = accept(listen.sock, (struct sockaddr*)0, 0);
        if (dsock < 0) {
            printf ("error!\n");
        }
        else {
            connect_meta* tmp = new connect_meta();
            tmp->sock = dsock;
            tmp->pos = -1;
            tmp->close = 0;
            pthread_mutix_init(&tmp->lock);
            tmp->link = connList;
            connList->link = tmp;
        }
    }
    while (p) {
        
    }
}


