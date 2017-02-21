#include "ys_sockService.h"

namespace ys {

sock_service::sock_service() {
    sock_module* moduleList = 0;
    pollArr = new struct pollfd[MAX_SIZE];
    pollArrSize = 0;
}

sock_service::~sock_service() {
	sock_module *tmp = moduleList;
	while(moduleList) {
		sock_module *tmp = moduleList;
		moduleList = moduleList->link;
		delete tmp;
	}
	delete[] pollArr;
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
    return false;
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

using namespace ys;

int main() {
	sock_module* module1 = new sock_module("127.0.0.1", 5001);
	sock_module* module2 = new sock_module("127.0.0.1", 5002);
	sock_module* module3 = new sock_module("127.0.0.1", 5003);
	sock_service service;
	service.addModule(module1);
	service.addModule(module2);
	service.addModule(module3);
	service.mainloop();
}

