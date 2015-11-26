#include<iostream>
#include<algorithm>
#include"Allocator.h"
#include"Iterator.h"
#include<vector>
#include"Array.h"
#include<array>
#include<string>
#include<list>
using namespace MyCppSTL;


int main()
{
	std::vector<std::string>ivec{"a","b","c","d","e","f"};
	std::vector<std::string>ivec2(10);
	std::vector<std::string>ivec3(10);
	std::vector<std::string>ivec4(10);
	MyCppSTL::uninitialized_fill(ivec2.begin(), ivec2.end(), "b");
	MyCppSTL::uninitialized_copy(ivec2.begin(), ivec2.end(), ivec3.begin());
	MyCppSTL::uninitialized_fill_n(ivec4.begin(), 10, "c");
	for (auto &e : ivec2)
	{
		std::cout << e << std::endl;
	}
	std::cout<<std::endl;

	for (auto &e : ivec3)
	{
		std::cout << e << std::endl;
	}
	std::cout << std::endl;

	for (auto &e : ivec4)
	{
		std::cout << e << std::endl;
	}
	std::cout << std::endl;
	return 0;
}