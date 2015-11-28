#ifndef VECTOR_H
#define VECTOR_H


#include<initializer_list>
#include<numeric>
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
		typedef MyCppSTL::vector_const_iterator<T, alloc> const_iterator;
		typedef alloc								alloc_type;
		typedef vector<T, alloc>					myVector;
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
		vector(const vector<T>&rhs)							   //赋值构造函数
		{
			allocate_and_copy(rhs._first,rhs._last);
		}
		vector(vector<T>&&rhs):_first(rhs._first),_last(rhs._last),_end_of_storage(rhs._end_of_storage)//移动构造函数
		{
			rhs._first = rhs._last = rhs._end_of_storage = 0;   //将移后源置于可析构状态
		}
		vector(std::initializer_list<T>il)							//列表初始化
		{
			allocate_and_copy(il.begin(), il.end());
		}
		//析构函数
		~vector()
		{
			free(_first, _last);
		}
		/*******************************************************************/

		//成员函数
		//赋值拷贝
		myVector& operator=(const myVector &rhs)
		{
			if (this != &rhs) //两者不是同一个对象
			{
				free(_first, _last);
				allocate_and_copy(rhs._first, rhs._last);
			}
			return *this;
		}
		//移动赋值拷贝
		myVector operator=(const myVector&&rhs)
		{
			if (*this != &rhs)
			{
				free(_first, _last);
				_first = rhs.begin();
				_last = rhs.end();
				_end_of_storage = rhs._end_of_storage;

				rhs.begin = rhs.end() = rhs._end_of_storage = 0;   //
			}
		}

		//元素获取
		reference at(size_type pos) 
		{
			check(pos);
			return *(_first + pos); 
		}
		const_reference at(size_type pos) const  //常量版
		{
			check(pos);
			return *(_first + pos);
		}
		reference operator[](size_type pos) //下标操作
		{
			return _first[pos];
		}
		const_reference operator[](size_type pos) const //常量版本
		{
			return _first[pos];
		}
		reference front()
		{
			return *begin();
		}
		const_reference front()const   //常量版本
		{
			return *cbegin();     
		}

		reference back()
		{
			return *(end() - 1);
		}
		const_reference back() const
		{
			return *(cend() - 1);
		}

		//迭代器
		const_iterator begin() const{ return const_iterator(_first); }   //常量版本
		const_iterator end() const { return const_iterator(_last); }     //
		const_iterator cbegin() const { return const_iterator(_first); }
		const_iterator cend() const { return const_iterator(_last); }
		iterator begin() { return iterator(_first); }   //非常量版本
		iterator end() { return iterator(_last); }
		//



		//容量(capacity)
		size_type capacity() const   //容量
		{
			return (_end_of_storage - _first);
		}
		size_type size()const //容器中元素大小
		{
			return (_last - _first);
		}
		size_type max_size() const
		{
			return (std::numeric_limits<value_type>::max());
		}
		bool empty() const
		{
			return (_first == _last);
		}
		void reserve(size_type new_cap);
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

	   //辅助函数,用于区分整数类型和迭代器类型
	   template<class InputIt>
	   void vector_aux(InputIt count, InputIt val, std::true_type)//这里用std::true_type是因为用的是库函数的trait
	   {
		   allocate_and_fill(count, val);
	   }
	   //辅助函数,这是迭代器
	   template<class InputIt>
	   void vector_aux(InputIt first, InputIt last, std::false_type)
	   {
		   allocate_and_copy(first, last);
	   }

	   //辅助函数，销毁分配的空间
	   template<class InputIter>
	   void free(InputIter first,InputIter last)
	   {
		   if (first)
		   {
			   destroy(first, last);
			   alloc_type::deallocate(_first, _end_of_storage - _first);
		   }
	   }
	   //辅助函数，边界检查
	   void check(const size_type &pos)
	   {
		   if (pos >= size())
		   {
			   throw std::out_of_range("out of range");
		   }
	   }
	   
	   
	
	};





}


#endif 
