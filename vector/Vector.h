#ifndef VECTOR_H
#define VECTOR_H


#include<initializer_list>
#include<type_traits>
#include"Allocator.h"
#include"Algorithm.h"
#include"__Type_traits.h"
#include"Iterator.h"


namespace MyCppSTL{
	
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class vector 
	{
	public:    //内嵌型别定义
		typedef T									value_type;
		typedef T*									pointer;
		typedef const T*							const_pointer;
		typedef T&									reference;
		typedef const T&							const_reference;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef MyCppSTL::vector_iterator<T, alloc>	iterator;
		typedef alloc								alloc_type;
	private:   //成员
		pointer _first;    //容器头
		pointer _last;	   //容器元素尾
		pointer _end_of_storage;  //可用容器尾
	public:	   
		//构造函数
		vector():_first(0), _last(0), _end_of_storage(0) {};	//默认构造函数
		vector(size_type count, const T&val) //接受一个数量和一个数值
		{
			allocate_and_fill(count, val);
		}
	  explicit vector(size_type count)//接受一个数量，采用元素默认的值,不允许隐式类型转换
		{ 
			allocate_and_fill(count, T());
		}			
	     
	    //与上一例二义性
		//template<class InputIt>   
		//vector(InputIt first, InputIt last);					//接受一对迭代器指向的范围
		template<class InputIt>
		vector(InputIt first, InputIt last)
		{
			vector_aux(first, last, std::is_integral<InputIt>::type());
		}
		//辅助函数
		template<class InputIt>
		void vector_aux(InputIt first, InputIt last, std::true_type)
		{
			allocate_and_fill(first, last);
		}
		//辅助函数,这是迭代器
		template<class InputIt>
		void vector_aux(InputIt first, InputIt last, std::false_type)
		{
			//allocate_and_fill(first, last);
		}
		vector(const vector<T>&rhs);							//赋值构造函数
		vector(vector<T>&&rhs);									//移动构造函数
		vector(std::initializer_list<T>&il);							//列表初始化
		

		//成员函数

	private:
		//辅助函数,分配空间并用指定值来初始化
		void allocate_and_fill(size_type n, const T&x)
		{
			_first = alloc::allocate(n);    //分配空间
			pointer cur = _first;
			MyCppSTL::uninitialized_fill_n(cur, n, x);
			_last = _first + n;
			_end_of_storage = _last;
		}
		//辅助函数
	

	
	};





}


#endif 
