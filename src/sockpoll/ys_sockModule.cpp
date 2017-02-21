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

namespace ys {

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
    link = 0;
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
        if (p->close) {
            p = p->link;
            continue;
        }
        psock[pos].fd = p->sock;
        p->pos = pos;
        psock[pos].events = POLLERR | POLLHUP;
        pthread_mutex_lock(&p->lock);
        if (0 == p->getState()) {
            psock[pos].events |= POLLIN;
        }
        if (4 = p->getState()) {
            psock[pos].events |= POLLOUT;
        }
        pos ++;        
		p = p->link;
    }
    nsock = pos;
}

void sock_module::process(struct pollfd *psock) {
    processL(psock);
    processC(psock);
}

void sock_module::processL(struct pollfd * psock) {
    if (listen.pos < 0 || !(psock[listen.pos] & POLLIN)) {
        return;
    }
    int dsock = accept(listen.sock, (struct sockaddr*)0, 0);
    if (dsock < 0) {
        printf ("error!\n");
    }
    else {
        connect_meta* tmp = new connect_meta();
        tmp->sock = dsock;
        tmp->pos = -1;
        tmp->close = 0;
        tmp->state = 0;
        tmp->setStateCanRead();
        tmp->setStateCanWrite();
        tmp->link = connList;
        connList->link = tmp;
    }
}

void sock_module::processC(struct pollfd * psodk) {
    connect_meta *cusor = connList;
    while (cusor) {
        if (cusor->pos < 0) {
            cusor = cusor->link;
            continue;
        }
        if (psodk[cusor->pos].revents & POLLERR ||
            psodk[cusor->pos].revents & POLLHUP) {
            close (cusor->sock);
            cusor->close = 1;
        }
        if (psodk[cusor->pos].revents & POLLIN && 0 == cusor->getState()) {
            cusor->setState(1);
            readEvent event = new readEvent();
            event->args = (void*)cusor;
            executePool.tpAddEvent(event);
        }
        if (psodk[cusor->pos].revents & POLLOUT && 4 == cusor->getState()) {
            cusor->setState(5);
            writeEvent event = new writeEvent();
            event->args = (void*)cusor;
            executePool.tpAddEvent(event);
        }
        cusor = cusor->link;
    }

    connect_meta **delcusor = connList;
    while (*delcusor) {
        if (1 == (*delcusor)->close) {
            connect_meta *tmp = *delcusor;
            *delcusor = (*delcusor)->link;
            delete tmp;
        }
        else {
            delcusor = &(*delcusor)->link;
        }
    }
}

}


