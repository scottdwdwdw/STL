#ifndef ITERATOR_H
#define ITERATOR_H


#include"Allocator.h"
/*
*  未通过标准库容器测试
*  通过内置型指针的测试
*/
namespace MyCppSTL
{
	/********iterator_catrgory的定义**********/
	struct input_iterator_tag{};
	struct forward_iterator_tag:public input_iterator_tag{};
	struct bidirectional_iterator_tag:public forward_iterator_tag{};
	struct random_access_iterator_tag:public bidirectional_iterator_tag{};
	/*****************************************/
	template<class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::value_type 			value_type;  				//解引用而获得的值类型
		typedef typename Iterator::difference_type 		difference_type;			//迭代器之差
		typedef typename Iterator::pointer 				pointer;					//指针类型
		typedef typename Iterator::reference 			reference;					//引用类型
		typedef typename Iterator::iterator_category 	iterator_category;			//迭代器类型，指明是input_iterator、output_iterator...	
	};
	//为T*,const T* 提供特例化版本
	template<class Iterator>
	struct iterator_traits<Iterator*>
	{
		typedef Iterator 						value_type;  				//解引用而获得的值类型
		typedef ptrdiff_t 						difference_type;			//迭代器之差
		typedef Iterator*	 					pointer;					//指针类型
		typedef Iterator&	  					reference;					//引用类型
		typedef random_access_iterator_tag	 	iterator_category;			//迭代器类型，内置指针是随机存取的	
	};
	template<class Iterator>
	struct iterator_traits<const Iterator*>
	{
		typedef Iterator 						value_type;  				//解引用而获得的值类型
		typedef ptrdiff_t 						difference_type;			//迭代器之差
		typedef Iterator*	 					pointer;					//指针类型
		typedef Iterator&	  					reference;					//引用类型
		typedef random_access_iterator_tag	 	iterator_category;			//迭代器类型，内置指针是随机存取的	
	};
	
	/*当我们自己定义自己的类的迭代器的时候可以从这个类继承而来，这样做是为了符合SGI规范*/
	template<class Category,class T,class Distance = ptrdiff_t,class Pointer = T*,class Reference = T&>
	struct iterator
	{
		typedef Category 	iterator_category;
		typedef T		 	value_type;
		typedef Distance 	difference_type;
		typedef Pointer	 	pointer;
		typedef Reference 	reference;
	};
	
	/*************迭代器操作***********************/

	/*******************advance函数****************/
	template<class InputIt,class Distance>
	inline void __advance(InputIt&it,Distance n, MyCppSTL::input_iterator_tag)
	{
		while(n--)++it;
	}
	
	/*  没有必要定义这个函数，因为input_iterator_tag和forward_iterator_tag的动作一致，
		且forward_iterator_tag可以向input_iterator_tag转换
	template<class InputIt,class Distance>
	inline void __advance(InputIt&it,Distance n,forward_iterator_tag)
	{
		while(n--)++it;
	}
	*/
	template<class InputIt,class Distance>
	inline void __advance(InputIt&it,Distance n, MyCppSTL::bidirectional_iterator_tag)
	{
		if(n>0)
		{
			while(n--)++it;		//向后移动
		}
		else
		{
			while(n++)--it;		//向前移动
		}
	}
	
	template<class InputIt,class Distance>
	inline void __advance(InputIt&it,Distance n, MyCppSTL::random_access_iterator_tag)
	{
		it+=n;
	}

	/*
	template<class InputIt>
	inline typename iterator_traits<InputIt>::iterator_category
		iterator_category(const InputIt&)
	{
		typedef typename iterator_traits<InputIt>::iterator_category category;
		return(category());
	}
	*/
	template<class InputIt,class Distance>
	inline void advance(InputIt&it,Distance n)
	{
		__advance(it,n, iterator_traits<InputIt>::iterator_category());	//根据不同iterator_category，调用不同的函数
	}
	
	/******************distance函数***************************/
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
	__distance(Iterator first,Iterator last,input_iterator_tag)
	{
		iterator_traits<Iterator>::difference_type n=0;
		while(first++!=last)++n;
		return n;
	}
	
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
	__distance(Iterator first,Iterator last,random_access_iterator_tag)
	{
		return last-first;
	}
	
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type 
	distance(Iterator first,Iterator last)
	{
		return (__distance(first,last,iterator_traits<Iterator>::iterator_category()));
	}
	
	/************next函数**********************************/
	template<class ForwardIt>
	ForwardIt next(ForwardIt it,typename std::iterator_traits<ForwardIt>::difference_type n = 1)
	{
		MyCppSTL::advance(it, n);
		return it;
	}
	/**********pre函数*******************/
	template<class BidirIt>
	BidirIt prev(BidirIt it, typename std::iterator_traits<BidirIt>::difference_type n = 1)
	{
		MyCppSTL::advance(it, -n);
		return it;
	}
	
	/*****容器的begin****/
	template<class C>    //容器
	auto begin(C&c)->decltype(c.begin())
	{
		return c.begin();
	}
	
	template<class C>
	auto begin( const C& c ) -> decltype(c.begin())
	{
		return c.begin();
	}
	template<class T,std::size_t N>
	T*begin(T(&array)[N])
	{
		return array;
	}
	
	template<class T, std::size_t N>
	constexpr T*cbegin(T(&array)[N] )
	{
		return array;
	}
	
	template<class C> 
	constexpr auto cbegin( const C& c )->decltype(MyCppSTL::begin(c))
	{
		return MyCppSTL::begin(c);
	}
	
	/**********end函数*************/
	template<class C>    //容器
	auto end(C&c)->decltype(c.end())
	{
		return c.end();
	}
	template<class C>    //容器
	auto end(const C&c)->decltype(c.end())
	{
		return c.end();
	}
	template<class T,size_t N>
	T*end(T(&array)[N])
	{
		return (array+N);
	}
	
	template<class T,size_t N>
	constexpr T*cend(T(&array)[N])
	{
		return (array+N);
	}
	
	template<class C>
	constexpr auto cend(C&c)->decltype(MyCppSTL::end(c))
	{
		return MyCppSTL::end(c);
	}


	/*****************array的迭代器**********************/

	//array 迭代器，常量版本
	//非常量的
	template<class T, std::size_t N>
	struct array_const_iterator
		:public MyCppSTL::iterator<MyCppSTL::random_access_iterator_tag,
		const T,
		std::ptrdiff_t,
		const T*,
		const T&>
	{
	public:
		typedef array_const_iterator<T, N>				MyIterator;
		typedef MyCppSTL::random_access_iterator_tag	iterator_tag;
		typedef const T									value_type;
		typedef const T*								pointer;
		typedef const T&								reference;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		pointer		_ptr_c;//成员
						 //构造函数
		array_const_iterator() :_ptr_c(0) {};	//默认构造函数
		explicit array_const_iterator(pointer p) :_ptr_c(p) {};

		//操作
		reference operator*() const { return *_ptr_c; }   //解引用
		reference operator[](size_type pos) const { return _ptr_c[pos]; }//下标操作
		//算术运算符
		MyIterator&operator++()  //前置++
		{
			++_ptr_c;
			return *this;
		}
		MyIterator operator++(int)  //后置++
		{
			MyIterator tmp = *this;
			++*this;
			return tmp;  //将该临时变量拷贝到调用点的对象中
		}
		MyIterator&operator+=(difference_type pos)
		{
			_ptr += pos;
			return *this; 
		}
		MyIterator operator+(difference_type pos) const//加法操作
		{
			return MyIterator(_ptr_c + pos);
		}
		MyIterator&operator--()  //前置--
		{
			--_ptr_c;
			return *this;
		}
		MyIterator operator--(int)  //后置--
		{
			MyIterator tmp = *this;
			--*this;
			return tmp;
		}
		MyIterator& operator-=(difference_type pos)//-=
		{
			_ptr_c -= pos;
			return *this;
		}
		MyIterator operator-(difference_type pos) const
		{
			return MyIterator(_ptr_c - pos);
		}
		difference_type operator-(const MyIterator&rhs) const
		{
			return (_ptr_c - rhs._ptr_c);
		}
		//关系运算符
		bool operator==(const MyIterator&rhs) const
		{
			return (_ptr_c == rhs._ptr_c);
		}
		bool operator!=(const MyIterator&rhs) const
		{
			return (!(*this == rhs));
		}
		bool operator<(const MyIterator&rhs) const
		{
			return (_ptr_c < rhs._ptr_c);
		}
		bool operator>(const MyIterator&rhs) const
		{
			return (_ptr_c > rhs._ptr_c);
		}
		bool operator>=(const MyIterator&rhs) const
		{
			return (!(*this<rhs));
		}
		bool operator<=(const MyIterator&rhs) const
		{
			return (!(*this>rhs));
		}
};


	//非常量的
	template<class T, std::size_t N>
	struct array_iterator :public array_const_iterator<T,N>
	{
		//内嵌型别的定义
		typedef MyCppSTL::random_access_iterator_tag	iterator_bag;
		typedef T										value_type;
		typedef T* 										pointer;
		//typedef const T*								const_pointer;
		typedef T&										reference;
		//typedef const T&								const_reference;
		typedef std::ptrdiff_t							difference_type;
		typedef std::size_t								size_type;
		
		//size_type	_size;			//大小
	public:
		pointer		_ptr;			//指针
		//构造函数
		array_iterator(){}			//默认构造函数
	    explicit array_iterator(pointer p):array_const_iterator<T, N>(p),_ptr(p)
		{
		}
		//操作
		reference operator*() 
		{ 
			return *_ptr;
		}
		array_iterator<T, N>&operator++()   //前置++
		{					
			//++*this;
			++_ptr;    //增加_ptr ++_ptr
			return *this;
		}
		array_iterator<T, N>operator++(int) //后置++
		{
			auto tmp = *this;    //返回量
			++*this;
			return tmp;
		}
		array_iterator<T, N>operator+(difference_type _off) const//+
		{
			auto tmp = *this;
			tmp += _off;      //
			return tmp;

		}

		array_iterator<T, N>&operator--()//前置--
		{
			--_ptr;				//未做边界检查
			return *this;
		}
		array_iterator<T, N>operator--(int)//后置--
		{
			auto tmp = *this;
			--*this;
			return tmp;

		}
		array_iterator<T, N>&operator-=(difference_type _off)//-=
		{
			_ptr -= _off;
			return *this;
		}
		array_iterator<T, N>operator-(difference_type _off) const//-
		{
			auto tmp = *this;
			tmp -= _off;
			return tmp;
		}

		array_iterator<T, N>&operator+=(difference_type _off)//+=
		{
			_ptr+=_off;
			return *this;
		}
		

		reference operator[](size_type _pos)//[]运算
		{
			return (_ptr[_pos]);
		}

		
		//比较运算
	
		bool operator==(const array_iterator<T, N>&rhs)const //==
		{
			return (_ptr == rhs._ptr);
		}
		bool operator!=(const array_iterator<T, N>&rhs)const//!=
		{
			return (!(*this == rhs));
		}
		bool operator<(const array_iterator<T, N>&rhs)const//<
		{
			return (_ptr < rhs._ptr);
		}
		bool operator>(const array_iterator<T, N>&rhs)const//>
		{
			return (_ptr>rhs._ptr);
		}
		bool operator>=(const array_iterator<T, N>&rhs)const//>=
		{
			return (!(*this < rhs));
		}
		bool operator<=(const array_iterator<T, N>&rhs)const//>=
		{
			return (!(*this > rhs));
		}
	

	};

	//array迭代器，reserve版本
	template<class T,std::size_t N>
	struct reserve_const_iterator :public MyCppSTL::iterator<MyCppSTL::random_access_iterator_tag,
		const T,
		std::ptrdiff_t,
		const T*,
		const T&>
	{
		typedef reserve_const_iterator<T, N>	MyIterator;
		typedef MyCppSTL::random_access_iterator_tag iterator_bag;
		typedef const T value_type;
		typedef const T& reference;
		typedef const T* pointer;
		typedef std::size_t size_value;
		typedef std::ptrdiff_t difference_type;

		//成员
		pointer _ptr_r;	
		//构造函数
		reserve_const_iterator() :_ptr_r(0) {}
		explicit reserve_const_iterator(pointer p) :_ptr_r(p) {}
		//操作
		reference operator*()const    //解引用
		{
			return (*_ptr_r);
		}
		reference operator[](const size_value&pos)const    //下标操作
		{
			return (_ptr_r[pos]);
		}
		MyIterator& operator++()   //前置递增
		{
			--_ptr_r;     //向前移动
			return *this;
		}
		MyIterator operator++(int)//后置递增
		{
			MyIterator tmp = *this;
			++*this;
			return tmp;
		}

		MyIterator &operator+=(difference_type n) const
		{
			_ptr_r -= n;
			return *this;
		}
		MyIterator operator+(difference_type n) const
		{
			MyIterator tmp = *this;
			tmp._ptr_r -= n;
			return tmp;
		}




	};


	//**************vector的迭代器*******************************
	//常量版本
	//vector_const_iterator,迭代器指向的对象的值不能改变，迭代器的值可以改变
	template<class T,class alloc=allocator<T>>
	class vector_const_iterator :public iterator<random_access_iterator_tag, T>
	{
	public:
		//内嵌型别
		typedef vector_const_iterator<T>	   _MyIter;
		typedef const T value;
		typedef const T* pointer;
		typedef const T& reference;
		typedef std::ptrdiff_t	difference_type;
		typedef std::size_t		size_type;
	protected:
		//成员
		pointer _ptr;
	
	public:
		//构造函数
		vector_const_iterator() :_ptr(0){}//默认构造函数
		vector_const_iterator(pointer ptr) :_ptr(ptr)//构造函数
		{
		}

		//解引用操作
		reference operator*() const
		{
			return (*_ptr);
		}
		//下标运算
		reference operator[](const size_type&pos)const
		{
			return _ptr[pos];
		}
		//前置递增
		_MyIter&operator++()
		{
			++_ptr;
			return *this;
		}
		//后置递增
		_MyIter operator++(int)
		{
			_MyIter tmp = *this;
			++*this;
			return tmp;
		}
		//复合赋值
		_MyIter&operator+=(const difference_type&pos)
		{
			_ptr += pos;
			return *this;
		}
		//加
		_MyIter operator+(const difference_type&pos)
		{
			_MyIter tmp = *this;
			tmp += pos;
			return tmp;
		}
		//前置减
		_MyIter&operator--()
		{
			--_ptr;
			return *this;
		}
		//后置递减
		_MyIter operator--(int)
		{
			_MyIter tmp = *this;
			--*this;
			return tmp;
		}
		//复合
		_MyIter&operator-=(const difference_type&pos)
		{
			_ptr-= pos;
			return *this;
		}
		//减
		_MyIter operator-(const difference_type&pos)
		{
			_MyIter tmp = *this;
			tmp -= pos;
			return tmp;
		}

		//比较运算符
		bool operator==(const _MyIter&rhs) const
		{
			return ((this->_ptr) == rhs._ptr);
		}
		bool operator!=(const _MyIter&rhs) const
		{
			return (!(*this == rhs));
		}
		bool operator<(const _MyIter&rhs) const
		{
			return ((this->_ptr) < rhs._ptr);
		}
		bool operator>(const _MyIter&rhs)const
		{
			return (this->_ptr>rhs._ptr);
		}
		bool operator>=(const _MyIter&rhs)const
		{
			return !(*this<rhs);
		}
		bool operator<=(const _MyIter&rhs)const
		{
			return !(*this>rhs);
		}

		//两个迭代器相减
		difference_type operator-(const _MyIter&rhs)
		{
			return (_ptr - rhs._ptr);
		}
			

	};
	
	//vector_const_iterator的非成员函数
   /*
	template<class T, class alloc = allocator<T>>
	std::ptrdiff_t operator-(const vector_const_iterator<T>&lhs, const vector_const_iterator<T>&rhs)
	{
		return (lhs._ptr - rhs._ptr);
	}
	*/
	//vectord的迭代器，非常量版本
	template<class T,class alloc=allocator<T>>
	class vector_iterator :public vector_const_iterator<T, alloc>
	{
		//内嵌型别定义
	public:
		typedef vector_iterator<T>	   _MyIter;
		typedef vector_const_iterator<T> _MyBase;
		typedef T value;
		typedef T* pointer;
		typedef T& reference;
		typedef std::ptrdiff_t	difference_type;
		typedef std::size_t		size_type;
		
		//构造函数
		vector_iterator() = default;
		vector_iterator(pointer ptr) :vector_const_iterator(ptr)
		{
			
		}
		//解引用
		reference operator*()
		{
			return ((reference)**(_MyBase*)this); //调用基类的接引用操作符，得到的是const T&类型，然后再强制转换为T&,即完成要求
		}
		//下标操作
		reference operator[](const size_type& pos)
		{
			return  ((reference)(*(_MyBase*)this)[pos]);
		}
		//前置递增
		_MyIter& operator++()
		{
			++*(_MyBase*)this;
			return *this;
		}
		//后置递增
		_MyIter operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		//复合赋值
		_MyIter& operator+=(const difference_type &pos)
		{
			*(_MyBase*)this += pos;
			return *this;
		}
		//加
		_MyIter operator+(const difference_type &pos)
		{
			auto tmp = *this;
			tmp += pos;
			return tmp;
		}
		//前置--
		_MyIter&operator--()
		{
			--*(_MyBase*)this;
			return *this;
		}
		//后置--
		_MyIter operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}
		//复合减
		_MyIter& operator-=(const difference_type&pos)
		{
			*(_MyBase*)this -= pos;
			return *this;
		}
		//减
		_MyIter operator-(const difference_type&pos)
		{
			auto tmp = *this;
			tmp -= pos;
			return tmp;
		}

		//迭代器相减
		difference_type operator-(const _MyIter&rhs)
		{
			return (_ptr - rhs._ptr);
		}

		//逻辑操作使用基类的


	};





	/************容器list的迭代器相关相关*********/
	//list节点
	template<class T>
	struct __list_node
	{
		typedef __list_node<T>* _NodePtr;
		_NodePtr  _pre;
		_NodePtr  _next;
		T data;
	};
	

	template<class T,class alloc=MyCppSTL::allocator<T>>
	class const_list_iterator:public iterator<MyCppSTL::bidirectional_iterator_tag,T>
	{
	public:  //内嵌型别
		typedef MyCppSTL::bidirectional_iterator_tag      iterator_category;
		typedef  const T*					          pointer;
		typedef  const T&					          reference;
		typedef  const T						      value_type;
		typedef std::ptrdiff_t						  difference_type;
		typedef std::size_t							  size_type;
		typedef __list_node<T>*					      _NodePtr;
		typedef const_list_iterator<T>		         _MyIterator;
	public: //构造函数
		const_list_iterator():_Ptr(0){} //  默认构造函数
		const_list_iterator(const _NodePtr&ptr) :_Ptr(ptr) {} //赋值构造函数


	    /*****迭代器的操作******/
		//赋值拷贝
		_MyIterator&operator=(const _MyIterator&rhs)
		{
			_Ptr = rhs._Ptr;
			return *this;
		}

		//解引用
		value_type operator*() const
		{
			return ((*_Ptr).data);
		}
		//箭头指向运算符
		pointer operator->()const
		{
			return (&operator*());
		}
		//前置递增
		_MyIterator& operator++()
		{
			_Ptr = _Ptr->_next;
			return *this;
		}
		//后置递增
		_MyIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		//前置递减
		_MyIterator& operator--()
		{
			_Ptr = _Ptr->_pre;
			return *this;
		}
		//后置递减
		_MyIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		//比较操作符
		bool operator==(const _MyIterator&rhs) const
		{
			return (_Ptr == rhs._Ptr);
		}
		bool operator!=(const _MyIterator&rhs)const
		{
			return (!(*this == rhs));
		}

	private:
		_NodePtr _Ptr;    //指向链表节点的指针
	};

	
	//list非常量版本迭代器
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class list_iterator :public const_list_iterator<T>
	{
	public:
		typedef T*							  pointer;
		typedef T&							  reference;
		typedef T							  value_type;
		typedef std::ptrdiff_t				  difference_type;
		typedef std::size_t				      size_type;
		typedef list_iterator<T>			  _MyIterator;
		typedef const_list_iterator<T>        _MyBase;
		typedef const_list_iterator<T>::_NodePtr _NodePtr;
	public:
		//构造函数
		list_iterator() :_MyBase() {}  //默认构造器
		list_iterator(const _NodePtr&ptr) :_MyBase(ptr) {} 

		//迭代器操作
		_MyIterator&operator=(const _MyIterator&rhs)
		{
			_Ptr = rhs._Ptr;
			return *this;
		}
		//前置递增
		_MyIterator& operator++()
		{
			++*((_MyBase*)this);   //调用基类的前置递增
			return *this;
		}
		//后置递增
		_MyIterator operator++(int)
		{
			++*this;  
			return *this;
		}
		//前置递增
		_MyIterator&operator--()
		{
			--*((_MyBase*)this);
			return *this;
		}

		//后置递减
		_MyIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		//比较操作使用基类的

	};
	
	
}
#endif




