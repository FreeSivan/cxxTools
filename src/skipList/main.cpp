#include "skipList.h"
#include <iostream>
using namespace std;

int main() {
	skList<int, int> sk;
    sk.addTerm(1, 4);
	sk.addTerm(32, 3);
	sk.addTerm(11, 3);
	sk.addTerm(11, 3);
	sk.addTerm(123, 6);
	sk.addTerm(6, 2);
	sk.addTerm(21, 4);
	sk.display();

    int a;
    sk.getValue(32, a);
    cout<<"a = "<<a<<endl;

    sk.delTerm(32);
    sk.display();

    sk.delTerm(6);
    sk.display();

    sk.delTerm(11);
    sk.display();

	return 0;
}
