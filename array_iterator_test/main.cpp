#include<iostream>
#include"Allocator.h"
#include"Iterator.h"
#include<vector>
#include"Array.h"
#include<array>
using namespace MyCppSTL;

//using namespace std;
template<class T>
void test(T&a, T b)
{
	std::cout << "aaa";
}




int main()
{
//	std::array<int,5>stda{ 1,2,3 };
	MyCppSTL::Array<int, 5>ia{ 1,2,3,4,5 };
	MyCppSTL::Array<int, 5>::iterator iaa(ia.begin());
	

	for (auto it = ia.begin(); it != ia.end(); ++it)
	{
		std::cout << *it << std::endl;
	}

	std::cout << iaa[3] << std::endl;
	std::cout << (iaa<iaa+3) << std::endl;
	std::cout << (iaa>iaa + 3) << std::endl;
	std::cout << (iaa<=iaa + 3) << std::endl;
	std::cout << (iaa>=iaa + 3) << std::endl;
	std::cout << "test ++ -- : " << " first: " << *iaa << std::endl;

	auto x = ia.end();
	std::cout << *(x-3) << std::endl;
	auto x1 = iaa++;
	std::cout << *x1 << std::endl;
	std::cout << *iaa << std::endl;
	auto x2 = ++iaa;
	std::cout << *x2 << std::endl;
	std::cout << *iaa<< std::endl;
	auto x3 = iaa--;
	std::cout << *x3 << std::endl;
	std::cout << *iaa << std::endl;
	auto x4 = --iaa;
	std::cout << *x4 << std::endl;
	std::cout << *iaa << std::endl;

	//auto x = a.begin();
	return 0;
}