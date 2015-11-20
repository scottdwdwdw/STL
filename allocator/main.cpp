#include<iostream>
#include"Allocator.h"


using namespace MyCppSTL;



int main()
{
	allocator<int>alloc;
	
	auto a=alloc.allocate(10);
	auto p = a;
	auto p2 = a;
	for (size_t i = 0; i < 10; ++i)
		alloc.construct(a++, 10);
	for (size_t i = 0; i < 10; ++i)
		alloc.destroy(p++);
	alloc.deallocate(p2,10);
	return 0;
}