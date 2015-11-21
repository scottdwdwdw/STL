#include<iostream>
#include"Allocator.h"

using namespace MyCppSTL;

//using namespace std;



int main()
{
	allocator<int>alloc;
	auto *p = alloc.allocate(10);
	auto begin = p;
	for (int i = 0; i < 10; ++i)
		alloc.construct(p++, i);
	
	auto *q = alloc.allocate(10);
	for (int i = 0; i < 10; ++i)
		alloc.destroy(p--);
	alloc.deallocate(begin, 10);

	return 0;
}