#include<iostream>
#include"Allocator.h"
#include"Iterator.h"
#include<vector>
using namespace MyCppSTL;

//using namespace std;
struct B {};
struct D1 :public B {};
struct D2 :public D1 {};

template<class T>
struct Mys
{
	typedef T value_type;
	int a;
};

template<class T>
typename T::value_type func2(T&x, T&y)
{
	return x + y;
}

template<class T>
struct iterator_trait 
{
	typedef typename T::value_type value_type;
};




int main()
{
	std::vector<int>a{ 1,2,3,4,5,6,7 };
	char b[] = { 'a','b' };
	auto x = MyCppSTL::begin(a);
	auto y = MyCppSTL::begin(b);
	auto z = MyCppSTL::end(b);
	auto zz = MyCppSTL::prev(z);
	std::cout << *zz;
	return 0;
}