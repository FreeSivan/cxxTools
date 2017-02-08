#include "ys_skipList.h"
#include <iostream>
using namespace std;

int main() {
	skList<int, int> sk(4);
    sk.addTerm(1, 4);
	sk.addTerm(39, 3);
	sk.addTerm(11, 3);
	sk.addTerm(11, 3);
	sk.addTerm(123, 6);
	sk.addTerm(80, 6);
	sk.addTerm(17, 6);
	sk.addTerm(43, 6);
	sk.addTerm(65, 6);
	sk.addTerm(21, 6);
	sk.addTerm(6, 2);
	sk.addTerm(21, 4);
	sk.addTerm(64, 4);
	sk.addTerm(17, 4);
	sk.addTerm(19, 4);
	sk.addTerm(7, 4);
	sk.addTerm(41, 4);
	sk.addTerm(37, 4);
	sk.addTerm(57, 4);
	sk.addTerm(40, 4);
	sk.addTerm(29, 4);
	sk.addTerm(54, 4);
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
