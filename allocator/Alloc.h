#ifndef ALLOC_H
#define ALLOC_H

/*
*	Alloc 负责对象构造前的内存分配、对象析构后的内存回收
*	allocate:     内存分配
*	deallocate:   内存释放
*/



#if 0
#	include<new>
#	define __THROW_BAD_ALLOC throw bad_alloc      //不抛出bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#	include<iostream>
#	define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl;exit(1); //如果遇到不能分配且没有指定处理函数
																	    // 的情况，打印信息并退出
#endif

namespace MyCppSTL
{
	 //第一级配置器
	template<int inst>
	class __malloc_alloc_template 
	{
	private:
		/* oom: out of memory */
		static void *oom_malloc(size_t);					//out of memory when malloc
		static void *oom_realloc(void*, size_t);				//out of memory when reallocate
		static void(*__malloc_alloc_oom_handler)();			//the handler of out of memory
	public:
		static inline void* allocate(size_t n);            //allocate memory
		static inline void* reallocate(void*p, size_t n);   //reallocate memory
		static inline void deallocate(void*p,size_t);				//free memory
		static void(*set_malloc_handler(void(*)()))();	    //out of memory function
	};

	/*分配空间成员*/
	template<int inst>
	void* __malloc_alloc_template<inst>::allocate(size_t n)
	{
		auto*result = malloc(n);
		if (result == 0)result = oom_malloc(n);
		return result;
	}
	
	/*再分配*/
	template<int inst>
	void* __malloc_alloc_template<inst>::reallocate(void*p, size_t n)
	{
		auto *result = realloc(p, n);
		if (result == 0)result = oom_realloc(p, n);
		return result;
	}

	/*释放内存*/
	template<int inst>
	void __malloc_alloc_template<inst>::deallocate(void*p, size_t n)
	{
		free(p);
	}

	/*指定分配失败时的处理函数*/
	template<int inst>
	void(*__malloc_alloc_template<inst>::set_malloc_handler(void(*f)()))()
	{
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);			//用old保存之前的处理方法，调用该函数后，实际使用的是__malloc_alloc_oom_handler指定的函数					
	}

	

	/*out of memory 处理函数,函数指针设为0*/
	template<int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;



	/*out of memory */
	template<int inst>
	void *__malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();				//声明一个处理函数
		void*result;
		for (;;)								//反复分配
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();             //调用处理函数
			result = malloc(n);
			if (result)return(result);			//分配成功返回
		}
	}

	/*out of memory realloc*/
	template<int inst>
	void *__malloc_alloc_template<inst>::oom_realloc(void*p, size_t n)
	{
		void(*my_malloc_handler)();				//声明一个处理函数
		void*result;
		for (;;)								//反复分配
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();             //调用处理函数
			result = realloc(p, n);
			if (result)return(result);			//分配成功返回
		}
	}

	typedef __malloc_alloc_template<0> malloc_alloc;
	
}


#endif