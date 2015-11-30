#ifndef VECTOR_H
#define VECTOR_H


#include<initializer_list>
#include<numeric>
#include<type_traits>
#include"Allocator.h"
#include"Algorithm.h"
#include"__Type_traits.h"
#include"Iterator.h"
#include<utility>
#include<new>


namespace MyCppSTL{
	
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class vector
	{
	public:    //内嵌型别定义
		typedef T									value_type;

		typedef T&									reference;
		typedef const T&							const_reference;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef MyCppSTL::vector_iterator<T, alloc>	iterator;
		typedef MyCppSTL::vector_const_iterator<T, alloc> const_iterator;
		typedef alloc								alloc_type;
		typedef vector<T, alloc>					myVector;
		typedef T*									pointer;
		typedef const T*							const_pointer;
	private:   //成员
		pointer _first;    //容器头
		pointer _last;	   //容器元素尾
		pointer _end_of_storage;  //可用容器尾
	public:
		//构造函数
		vector() :_first(0), _last(0), _end_of_storage(0) {};	//默认构造函数
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
			allocate_and_copy(rhs._first, rhs._last);
		}
		vector(vector<T>&&rhs) :_first(rhs._first), _last(rhs._last), _end_of_storage(rhs._end_of_storage)//移动构造函数
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
		const_iterator begin() const { return const_iterator(_first); }   //常量版本
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
		void reserve(size_type new_cap)//改变capacity,若new_cap小于当前cap,则不变
		{
			if (new_cap > capacity()) //重新分配空间，并把旧数据拷贝到新的地址
			{
				pointer new_begin= alloc::allocate(new_cap);
				auto oldsize = size();
				pointer cur = new_begin;
				uninitialized_move(_first, _last, cur);
				free(_first, _last);
				_first = new_begin;
				_last = _first + oldsize;
				_end_of_storage = _first + new_cap;
				
			}
		}

		//对容器元素的添加，修改
		void push_back(const T&value)
		{
			if (_last != _end_of_storage)//不需要重新分配空间，直接再尾部添加元素
			{
				construct(_last, value);
				++_last;    //后移
			}
			else   //否则重新分配空间，并把旧的元素拷贝到新的空间中,再销毁旧空间
			{
				const size_type old_size = size();
				const size_type new_size = ((size() == 0) ? 1 : 2 * size());  //按照两倍的关系分配
				pointer new_begin = alloc::allocate(new_size);      //重新分配空间
				pointer cur = new_begin;
				MyCppSTL::uninitialized_copy(_first, _last, cur);   //拷贝到新的空间中
				free(_first, _last);              //销毁旧空间
				_first = new_begin;				  //赋给新空间首地址
				_last = _first + old_size + 1;    //新结束地址
				construct(_last - 1, value);        //添加元素
				_end_of_storage = _first + new_size;
			}
		}


		void push_back(T&&value)
		{
			if (_last != _end_of_storage)
			{
				construct(_last, std::move(value));//移动构造
				++_last;
			}
			else   //重新分配空间
			{
				//const size_type old_size = size();
				const size_type new_size = ((size() == 0) ? 1 : 2 * size());  //按照两倍的关系分配
				reserve(new_size);
				construct(_last, value);        //添加元素
				++_last;
				//_end_of_storage = _first + new_size;
			}
		}

		void pop_back()
		{
			if (empty())
			{
				throw std::runtime_error("empty vector");
			}
			destroy(_last - 1);
			_last -= 1;
		}

		iterator erase(iterator pos)   //移除指定位置的元素
		{
			if (pos > end() || pos < begin())
			{
				throw std::out_of_range("out of range");
			}
			for (auto it = pos; it < end()-1; ++it)
			{
				*it = *(it + 1);
			}
			destroy(_last - 1);
			_last -= 1;
			return pos;
		}

		iterator erase(iterator first, iterator last) //移除指定区间
		{
			difference_type n = last - first;
			auto pos = first;
			for (; last < end(); )
			{
				*first++ = *last++;
			}
			destroy(first, end());
			_last -= (size_type)n;
			return pos;
		}

		void clear()
		{
			erase(begin(), end());
		}

		void resize(size_type count, const T &value)
		{
			if (count < size())
			{
				erase(begin() + count, end());
			}
			else
			{
				insert(end(), count-size(), value);
			}
		}

