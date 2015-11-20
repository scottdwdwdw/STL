#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include"Alloc.h"
#include"Construct.h"

/*
*  分配器接口，包装第一二级配置器，提供标准接口
*/

namespace MyCppSTL 
{

	template<class T,class Alloc=malloc_alloc>
	class allocator
	{
	public:
		/*空间的分配与释放*/
		static T*allocate(size_t n)
		{
			return 0>=n?0:static_cast<T*> (Alloc::allocate(n*sizeof(T)));//分配n*sizeof(T)个空间
		}
		static T*allocate(void)
		{
			return static_cast<T*>(Alloc::allocate(sizeof(T)));
		}
		static void deallocate(T*p, size_t n)
		{
			if (n != 0) { Alloc::deallocate(p, n); }
		}
		static void deallocate(T*p)
		{
			 Alloc::deallocate(p,sizeof(T)); 
		}
		/*对象的构造与析构*/
		static void construct(T*p,const T&n)
		{
			MyCppSTL::construct(p, n);
		}
		static void destroy(T*p)
		{
			MyCppSTL::destroy(p);
		}

	};

	

}

#endif