skipList 跳表，多层索引的链表，每个节点包含一个key-value组合

提供方法：

bool getValue(K key, V& val)：通过key获取value，返回true：key值存在， false：key值不存在

bool addTerm(K key, V val); 向跳表中插入key-value对，true：插入成功，false：插入失败

bool delTerm(K key); 删除key值，true：删除成功， false：删除失败
