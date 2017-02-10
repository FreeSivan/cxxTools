/**
 * author : yue sivan
 */
#ifndef _SOCK_SERVICE_H_
#define _SOCK_SERVICE_H_
#include "sockModule.h"

class sock_service {
public:
    sock_service();
    ~sock_service();
public:
    bool addModule(sock_module* module);
    bool delModule(sock_module* module);
    void mainloop();
private:
    sock_module* moduleList;
    struct pollfd* pollArr;
    int possArrSize;
private:
    sock_service(const sock_service&);
    sock_service& operator=(const sock_service&);
};

#endif

