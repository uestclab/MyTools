#include "smallobject_allocator.h"
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

class smallobject_allocator;


class small_alloc
{
	public:
	static void* operator new(size_t size)
	{
		return smallobject_allocator::instance().allocate(size);
	}
	
	static void operator delete(void* p,size_t size)
	{
		return smallobject_allocator::instance().deallocate(p,size);
	}
};

class test:public small_alloc
{
	public:
	void testfunc()
	{
		for(int i=0;i<1000000;i++)
		{
			
			void *pInt =(void *) small_alloc::operator new(9);
			small_alloc::operator delete(pInt,9);
			pInt =(void *) small_alloc::operator new(52);
			small_alloc::operator delete(pInt,52);
			pInt =(void *) small_alloc::operator new(24);
			small_alloc::operator delete(pInt,24);
			/*
			char *pInt =  new char[9];
			delete pInt;
			pInt =  new char[52];
			delete pInt;
			pInt =  new char[24];
			delete pInt;
	*/
		}
	}
	
};

int main()
{

	long long start;
	timeval tv;
	gettimeofday(&tv,NULL);
	start = tv.tv_sec*1000*1000 + tv.tv_usec;
	test myTest;
	
	myTest.testfunc();
	gettimeofday(&tv,NULL);
	long long duration = tv.tv_sec*1000*1000 + tv.tv_usec - start;
	cout << "duration = " << duration << endl;
	return 1;
}
