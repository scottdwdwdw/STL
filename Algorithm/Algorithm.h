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
	//返回的是递增后的迭代器
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

	//辅助函数，是pod,直接复制
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, MyCppSTL::__true_type)
	{
		InputIterator i = first;
		for (; i < last; ++i)
		{
			*dest++ = *i;
		}
		return dest;
	}

	//辅助函数，不是pod,调用construct完成copy
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, MyCppSTL::__false_type)
	{
		InputIterator i = first;
		for (; i < last; ++i, ++dest)
		{
			construct(&*(dest), *i);
		}

		return dest;
	}

	//uninitialized_copy的特例化版本,针对，char*
	inline char*uninitialized_copy(const char*first, const char*last, char*dest)
	{
		std::memmove(dest, first, last - first);
		return dest + (last - first);
	}
	//使用move语句
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator dest)
	{
		InputIterator i = first;
		for (; i < last; ++i, ++dest)
		{
			construct(&*(dest), std::move(*i)); //使用移动的方法
		}
		return dest;
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
	inline void __uninitialized_fill_aux(InputIterator first, InputIterator last, const T&val, MyCppSTL::__false_type)
	{
		for (; first < last; ++first)
		{
			construct(&*first, val);
		}
	}

	//uninitialized_fill_n
	//返回的是递增后的first
	template<class InputIterator, class size, class T>
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
	inline InputIterator __uninitialized_fill_n_aux(InputIterator first, size n, const T&val, MyCppSTL::__true_type)
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
	template<class Iterator, class value_type>
	void fill(Iterator first, Iterator last, const value_type&val)
	{

		for (; first != last; ++first)
			*first = val;


	}
	//fill_n
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T&val)
	{
		for (; n > 0; --n, ++first)
		{
			*first = val;
		}
		return first;
	}

	//iter_swap
	template<class ForwardIterator1, class ForwardIterator2>
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
	template<class ForwardIterator1, class ForwardIterator2>
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
	template<class ForwardIterator1, class ForwardIterator2>
	bool lexicographical_compare(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
		{
			if (*first1 < *first2)return true;
			else if (*first1 > *first2)return false;
		}
		//equal or one range is a prefix of another
		return((first1 == last1) && (first2 != last2));
	}


	//拷贝操作
	//完全泛化版本
	//返回的是拷贝后，dest的迭代器(指向最后一个元素的下一个位置)
	template<class InputIter, class OutputIter>
	inline OutputIter copy(InputIter first, InputIter last, OutputIter dest)
	{
		return _copy_dispatch<InputIter,OutputIter>()(first, last, dest);
	}
	//copy特化版本
	template<>
	inline char* copy(const char*first, const char*last, char*dest)
	{
		memmove(dest, first, last - first);
		return dest + (last - first);
	}

	//copy泛化版本的辅助函数
	template<class InputIter,class OutputIter>
	struct _copy_dispatch
	{
		OutputIter operator()(InputIter first, InputIter last, OutputIter dest)
		{
			return _copy(first, last, dest, MyCppSTL::iterator_traits<InputIter>::iterator_category());  //这里可能有bug
		}

	};

	//_copy_dispatch的特化版本
	template<class T>
	struct _copy_dispatch<T*, T*>
	{
		T*operator()(T*first, T*last, T*dest)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_constructor t;
			return _copy_t(first, last, dest, t());
		}
	};
	template<class T>
	struct _copy_dispatch<const T*, T*>
	{
		T*operator()(const T*first, const T*last, T*dest)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_constructor t;
			return _copy_t(first, last, dest, t());
		}
	};

	//_copy_dispatch泛化版本的辅助函数
	//input_iterator_bag版本
	template<class InputIter,class OutputIter>
	inline OutputIter _copy(InputIter first, InputIter last, OutputIter dest, input_iterator_tag)
	{
		for (; first != last; ++first, ++dest)
		{
			*dest = *first;
		}
		return dest;
	}

	//random_access_iterator_tag版本
	template<class InputIter, class OutputIter>
	inline OutputIter _copy(InputIter first, InputIter last, OutputIter dest, random_access_iterator_tag)
	{
		std::ptrdiff_t distance = last - first;
		return _copy_d(first, last, dest, distance);
	}

	//random_access_iterator_tag版本辅助函数
	template<class InputIter, class OutputIter>
	inline OutputIter _copy_d(InputIter first, InputIter last, OutputIter dest, std::ptrdiff_t distance)
	{
		for (; distance > 0; --distance, ++first, ++dest)
		{
			*dest = *first;
		}
		return dest;
	}

	//_copy_dispatch特化版本的辅助函数
	template<class T>
	inline T* _copy_t(const T* first, const T* last, T* dest, __true_type)  //
	{
		memmove(dest, first, sizeof(T)*(last - first));
		return dest + (last - first);
	}

	//_copy_dispatch特化版本的辅助函数
	template<class T>
	inline T* _copy_t(const T* first, const T* last, T* dest, __false_type)  //
	{
		return _copy_d(first, last, dest, last - first);
	}


	//copy_backward方法的实现
	//copy_backward泛化版本
	template<class BidirectionalIterator1,class BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest)
	{
		return _copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, dest);
	}

	//泛化版本，决定是调用random版还是调用bidrection版本
	template<class InputIterator, class OutputIterator>
	struct _copy_backward_dispatch
	{
		typedef typename MyCppSTL::iterator_traits<InputIterator>::iterator_category Cat;
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator dest)
		{
			
			return _copy_backward_aux(first, last, dest, Cat());//分派
		}
	};

	//特化版本,
	template<class T>
	struct _copy_backward_dispatch<T*, T*>
	{
		T* operator()(const T*first, const T*last, T*dest)
		{
			return _copy_backward_t(first, last, dest, __type_traits<T>::has_trivial_assignment_constructor());
		}
		
	};

	template<class T>
	struct _copy_backward_dispatch<const T*, T*>
	{
		T*operator()(const T*first, const T*last, T*dest)
		{
			return _copy_backward_dispatch<T*, T*>()(first, last, dest);
		}
	};






	//bidirectional_iterator 版本，作为最底层的实现
	template<class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 _copy_backward_aux(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest,bidirectional_iterator_tag)
	{
		--last;
		for (; last!=first; --last, --dest)
		{
			*dest = *last;
		}
		return dest;
	}
	//random_access_iterator版本，作为最底层的实现
	template<class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 _copy_backward_aux(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest, random_access_iterator_tag)
	{
		std::ptrdiff_t distance = last - first;
		--last;
		--dest;
		for (; distance > 0; --distance, --last, --dest)
		{
			*dest = *last;
		}
		return dest;
	}


	template<class T>
	inline T*_copy_backward_t(const T*first, const T*last, T*dest, MyCppSTL::__true_type)
	{
		std::ptrdiff_t n = last - first;
		memmove(dest - n, first, sizeof(T)*n);
		return dest - n;
	}

	template<class T>
	inline T*_copy_backward_t(const T*first, const T*last, T*dest, MyCppSTL::__false_type)
	{
		return _copy_backward_aux(first, last, dest, random_access_iterator_tag);
	}

	

	

}


#endif