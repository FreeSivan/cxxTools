#include "ys_sockService.h"

namespace ys {

sock_service::sock_service() {
    sock_module* moduleList = 0;
    pollArr = new struct pollfd[MAX_SIZE];
    pollArrSize = 0;
}

sock_service::~sock_service() {

}

bool sock_service::addModule(sock_module * module) {
    module->link = moduleList;
    moduleList = module;
    return true;
}

bool sock_service::delModule(sock_module* module) {
    if (!module) {
        return false;
    }
    sock_module *cusor = moduleList;
    while(cusor) {
        if (cusor->link == module) {
            cusor->link = module->link;
            delete module;
            return true;
        }
        cusor = cusor->link;
    }
    return false;;
}

void sock_service::mainloop() {
    sock_module *cusor;
    while (1) {
        cusor = moduleList;
        while (cusor) {
            cusor->prepare(psock, pollArrSize);
            cusor = cusor->link;
        }
        poll(pollArr, pollArrSize, 100);
        cusor = moduleList;
        while (cusor) {
            cusor->process(psock);
        }
    }
}

}

