#include<iostream>
#include"Allocator.h"
#include"Iterator.h"
#include<vector>
using namespace MyCppSTL;

//using namespace std;
template<class T>
void test(T&a, T b)
{
	std::cout << "aaa";
}


int main()
{
	
	MyCppSTL::allocator<int>alloc;
	auto q = alloc.allocate(10);
	auto begin = q;
	for (int i = 0; i < 10; ++i)
	{
		alloc.construct(q++, i);
	}
	for (int i = 0; i < 10; ++i)
	{
		std::cout << begin[i] << std::endl;
	}
	std::cout << alloc.address(begin[3]) << std::endl;
	while(q!=begin)
	{
		alloc.destroy(--q);
	}
	alloc.deallocate(begin, 10);

	return 0;
}