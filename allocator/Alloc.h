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
		static void *oom_malloc(size_t);				//out of memory when malloc
		static void *oom_realloc(void*, size_t);			//out of memory when reallocate
		static void(*__malloc_alloc_oom_handler)();			//the handler of out of memory
	public:
		static inline void* allocate(size_t n);         	//allocate memory
		static inline void* reallocate(void*p, size_t n);   	//reallocate memory
		static inline void deallocate(void*p,size_t);	 	//free memory
		static void(*set_malloc_handler(void(*)()))();		//out of memory function
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
		return (old);	//用old保存之前的处理方法，调用该函数后，实际使用的是__malloc_alloc_oom_handler指定的函数					
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
		for (;;)						//反复分配
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();             	//调用处理函数
			result = malloc(n);
			if (result)return(result);	 	//分配成功返回
		}
	}

	/*out of memory realloc*/
	template<int inst>
	void *__malloc_alloc_template<inst>::oom_realloc(void*p, size_t n)
	{
		void(*my_malloc_handler)();				//声明一个处理函数
		void*result;
		for (;;)						//反复分配
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			(*my_malloc_handler)();             	//调用处理函数
			result = realloc(p, n);
			if (result)return(result);		//分配成功返回
		}
	}

	
	

/***************************************************************************************/
//第二级配置器

	enum {__ALIGN=8};  				//小型区块按照8字节对齐
	enum {__MAX_BYTES=128};				//小型区块的最大字节
	enum {__NUMFREELIST=__MAX_BYTES/__ALIGN}	 //free_lists的个数
	enum { __DEFAULT_NOBJS = 20 };			//重新分配时，默认的数量

template<int inst>
class __default_alloc_template
{
private:
	//将分配的字节数都调整为8的倍数
	static size_t ROUND_UP(size_t bytes) { return (((bytes)+__ALIGN - 1)&~(__ALIGN - 1)); }  
	union obj    //free_lists节点
	{
		union obj*free_list_link;    
		char client_data[1];
	};

	static obj* volatile free_lists[__NUMFREELIST]; //存储free_lists节点指针的数组，通过它们来寻找对应小型区块空闲区
	static size_t FREELIST_INDEX(size_t bytes)//通过调整后的字节数来计算要使用的小型区块数组里的索引。
	{
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}

	static void*refill(size_t n);
	/*分配大小为size，数量为nobjs的空间，注意nobjs是以引用的方式传递，nobjs反映实际分到的内存大小*/
	static char*chunk_alloc(size_t size, int &nobjs);

	/*内存池指针*/
	static char*start_free;		 //内存池开始位置
	static char*end_free;		 //内存池结束位置
	static size_t heap_size;	 //已经分过的内存大小
	/*接口*/
public:
	static void *allocate(size_t n);
	static void	deallocate(void*p, size_t n);
	static void *reallocate(void*p, size_t n);

};

//static成员的初始化
//16个小型区块地址初始化
template<int inst> typename __default_alloc_template<inst>::obj* volatile 
                	__default_alloc_template<inst>::free_lists[__NUMFREELIST] =
						{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//内存池起始位置
template<int inst>
char* __default_alloc_template<inst>::start_free = 0;
//内存池结束位置
template<int inst>
char* __default_alloc_template<inst>::end_free = 0;
//已分配的堆大小
template<int inst>
size_t __default_alloc_template<inst>::heap_size = 0;



template<int inst>
void * __default_alloc_template<inst>::refill(size_t n)
{
	int nobjs = __DEFAULT_NOBJS;   		//重新分配空间，默认__DEFAULT_NOBJS个
	char*chunk = chunk_alloc(n, nobjs);  	//调用内存池获得内存
	obj*volatile*my_free_list;
	obj*result;
	obj*current_obj, *next_obj;   		//链表构建
	int i;
	if (nobjs == 1)return (chunk);

	my_free_list = free_lists + FREELIST_INDEX(n);   //获得插入的索引值
	result = (obj*)chunk;	   			//取出这一块给用户使用
	*my_free_list = next_obj = (obj*)(chunk + n);  
	/*构建链表，将获得的类存按照size大小串起来*/
	for (i = 1; i < nobjs; ++i)//第一个用于reslt了，指示第一个空闲的小型区块。
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);   //next节点向后移动
		if (nobjs == i + 1)
		{
			current_obj->free_list_link = 0;  //最后一个数据块
		}
		else
		{
			current_obj->free_list_link = next_obj;
		}

		
	}
	return (result);

}

