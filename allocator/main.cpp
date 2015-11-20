#include<iostream>
#include"Allocator.h"


using namespace MyCppSTL;



int main()
{
	allocator<int>alloc;
	auto a=alloc.allocate(10);
	alloc.construct(a, 10.9);
	std::cout << *a << std::endl;
	return 0;
}