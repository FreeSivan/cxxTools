mt_queue是一个多线程队列

提供方法：

bool put(mt_node<T> *val); 往队列尾部加入一个节点

mt_node<T>* get();  从队列头部取出一个节点

void setsize(int size); 修改队列长度

void setflag(int flag); 修改标识符，如果置为0，则队列不可用，无法向队列中添加和获取节点

void releaseFull(); 广播满等待信号，释放所有因队列满无法添加节点而等待的线程

void releaseFree(); 广播空等待信号，释放所有因队列空无法获取节点而等待的线程

void resetqueue(); 重置队列

