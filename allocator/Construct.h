#ifndef CONSTRUCT_H
#define CONSTRUCT_H

//#include<traits_type>
/*
*	定义alloc需要使用的construct和destroy函数
*       construct：对象构造
*	destroy：对象析构
*/

#include<new.h>

namespace MyCppSTL
{
	/*construct 对象构造*/
	/*要求T2到T1的类型是可以转换的*/
	template<class T1,class T2>
	inline void construct(T1*p,const T2& value)
	{
		new(p)T1(value);            //调用占位new,在位置p处构造参数value
	}
	/*destroy 对象销毁*/
	template<class T>
	inline void destroy(T*p)
	{
		p->~T();		   //直接调用类型T的析构函数
	}




}
 


#endif
