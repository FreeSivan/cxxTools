/**
 * author : yue sivan
 */
#ifndef _SOCK_MODULE_H_
#define _SOCK_MODULE_H_
#include "sockdata.h"

namespace ys {

class sock_module {
public:
    sock_module(char* host, int port);
    virtual ~sock_module();
public:
    virtual void prepare(struct pollfd *psock, int& nsock);
    virtual void process(struct pollfd *psock);
private:
    listen_meta listen;
    connect_meta* connList;
    sock_module* link;
    threadPool executePool;
private:
    sock_module(const sock_module&);
    sock_module& operator=(const sock_module&);
    void processL(struct pollfd *psock);
    void processC(struct pollfd *psodk);
};

}

#endif

