#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include"Alloc.h"
#include"Construct.h"

/*
*  分配器接口，包装第一二级配置器，提供标准接口
*/



#ifdef __USE_MALLOC
typedef MyCppSTL::malloc_alloc alloc;
# else
typedef MyCppSTL::default_alloc alloc;
#endif


namespace MyCppSTL 
{
	
	/*分配器接口*/
	template<class T,class Alloc=alloc>
	class allocator
	{
	public:
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef std::size_t		size_type;
		typedef std::ptrdiff_t	difference_type;



	public:
		/*空间的分配与释放*/
		static inline T*allocate(size_t n);
		static inline T*allocate(void);
		static inline void deallocate(T*p, size_t n);
		static inline void deallocate(T*p);
		/*对象的构造与析构*/
		static inline void construct(T*p, const T&n);
		static inline void destroy(T*p);
		/*成员函数*/
		inline T* address(T&);
		inline const T*address(const T&);

	};

	template<class T, class Alloc = alloc>
	T*allocator<T,Alloc>::allocate(size_t n)
	{
		return 0 >= n ? 0 : static_cast<T*> (Alloc::allocate(n*sizeof(T)));//分配n*sizeof(T)个空间
	}

	template<class T, class Alloc = alloc>
	T*allocator<T, Alloc>::allocate(void)
	{
		return static_cast<T*>(Alloc::allocate(sizeof(T)));
	}

	template<class T, class Alloc>
    void allocator<T, Alloc>::deallocate(T*p, size_t n)
	{
		if (n != 0) { Alloc::deallocate(p, n); }
	}

	template<class T, class Alloc>
	void allocator<T, Alloc>::deallocate(T*p)
	{
		Alloc::deallocate(p, sizeof(T));
	}

	template<class T, class Alloc>
	void allocator<T, Alloc>::construct(T*p, const T&n)
	{
		MyCppSTL::construct(p, n);
	}

	template<class T, class Alloc>
	void allocator<T, Alloc>::destroy(T*p)
	{
		MyCppSTL::destroy(p);
	}

	template<class T, class Alloc>
	T* allocator<T, Alloc>::address(T&x)
	{
		return &x;
	}

	template<class T, class Alloc>
    const T*allocator<T, Alloc>::address(const T&x)
	{
		return &x;
	}
	
	
	//allocator的嵌套型别
	template<class Alloc>
	struct allocator_traits
	{
		typedef typename Alloc::value_type		value_type;
		typedef typename Alloc::pointer			pointer;
		typedef typename Alloc::const_pointer	const_pointer;
		typedef typename Alloc::reference		reference;
		typedef typename Alloc::const_reference	const_reference;
		typedef typename Alloc::size_type		size_type;
		typedef typename Alloc::difference_type	difference_type;
	};

}

#endif