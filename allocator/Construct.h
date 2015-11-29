#ifndef CONSTRUCT_H
#define CONSTRUCT_H

//#include<traits_type>
/*
*	定义alloc需要使用的construct和destroy函数
*   construct：对象构造
*	destroy：对象析构
*/

#include<new>
#include "__Type_traits.h"


namespace MyCppSTL
{
	/*construct 对象构造*/
	/*要求T2到T1的类型是可以转换的*/
	template<class T1,class T2>
	inline void construct(T1*p,const T2& value)
	{
		new(p)T1(value);      //调用占位new,在位置p处构造参数value
	}
	/*destroy 对象销毁*/
	template<class T>
	inline void destroy(T*p)
	{
		p->~T();		   //直接调用类型T的析构函数
	}

	
	template<class Forwarditerator>
	inline void destroy(Forwarditerator first, Forwarditerator last)
	{
		__destroy_aux(first, last,MyCppSTL::__type_traits<Forwarditerator>::is_POD_type());
	}
	
	/*
	template<class ForwardIterator,class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last,T*)
	{

	}
	*/
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, MyCppSTL::__true_type)
	{
	}

	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, MyCppSTL::__false_type)
	{
		for (; first < last; ++first)
			destroy(&*first);
	}

	

	//特例化版本
	template<>
	inline void destroy(char*, char*) {}
	template<>
	inline void destroy(int*, int*) {}
	template<>
	inline void destroy(double*, double*) {}




}
 


#endif
