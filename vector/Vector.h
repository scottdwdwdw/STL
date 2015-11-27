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
		//typedef MyCppSTL::vector_iterator<T, alloc>	iterator;
		typedef MyCppSTL::vector_const_iterator<T, alloc> const_iterator;
		typedef alloc								alloc_type;
	private:   //成员
		pointer _first;    //容器头
		pointer _last;	   //容器元素尾
		pointer _end_of_storage;  //可用容器尾
	public:	   
		//构造函数
		vector():_first(0), _last(0), _end_of_storage(0) {};	//默认构造函数
	    explicit vector(size_type count)//接受一个数量，采用元素默认的值,不允许隐式类型转换
		{ 
			allocate_and_fill(count, T());
		}	

	  //接受一对迭代器指向的范围,或者是两个整数
		template<class InputIt>
		vector(InputIt first, InputIt last)
		{
			vector_aux(first, last, std::is_integral<InputIt>::type());
		}
		//辅助函数
		template<class InputIt>
		void vector_aux(InputIt count, InputIt val, std::true_type)//这里用std::true_type是因为用的是库函数的trait
		{
			allocate_and_fill(count, val);
		}
		//辅助函数,这是迭代器
		template<class InputIt>
		void vector_aux(InputIt first, InputIt last, std::false_type)
		{
			/*
			_first = alloc::allocate(last-first);    //分配空间
			pointer cur = _first;
			uninitialized_copy(first, last, cur);
			_last = _end_of_storage = _first + (last - first);  //必须要加括号，因为类型不同
			*/
			allocate_and_copy(first, last);
		}
		vector(const vector<T>&rhs)							   //赋值构造函数
		{
			/*
			_first = alloc::allocate(rhs._last - rhs._first);	//分配空间
			pointer cur = _first;
			uninitialized_copy(rhs._first, rhs._last, cur);
			_last = _end_of_storage = _first + (rhs._last - rhs._first);
			*/
			allocate_and_copy(rhs.begin(),rhs.end());
		}
		vector(vector<T>&&rhs):_first(rhs._first),_last(rhs._last),_end_of_storage(rhs._end_of_storage)//移动构造函数
		{
			rhs._first = rhs._last = rhs._end_of_storage = 0;   //将移后源置于可析构状态
		}
		vector(std::initializer_list<T>il)							//列表初始化
		{
			/*
			_first= alloc::allocate(il.end()-il.begin());
			pointer cur = _first;
			uninitialized_copy(il.begin(), il.end(), cur);
			_last = _end_of_storage = _first+(il.size());
			*/
			allocate_and_copy(il.begin(), il.end());
		}

		

		//成员函数
		const_iterator begin() const{ return const_iterator(_first); }   //常量版本
		const_iterator end() const { return const_iterator(_last); }     //


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
		
	   template<class InputIterator>
	   void allocate_and_copy(InputIterator first, InputIterator last)
	   {
		   _first = alloc::allocate(last-first);
		   pointer cur = _first;
		   uninitialized_copy(first, last, cur);
		   _last = _end_of_storage = _first + (last-first);
	   }
	   
	
	};





}


#endif 
