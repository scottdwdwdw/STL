#ifndef ALGORITHM_H
#define ALGORITHM_H


#include"__Type_traits.h"
#include"Iterator.h"
//#include"Allocator.h"
#include"Construct.h"
namespace MyCppSTL
{
	//uninitialized_   
	//uninitialized_copy()
	//要求目的地址是待初始化的一块内存，且容量至少有输入范围那么大
	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest)
	{
		return (__uninitialized_copy(first, last, dest));
	}

	//辅助函数
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest)
	{
		typedef MyCppSTL::iterator_traits<InputIterator>::value_type value_type;
		return (__uninitialized_copy_aux(first, last, dest, MyCppSTL::__type_traits<value_type>::is_POD_type()));
	}

	//辅助函数，是pod,调用std的copy
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, MyCppSTL::__true_type)
	{
		return (std::copy(first, last, dest));
	}

	//辅助函数，不是pod,调用construct完成copy
	template<class InputIterator,class ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest,MyCppSTL::__false_type)
	{
		InputIterator i = first;
		for (; i<last; ++i)
		{
			construct(&*(dest + (i - first)), *i);
		}

		return first;
	}

	//uninitialized_copy的特例化版本,针对，char*
	inline char*uninitialized_copy(const char*first, const char*last, char*dest)
	{
		std::memmove(dest, first, last - first);
		return dest + (last - first);
	}


	//uninitialized_fill()
	//要求[first,last)要求是未初始化的内存
	template<class InputIterator, class T>
	void uninitialized_fill(InputIterator first, InputIterator last, const T&val)
	{
		__uninitialized_fill(first, last, val);
	}

	//辅助函数，根据元素类型选择不同的重载函数
	template<class InputIterator, class T>
	inline void __uninitialized_fill(InputIterator first, InputIterator last, const T&val)
	{
		__uninitialized_fill_aux(first, last, val, MyCppSTL::__type_traits<T>::is_POD_type());
	}

	template<class InputIterator, class T>
	inline void __uninitialized_fill_aux(InputIterator first, InputIterator last, const T&val, MyCppSTL::__true_type)
	{
		MyCppSTL::fill(first, last, val);
	}
	//uninitialized_fill辅助函数
	//不是pod类型,调用construct构造
	template<class InputIterator, class T>
	inline void __uninitialized_fill_aux(InputIterator first, InputIterator last, const T&val,MyCppSTL::__false_type)
	{
		for (; first < last; ++first)
		{
			construct(&*first, val);
		}
	}

	//uninitialized_fill_n
	//
	template<class InputIterator,class size, class T>
	InputIterator uninitialized_fill_n(InputIterator first, size n, const T&val)
	{
		return (__uninitialized_fill_n(first, n, val));
	}
	//辅助函数 is_POD_type或不是
	template<class InputIterator, class size, class T>
	inline InputIterator __uninitialized_fill_n(InputIterator first, size n, const T&val)
	{
		return (__uninitialized_fill_n_aux(first, n, val, MyCppSTL::__type_traits<T>::is_POD_type()));
	}

	//辅助函数，是POD，直接调用上层函数完成
	template<class InputIterator, class size, class T>
	inline InputIterator __uninitialized_fill_n_aux(InputIterator first, size n, const T&val,MyCppSTL:: __true_type)
	{
		return (MyCppSTL::fill_n(first, n, val));
	}
	//辅助函数，不是pod,调用construct来完成
	template<class InputIterator, class size, class T>
	inline InputIterator __uninitialized_fill_n_aux(InputIterator first, size n, const T&val, MyCppSTL::__false_type)
	{
		
		for (; n > 0; ++first, --n)
		{
			construct(&*first, val);
		}
		return first;
	}

	
	//fill
	template<class Iterator,class value_type>
	void fill(Iterator first, Iterator last, const value_type&val)
	{
		
		for (; first != last; ++first)
			*first = val;
	
		
	}
	//fill_n
	template<class OutputIterator,class Size,class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T&val)
	{
		for (; n > 0; --n, ++first)
		{
			*first = val;
		}
		return first;
	}

	//iter_swap
	template<class ForwardIterator1,class ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
	{
		MyCppSTL::iterator_traits<ForwardIterator1>::value_type tmp = *a;
		*a = *b;
		*b = tmp;
	}
	//swap
	template<class T>
	inline void swap(T&a, T&b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	//equal
	template<class ForwardIterator1,class ForwardIterator2>
	inline bool equal(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 first2)
	{
		for (; first != last; ++first, ++first2)
		{
			if (*first != *first2)return false;
		}
		return true;
	}
	//lexicographical_compare
	//true if the first range is lexicographically less than the second.
	template<class ForwardIterator1,class ForwardIterator2>
	bool lexicographical_compare(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		for (; (first1 < last1) && (first2 < last2); ++first1, ++first2)
		{
			if (*first1 < *first2)return true;
			else if (*first1 > *first2)return false;
		}
		//equal or one range is a prefix of another
		return((first1 == last1) && (first1 != last1));
	}

}


#endif