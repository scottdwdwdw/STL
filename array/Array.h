#ifndef ARRAY_H
#define ARRAY_H

#include "Iterator.h"

namespace MyCppSTL
{
	
	/*array类*/
	template<class T,std::size_t N>
	struct Array    
	{
		//内嵌类型
		typedef T										value_type;
		typedef T* 										pointer;
		typedef const T*								const_pointer;
		typedef T&										reference;
		typedef const T&								const_reference;
		typedef std::ptrdiff_t							difference_type;
		typedef std::size_t								size_type;
		typedef MyCppSTL::array_iterator<T,N>			iterator;		//迭代器类型
		//const_iterator
		// 
		//构造函数
		/*
		*	由于array是聚合类，所以它的构造函数都被隐式的构造 出来
		*/
		//元素获取
		T &at(std::size_t pos) 
		{
			check_pos(pos);
			return _array[pos];
		}
		const T&at(std::size_t pos)const
		{
			check_pos(pos);
			return _array[pos];
		}
		T&operator[](std::size_t pos) { return _array[pos]; }
		const T&operator[](std::size_t pos) const { return _array[pos]; }
		T&front() { return _array[0]; }
		const T&front()const { return _array[0]; }
		T& back() { return _array[N-1]; }
		const T&back() const{ return _array[N - 1]; }
		//array迭代器

		//容量
		const std::size_t size() { return N; }
		const std::size_t size() const { return N; }
		const bool empty() { return N == 0 ? true : false; }
		const bool empty() const { return N == 0 ? true : false; }
		const std::size_t max_size() { return N; }
		const std::size_t max_size() const { return N; }

		//操作
		void fill(const T&val)
		{
			
		}
		//迭代器操作
		iterator begin() { return iterator(_array); }
		const T*begin() const { return _array;  }
		const T*cbegin() const { return _array; }
		iterator end() { return iterator(_array + N); }
		const T*end()const { return _array + N; }
		const T*cend()const { return _array + N; }

		T _array[N];
	private:
		void check_pos(std::size_t pos) 
		{
			if (pos > N)throw std::out_of_range("out of range");
		}
	};


	
}


#endif