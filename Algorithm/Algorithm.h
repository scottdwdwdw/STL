#ifndef ALGORITHM_H
#define ALGORITHM_H


#include"__Type_traits.h"
#include"Iterator.h"

namespace MyCppSTL
{
	//fill
	template<class Iterator,class value_type>
	void fill(Iterator first, Iterator last, const value_type&val)
	{
		
		for (; first != last; ++first)
			*first = val;
	
		
	}

	//ÌØÀý»¯°æ±¾
	/*
	void __fill(const char*first, const char*second, const char&val,char)
	{
		std::ptrdiff_t size = second - first;
		memset((void*)first, size, val);
	}
	*/
	//fill_n
	template<class Iterator,class Size,class value_type>
	void fill_n(Iterator first,Size n , const value_type&val)
	{
		for (; n > 0; --n,++first)
		{
			*first = val;
		}
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