		void resize(size_type count)
		{
			resize(count, T());
		}

		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args) //pos之前直接构造元素
		{
			auto pos_ptr = _first + (pos - begin());
			if ((size() + 1) <= capacity())//空间足够
			{
				
				for (auto it =_last - 1; it >= pos_ptr; --it)
				{
					construct(it + 1, *it);
				}
				construct(pos_ptr, std::forward<Args>(args)...);
				++_last;
				return iterator(pos_ptr);
			}
			else
			{
				const size_type new_size = ((size() == 0) ? 1 : 2 * size());  //按照两倍的关系分配
				auto count = pos_ptr - _first;
				reserve(new_size);
				pos_ptr = _first + count;
				for (auto it = _last - 1; it >= pos_ptr; --it)
				{
					construct(it + 1, *it);
				}
				construct(pos_ptr, std::forward<Args>(args)...);
				++_last;
				return iterator(pos_ptr);
			}



		}

		template< class... Args >
		void emplace_back(Args&&... args)  //在末尾构造元素
		{
			emplace(end(), args...);
		}


		iterator insert(iterator pos, const T&val)
		{
			/*首先判断增加了这个元素后会不会超出capacity*/
			if (pos > end())
			{
				throw std::out_of_range("out of range");
			}
			if ((size() + 1) <= capacity())
			{
				//先让用最有一个元素往后构造一个新的元素，然后其余的就单纯的移动
				construct(_last, *(_last - 1));
				++_last;
				for (auto it = pos+1; it < _last - 1; ++it)
				{
					*it = *(it - 1);
				}
				*pos = val;
				return pos;
			}
			else  //空间不够，需要重新分配空间
			{
				difference_type distance = pos - begin();
				const size_type new_size = ((size() == 0) ? 1 : 2 * size());  //按照两倍的关系分配
				reserve(new_size);
				auto new_pos = _first + distance;//恢复位置
				insert(new_pos, val);  //递归调用
				return new_pos;
			}
		}

		template< class InputIt >
		void insert(iterator pos, InputIt first, InputIt last)//接受迭代器指定的范围,要与接受两个整数的区别
		{
			insert_aux(pos, first, last, std::is_integral<InputIt>::type());
		}

		
		template<class T>
		void insert(iterator pos, size_type count, const T& value)//接受迭代器指定的范围,要与接受两个整数的区别
		{
			insert_aux(pos, count, value, std::is_integral<T>::type());
		}
		

		//


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

	   //辅助函数，这是整数
	   template<class InputIt>
	   void insert_aux(iterator pos, InputIt count, const value_type&value,std::true_type)//插入count个value
	   {
		   if (pos > end())
		   {
			   throw std::out_of_range("out of range");
		   }
		   if ((size() + count) <= capacity())
		   {
			   difference_type distance = end() - pos;
			   uninitialized_fill_n(_last, count, *(_last - 1));//先构造出count个空间
			   if (((size_type)distance)<=((size_type)count))  //插入元素的数量count大于pos到_last元素的数量
			   {
				   for (auto it = pos; it < _last; ++it) //移动元素
				   {
					   *(it + count) = *it;
				   }
			   }
			   else //数量小于distance，那么先移动尾部的元素，在移动pos之后的元素
			   {
				   for (auto it2 = end()-1; it2>pos-1; --it2)
				   {
					   *(it2 + count) = *it2;
				   }
			       for (auto it = pos; it < pos+count; ++it) //移动元素
				   {
						   *(it + count) = *it;
				    }
				
			   }
			   //  //这样的设计是不是很不好？
			   for (size_type n = 0; n<((size_type)count); ++n)//插入元素
			   {
				   *pos++ = value;
			   }
			   _last += count;

		   }
		   else  //空间不够
		   {
			   difference_type distance = pos - begin();
			   const size_type new_size = ((size() == 0) ? 1 : (2 * (size() + count)));  //按照两倍的关系分配
			   reserve(new_size);
			   auto new_pos = _first + distance;//恢复位置
			   insert(new_pos, count, value);  //递归调用

		   }
	   }
	   //insert的辅助函数，是迭代器
	   template< class InputIt >
	   void insert_aux(iterator pos, InputIt first, InputIt last,std::false_type)//接受迭代器指定的范围,要与接受两个整数的区别
	   {
		   if ((pos > end()) || (last<first))
		   {
			   throw std::out_of_range("out of range");
		   }
		   difference_type distance = end() - pos;
		   difference_type count = last - first;//要插入元素的数量
		   if (((size_type)count + size()) <= capacity())//有空间，插入
		   {
			   uninitialized_fill_n(_last, count, *(_last - 1));//先构造出count个空间
			   if (((size_type)distance) <= ((size_type)count))  //插入元素的数量count大于pos到_last元素的数量
			   {
				   for (auto it = pos; it < _last; ++it) //移动元素
				   {
					   *(it + count) = *it;
				   }
			   }
			   else //数量小于distance，那么先移动尾部的元素，在移动pos之后的元素
			   {
				   for (auto it2 = end() - 1; it2>pos - 1; --it2)
				   {
					   *(it2 + count) = *it2;
				   }
				   for (auto it = pos; it < pos + count; ++it) //移动元素
				   {
					   *(it + count) = *it;
				   }

			   }
			   for (size_type n = 0; n<((size_type)count); ++n)//插入元素
			   {
				   *pos++ = *first++;
			   }
			   _last += count;
		   }
		   else//空间不够
		   {
			   difference_type distance = pos - begin();
			   const size_type new_size = ((size() == 0) ? 1 : (2 * (size() + count)));  //按照两倍的关系分配
			   reserve(new_size);
			   auto new_pos = _first + distance;//恢复位置
			   insert(new_pos, first, last);  //递归调用
		   }
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

	//非成员函数，比较操作
	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator==(const vector<T>&lhs, const vector<T>&rhs)
	{
		if (lhs.size() == rhs.size())
		{
			return (equal(lhs.begin(), lhs.end(), rhs.begin()));
		}

		return false;
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator!=(const vector<T>&lhs, const vector<T>&rhs)
	{
		return !(lhs == rhs);
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator<(const vector<T>&lhs, const vector<T>&rhs)
	{
		return(MyCppSTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator>(const vector<T>&lhs, const vector<T>&rhs)
	{
		return (rhs < lhs);
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator<=(const vector<T>&lhs, const vector<T>&rhs)
	{
		return !(lhs>rhs);
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator>=(const vector<T>&lhs, const vector<T>&rhs)
	{
		return !(lhs < rhs);
	}

}


#endif 
