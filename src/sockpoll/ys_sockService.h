/**
 * author : yue sivan
 */
#ifndef _SOCK_SERVICE_H_
#define _SOCK_SERVICE_H_
#include "sockModule.h"

namespace ys {

#define MAX_SIZE 5000

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
    int pollArrSize;
private:
    sock_service(const sock_service&);
    sock_service& operator=(const sock_service&);
};

}
#endif