/*内存池的构建*/
/*
*	参数：size 小型区块大小;nobjs 需求的小型区块个数
*/
template<int inst>
char*__default_alloc_template<inst>::chunk_alloc(size_t size, int&nobjs)
{
	char*result;
	size_t total_size = size*nobjs;
	size_t bytes_left = end_free - start_free;
	if (bytes_left >= total_size)
	{
		result = start_free;
		start_free += total_size;
		return (result);
	}
	else if (bytes_left >= size)		//剩下的内存可以提供一个及以上，但不能满足要求的那么多
	{
		nobjs = bytes_left / size;	//最多能提供多少个这样的小型区块
		total_size = nobjs*size;
		result = start_free;
		start_free += total_size;
		return (result);
	}
	else  
	{
		if (bytes_left > 0)//查询是否有其他的小型区块能用这点内存，如果可以的话，就把这点内存分配给它
		{
			obj*volatile *my_free_list = free_lists + FREELIST_INDEX(bytes_left);//查询能插入到哪个小型区块
			((obj*)start_free)->free_list_link = *my_free_list;   		      //插入原有链表中
			*my_free_list = ((obj*)start_free);
		}
		size_t bytes_to_get = 2 * total_size + ROUND_UP(heap_size >> 4);  	      //多分1倍，加上偏置量
		start_free = static_cast<char*>(malloc(bytes_to_get));			       //从堆里重新获得内存
		if (start_free == 0)  //分配失败,堆空间不足了.
		{
			int i = 0;
			obj*volatile*my_free_list, *p;
			/*
			*	堆里没有空间了，那么查询现有的free_list里还有没有多余的空间，如果有的话，就取出来用
			*/
			for (i = size; i <= __MAX_BYTES; i += __ALIGN)
			{
				my_free_list = free_lists + FREELIST_INDEX(i);  //获得小型区块的空闲空间起始地址
				p = *my_free_list;
				if (p != 0)//找到
				{
					*my_free_list = p->free_list_link;
					start_free = (char*)(p);
					end_free = start_free + i;
					return (chunk_alloc(size, nobjs));   //递归调用，
				}

			}
			end_free = 0;   //已经找不到了
			start_free = static_cast<char*>(malloc_alloc::allocate(bytes_to_get));//如果失败就会报错
		}
		heap_size = bytes_to_get;   //如果成功分配了内存，就记录下得到了多少内存
		end_free = start_free + bytes_to_get;
		return(chunk_alloc(size, nobjs));
	}
	
}


/*空间分配*/
template<int inst>
inline void * __default_alloc_template<inst>::allocate(size_t n)
{
	obj*volatile*my_free_list;
	obj*result;
	if (n > (size_t)__MAX_BYTES)
	{
		return (malloc_alloc::allocate(n));
	}
	my_free_list = free_lists + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0)
	{
		void*r = refill(ROUND_UP(n));
		return (r);
	}
	*my_free_list = result->free_list_link;
	return (result);

}



/*释放空间*/
template<int inst>
void __default_alloc_template<inst>::deallocate(void * p, size_t n)
{
	obj*q = (obj*)p;
	obj*volatile*my_free_list;
	if (n > ((size_t)__MAX_BYTES))
	{
		malloc_alloc::deallocate(p,n);
	}
	my_free_list = free_lists + FREELIST_INDEX(n);
	/*将p所指向的空间插到链表头*/
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

/**********************************************************************************/
	typedef __malloc_alloc_template<0> malloc_alloc;
	typedef __default_alloc_template<0> default_alloc;   //直接将模板参数设定为0

}






#endif
