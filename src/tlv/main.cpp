#include <typeinfo>
#include <iostream>
#include "tlvValue.h"
#include "tlvTool.h"
#include "tlvDef.h"
#include "tlvCode.h"
using namespace std;
int main()
{
	TlvCode<testType> testobj;
	
	testobj.Set<TAG_NUM_ONE>()   = 1;
	testobj.Set<TAG_NUM_TWO>()   = 2;
	testobj.Set<TAG_NUM_THREE>() = 3;
	testobj.Set<TAG_NUM_FOUR>()  = 4;
	testobj.Set<TAG_NUM_FIVE>()  = 5;
	testobj.Set<TAG_NUM_SIX>()   = 6;
	testobj.Set<TAG_NUM_SEVEN>() = 7;
	testobj.Set<TAG_NUM_EIGHT>() = 8;
	testobj.Set<TAG_NUM_NINE>()  = 9;
	testobj.Set<TAG_NUM_TEN>()   = 10;
	
	char buffer[1024];
	int length = 0;
	
	testobj.Encode(buffer,1024,length);
	cout<<"length == "<<length<<endl;
	for(int i = 0; i < length; ++i)
	{
		if(i % 16 == 0) cout<<endl;
		cout<<(int)buffer[i]<<" ";
	}
	cout<<endl;
	TlvCode<testType> testobj1;
	testobj1.Decode(buffer,length);
	
	cout<<"TAG_NUM_ONE == "<<(int)testobj1.Get<TAG_NUM_ONE>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_ONE>()).name()<<endl;
	cout<<"TAG_NUM_TWO == "<<(int)testobj1.Get<TAG_NUM_TWO>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_TWO>()).name()<<endl;
	cout<<"TAG_NUM_THREE == "<<(int)testobj1.Get<TAG_NUM_THREE>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_THREE>()).name()<<endl;
	cout<<"TAG_NUM_FOUR == "<<(int)testobj1.Get<TAG_NUM_FOUR>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_FOUR>()).name()<<endl;
	cout<<"TAG_NUM_FIVE == "<<(int)testobj1.Get<TAG_NUM_FIVE>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_FIVE>()).name()<<endl;
	cout<<"TAG_NUM_SIX == "<<(int)testobj1.Get<TAG_NUM_SIX>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_SIX>()).name()<<endl;
	cout<<"TAG_NUM_SEVEN == "<<(int)testobj1.Get<TAG_NUM_SEVEN>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_SEVEN>()).name()<<endl;
	cout<<"TAG_NUM_EIGHT == "<<(int)testobj1.Get<TAG_NUM_EIGHT>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_EIGHT>()).name()<<endl;
	cout<<"TAG_NUM_NINE == "<<(int)testobj1.Get<TAG_NUM_NINE>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_NINE>()).name()<<endl;
	cout<<"TAG_NUM_TEN == "<<(int)testobj1.Get<TAG_NUM_TEN>()
		<<"   type == "<<typeid(testobj1.Get<TAG_NUM_TEN>()).name()<<endl;
		
	system("PAUSE");
	return 0;
}
