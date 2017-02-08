#include "skipList.h"
#include <iostream>
using namespace std;

int main() {
	skList<int, int> sk;
	
    cout<<"1111"<<endl;
    sk.addTerm(1, 4);
	sk.addTerm(32, 3);
	sk.addTerm(11, 3);
	sk.addTerm(123, 6);
	sk.addTerm(6, 2);
	sk.addTerm(21, 4);
	sk.display();

	return 0;
}
