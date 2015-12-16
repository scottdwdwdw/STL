#ifndef DEQUE_H
#define DEQUE_H

#include"Allocator.h"
#include"Iterator.h"
#include"Algorithm.h"
#include"__Type_traits.h"
#include<algorithm>
#include<initializer_list>


namespace MyCppSTL
{

//#define DEBUG

#ifdef DEBUG
	#define	_NODE_SIZE 1
#else
	#define	_NODE_SIZE 8
#endif

	#define max(a,b)  (a>b)?(a):(b)

#ifdef DEBUG
	inline std::size_t __deque_buf_size(std::size_t n) { return n < 2 ? (512 / n) : std::size_t(4); }
#else
	inline std::size_t __deque_buf_size(std::size_t n) { return n < 512 ? (512 / n) : std::size_t(1); }
#endif
	

	/*  deque的const迭代器 */
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class deque_const_iterator :public iterator<MyCppSTL::random_access_iterator_tag, T>
	{
	
	public:
		//内嵌型别
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque_const_iterator<T>    _MyIter;
		typedef pointer*				   map_pointer;
		//存储区大小
		static inline std::size_t deque_buf_size(std::size_t n){return __deque_buf_size(n);}
	public:
		//成员
		pointer _cur; //指示当前位置
		pointer _first;
		pointer _last;
		map_pointer _node;    //指示当前所在的map节点
	public:
		//默认构造函数
		deque_const_iterator():_cur(0), _first(0), _last(0),_node(0)
		{
		}
		//构造函数
		deque_const_iterator(pointer cur,map_pointer node):_cur(cur),_node(node)
		{
			set_node(node);
		}
	   
		//拷贝构造函数
		deque_const_iterator(const _MyIter&rhs) :_cur(rhs._cur), _node(rhs._node), _first(rhs._first), _last(rhs._last)
		{
		}
		
		//迭代器操作
		const_reference operator*() const //取值
		{
			return *_cur;
		}
	
		//迭代器自增 ++
		_MyIter&operator++() //前置递增
		{
			++_cur;
			if (_cur == _last)   //到达该缓冲区的末尾
			{
				set_node(_node + 1);
				_cur = _first;
			}
			return *this;
		}

		_MyIter operator++(int) //后置递增
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		_MyIter&operator+=(difference_type n) //复合赋值
		{
			difference_type offset = n + (_cur - _first);
			if (offset >= 0 && offset < (difference_type)deque_buf_size(sizeof(size_type)))
			{
				_cur += n;
			}
			else
			{
				difference_type node_offset = offset > 0 ? (offset / (difference_type)deque_buf_size(sizeof(size_type))) :
					(offset / (difference_type)deque_buf_size(sizeof(size_type))) - 1;

				set_node(_node + node_offset);
				_cur = _first + (offset - node_offset*deque_buf_size(sizeof(size_type)));
			}
			return *this;
		}

		_MyIter operator+(difference_type n) const//
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}

		_MyIter&operator--() //前置递减
		{
			//--_cur;
			if (_cur == _first)   //到达该缓冲区的末尾
			{
				set_node(_node - 1);
				_cur = _last;
			}
			--_cur;
			return *this;
		}

		_MyIter operator--(int) //后置递减
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}
		_MyIter&operator-=(difference_type n) //复合赋值
		{
			*this += -n;
			return *this;
		}

		_MyIter operator-(difference_type n)
		{
			auto tmp = *this;
			tmp -= n;
			return tmp;
		}

		difference_type operator-(const _MyIter&rhs) //迭代器距离
		{
			auto tmp = rhs;
			auto n = 0;
			while (_cur != tmp._cur)
			{
				++n;
				++tmp;
			}
			return n;
		}

		const_reference operator[](const size_type n)
		{
			return *(*this + n);
		}

		//逻辑操作
		bool operator==(_MyIter&rhs) const
		{
			return _cur == rhs._cur;
		}
		bool operator!=(_MyIter&rhs) const
		{
			return (!(*this == rhs));
		}
		bool operator<(_MyIter&rhs) const
		{
			return (_node == rhs._node ? _cur < rhs._cur : _node < rhs._node);
		}
		bool operator>(_MyIter&rhs) const
		{
			return (rhs < *this);
		}
		bool operator<=(_MyIter&rhs) const
		{
			return (!(*this>rhs));
		}

		bool operator>=(_MyIter&rhs) const
		{
			return (!(*this<rhs));
		}


	public:
		void set_node(map_pointer node)
		{
			_node = node;
			_first = *node;
			_last = _first + difference_type(deque_buf_size(sizeof(value_type)));
		}
		
	
	}; //end-of-const-iterator



template<class T, class alloc = MyCppSTL::allocator<T>>
class deque_iterator :public deque_const_iterator<T>
{
	public:
		//内嵌型别
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
	//	typedef deque_const_iterator<T>    _MyIter;
		typedef pointer*				   map_pointer;
		typedef deque_iterator<T>          _Myiter;
		typedef deque_const_iterator<T>    _MyBase;
	public:
		//构造函数
		//默认构造函数
		deque_iterator() :deque_const_iterator() {}
		//接受两个参数的构造函数
		deque_iterator(pointer cur, map_pointer node) :deque_const_iterator(cur, node) {}
		//拷贝构造函数
		deque_iterator(_Myiter&rhs) :deque_const_iterator(rhs) {}
		
		//迭代器的操作
		reference operator*()   //解引用
		{
			return (reference)**((_MyBase*)this);
		}
		_Myiter&operator++()  //前置递增
		{
			++*(_MyBase*)this;
			return *this;
		}
		_Myiter operator++(int) //后置递增
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		_Myiter&operator--()  //前置递减
		{
			--*(_MyBase*)this;
			return *this;
		}
		_Myiter operator--(int) //后置递减
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		_Myiter &operator+=(difference_type n) //
		{
			*(_MyBase*)this += n;
				return *this;
		}

		_Myiter operator+(difference_type n)
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}

		
		_Myiter operator-(difference_type n)
		{
			auto tmp = *this;
			tmp += -n;
			return tmp;
		}
		
		
		difference_type operator-(const _Myiter&rhs) const
		{
			return  ((*(_MyBase*)this) - rhs);
		}

		_Myiter &operator-=(difference_type n)
		{
			return *this += -n;
		}

		reference operator[](const size_type &n)
		{
			return *(*this + n);
		}


};  //end of iterator

/***************/
template<class T, class alloc> class deque;

template<class T, class alloc>
bool operator==(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);

template<class T, class alloc>
bool operator!=(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);

template<class T, class alloc>
bool operator>(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);

template<class T, class alloc>
bool operator<(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);

template<class T, class alloc>
bool operator<=(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);

template<class T, class alloc>
bool operator>=(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);

//deque模板
template<class T,class alloc=MyCppSTL::allocator<T>>
class deque {
	public:
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque<T>					my_deque;

		typedef deque_const_iterator<T>		const_iterator;
		typedef deque_iterator<T>			iterator;
		typedef pointer*					map_pointer;
		
		typedef MyCppSTL::allocator<pointer>  map_alloc;
		enum {NODESIZE= _NODE_SIZE};

		friend bool operator==<>(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);
		friend bool operator!=<>(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);
		friend bool operator><>(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);
		friend bool operator< <>(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);
		friend bool operator<= <>(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);
		friend bool operator>= <>(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs);
	public:
		//构造函数
	explicit deque()  //默认构造函数 
			{
				create_map(0);   //创建map
			}
	
	deque(size_type count, const T& value)  //接受一定数量的值初始化
	{
		create_map_and_node(count, value);
	}

	explicit deque(size_type count)
	{
		create_map_and_node(count, T());
	}

	template< class InputIt >
	deque(InputIt first, InputIt last)
	{
		initialize_dispatch(first, last, std::is_integral<InputIt>::type());//输入的可能是两个整数，可能是两个迭代器，调用不同的函数
	}

	deque(const deque& other) //拷贝构造函数
	{
		initialize_dispatch(other.begin(), other.end(),std::false_type());
	}

	deque(deque&& other):_begin(other._begin),_end(other._end),_map(other._map),_map_size(other._map_size)
	{
		other._begin._cur= other._begin._first=other._begin._last=0;
		other._begin._node =0;
		other._end._cur =other._end._first = other._end._last = 0;
		other._end._node = 0;
		other._map = 0;
		other._map_size = 0;
	}

	deque(std::initializer_list<T> init)
	{
		range_initialize(init.begin(), init.end(), forward_iterator_tag());
	}
	//析构
	~deque()
	{
		free();
	}
	//拷贝赋值运算符
	my_deque&operator=(const my_deque&rhs)
	{
		if (_begin._cur != rhs._begin._cur)
		{
			range_initialize(rhs.begin(), rhs.end(), MyCppSTL::forward_iterator_tag());
		}

		return *this;
	}
	my_deque&operator=(my_deque&&rhs)
	{
		if (_begin._cur != rhs._begin._cur)
		{
			_begin = rhs._begin;
			_end = rhs._end;
			_map = rhs._map;
			_map_size = rhs._map_size;
			rhs._begin._cur = 0; rhs._end._cur = 0; rhs._begin._first = rhs._begin._last = 0; rhs._begin._node = 0;
			rhs._end._first = rhs._end._last = 0; rhs._end._node = 0;
			rhs._map = 0;
			rhs._map_size = 0;
		}
		return *this;
	}
	my_deque&operator=(const std::initializer_list<T>&rhs)
	{
		range_initialize(rhs.begin(), rhs.end(), MyCppSTL::forward_iterator_tag());
		return *this;
	}
	//element access
	reference at(size_type pos)
	{
		if (pos > size())std::out_of_range("out of range");
		return *(begin() + pos);
	}
	const_reference at(size_type pos) const
	{
		if (pos > size())std::out_of_range("out of range");
		return *(begin() + pos);
	}

	reference operator[](size_type pos)
	{
		return *(begin() + pos);
	}
	const_reference operator[](size_type pos) const
	{
		return *(begin() + pos);
	}

	reference front()
	{
		return *begin();
	}

	const_reference front() const
	{
		return *begin();
	}
	reference back()
	{
		return *(end() - 1);
	}
	const_reference back() const
	{
		return *(end() - 1);
	}
	//capacity
	size_type size() const
	{
		return (size_type)(_end - _begin);
	}
	bool empty() const
	{
		return (_begin._cur==_end._cur);
	}

	//迭代器
	
	iterator begin() { return iterator(_begin); }
	const_iterator begin() const 
	{ 
		return const_iterator(_begin._cur, _begin._node);
		
	}
	const_iterator cbegin() const 
	{
		return const_iterator(_begin._cur,_begin._node);
	}
	
	iterator end() { return _end; }
	const_iterator end() const 
	{ 
		return const_iterator(_end._cur,_end._node);
	}
	const_iterator cend() const
	{ 
		return const_iterator(_end._cur, _end._node); 
	}

	//元素操作
	void push_back(const value_type&value)
	{
		if (_end._cur != _end._last-1)  //*****留下1个空间
		{
			construct(_end._cur, value);
			++_end;
		}
		else
		{
			push_back_aux(value);
			
		}
	}
	void push_front(const T& value)
	{
		if (_begin._cur != _begin._first)
		{
			--_begin;
			construct(_begin._cur, value);
			
		}
		else
		{
			push_front_aux(value);
		}
	}

	//插入元素
	iterator insert(iterator pos, size_type count, const T& value)
	{
		return (insert_dispatch(pos, count, value));

	}

	iterator insert(iterator pos, const T&value)
	{
		return (insert_dispatch(pos, 1, value));
	}

	template< class InputIt >
	iterator  insert(iterator pos, InputIt first, InputIt last)
	{
		return (insert_aux(pos, first, last, std::is_integral<InputIt>::type()));
	}

	iterator insert(iterator pos, std::initializer_list<T> ilist)
	{
		return (insert(pos, ilist.begin(), ilist.end()));
	}

	//
	void pop_front()
	{
		erase(begin());
	}

	void pop_back()
	{
		erase(end() - 1);
	}
	//erase
	iterator erase(iterator pos) 
	{
		return erase_dispatch(pos,pos+1);
	}

	iterator erase(iterator first, iterator last)
	{
		
		erase_dispatch(first,last);
		return last;
	}
	void clear()
	{
		free();
		_begin._node = 0; _end._node = 0;
		_begin._first = _begin._last = _begin._cur = 0;
		_end._last = _end._last = _end._cur = 0;
	}

	void resize(size_type count)
	{
		resize(count, T());
	}

	void resize(size_type count, const value_type& value)
	{
		if (count < size())
		{
			size_type elem_erase = size() - count;
			erase(_end - elem_erase, _end);
		}
		else if (count>size())
		{
			size_type elem_erase = count - size();
			insert(_end, elem_erase,value);
		}
	}

	void swap(my_deque&rhs)
	{
		auto tmp = *this;
		_begin = rhs._begin;
		_end = rhs._end;
		_map = rhs._map;
		_map_size = rhs._map_size;
		rhs._begin = tmp._begin;
		rhs._end = tmp._end;
		rhs._map = tmp._map;
		rhs._map_size = tmp._map_size;
	}
	protected:
		//辅助函数，创建map
		std::size_t deque_buf_size(std::size_t n) { return __deque_buf_size(n); }
		void create_map(size_type elem_num);
		void create_map_and_node(size_type elem_num,value_type value);
		void node_move(map_pointer node);

		
		template<class InputIter>
		inline void initialize_dispatch(InputIter first, InputIter last, std::false_type);
		template<class Integer>
		inline void initialize_dispatch(Integer count, Integer value, std::true_type);

		template<class InputIter>
		void range_initialize(InputIter first, InputIter last, MyCppSTL::forward_iterator_tag);
		template<class InputIter>
		void range_initialize(InputIter first, InputIter last, MyCppSTL::input_iterator_tag);
		
		void push_back_aux(const value_type&value); //当map空间不够的时候从新分配
		void push_front_aux(const value_type&value);

		
		void reserve_map_at_back(size_type nodes_to_add = 1);
		void reserve_map_at_front(size_type nodes_to_add = 1);
		void reallocate_map(size_type nodes_to_add, bool add_at_front);

		iterator insert_dispatch(iterator pos, size_type count, const T&value);
		iterator insert_at_begin(iterator pos, size_type count, const T& value);
		iterator insert_at_end(iterator pos, size_type count, const T& value);

		template<class integer>
		iterator insert_aux(iterator pos, integer count, integer value, std::true_type);
		template<class InputIter>
		iterator insert_aux(iterator pos, InputIter first, InputIter last, std::false_type);

	
		iterator erase_dispatch(iterator first,iterator last);
		iterator erase_at_end(iterator pos);
		iterator erase_at_begin(iterator pos);
		void free()
		{
			if (_begin._cur)
			{
				auto it = begin()._node;
				for (; it < end()._node; ++it)
				{
					destroy(*it, *it + __deque_buf_size(sizeof(T)));
					alloc::deallocate(*it, __deque_buf_size(sizeof(T)));
				}
				destroy(*it, *it + (end()._cur - *it));
				alloc::deallocate(*it, end()._cur - *it);
			}
		}
	private:
		map_pointer	_map; //指向map的指针
		size_type _map_size; //map大小
		iterator _begin; 
		iterator _end;   
	
	};  //end of deque


	//deque implement
	/***辅助函数****/

	//创建map

    template<class T, class alloc >
	void deque<T, alloc>::create_map(size_type elem_num)
	{
		//计算需要多个map节点
		//deque_buf_size返回的是单个节点存储大小
		size_type num_nodes = elem_num / deque_buf_size(sizeof(value_type)) +1;  
#ifdef DEBUG	 //调试用
		_map_size = max(NODESIZE, num_nodes);
#else
		_map_size = max(NODESIZE, num_nodes+2);   
#endif

		_map =map_alloc::allocate(_map_size);    //分配节点
		map_pointer nstart = _map + (_map_size - num_nodes) / 2;//从中间开始
		map_pointer nend = nstart + num_nodes-1;    
		map_pointer cur = nstart;
		for (; cur <= nend; ++cur)
		{
			*cur=alloc::allocate(deque_buf_size(sizeof(value_type))); //为每个节点分配存储区
		}
		_begin.set_node(nstart);
		_end.set_node(nend);
		_begin._cur = _begin._first;
		_end._cur = _end._first + elem_num%deque_buf_size(sizeof(value_type));
	}

	template<class T,class alloc>
	void deque<T, alloc>::create_map_and_node(size_type count, value_type value)
	{
		create_map(count);    //构造map节点，并初始化成员
		map_pointer cur = _begin._node;
		for (; cur < _end._node - 1; ++cur)
		{
			uninitialized_fill(*cur, *cur + deque_buf_size(sizeof(value_type)), value);
		}
		uninitialized_fill(_end._first, _end._cur, value);
	}
	

	template<class T,class alloc>
	template<class Integer>
	inline void deque<T,alloc>::initialize_dispatch(Integer count, Integer value, std::true_type)
	{
		create_map_and_node(count, value);
	}

	template<class T,class alloc>
	template<class InputIter>      //是迭代器
	inline void deque<T, alloc>::initialize_dispatch(InputIter first, InputIter last,std::false_type)
	{
		//如果是input_iterator,调用push_back来实现
		//如果是forward_iterator或更好的迭代器，则调用它们的copy 构造函数来实现
		typedef typename iterator_traits<InputIter>::iterator_category Cat;
		range_initialize(first, last, Cat());   //调用相应的迭代器
	}

	template<class T,class alloc>
	template<class ForwardIter>
	void deque<T, alloc>::range_initialize(ForwardIter first, ForwardIter last, MyCppSTL::forward_iterator_tag)
	{
		difference_type n = last - first;
		create_map(n);   //创造出map节点,并设置好迭代器
		iterator cur_iter = _begin;
		for (; n> 0; --n,++first)
		{
			construct(cur_iter._cur, *first);  //创建
			++(cur_iter._cur);
			if (cur_iter._cur == cur_iter._last)
			{
				++cur_iter;
			}

		}
	}

	template<class T,class alloc>
	template<class InputIter>
	void deque<T, alloc>::range_initialize(InputIter first, InputIter last, MyCppSTL::input_iterator_tag)
	{
		//调用push_back来完成
	}

	
	
	template<class T,class alloc=MyCppSTL::allocator<T>>
	void deque<T, alloc>::reallocate_map(size_type nodes_to_add, bool add_at_front)
	{
		size_type old_nodes = (size_type)((_end._node - _begin._node)+1);   //分配前的节点数
		size_type new_nodes = old_nodes + nodes_to_add;

		map_pointer new_begin;
		if (2*new_nodes < _map_size) //旧的map空间还有足够的空间
		{
			new_begin = _map + (_map_size - new_nodes) / 2 + (add_at_front ? nodes_to_add : 0);  //调整开始位置
			if (new_begin < _begin._node)//at_back
			{
				MyCppSTL::copy(_begin._node, _end._node+1, new_begin);  
			}
			else //at_front
			{
				MyCppSTL::copy_backward(_begin._node, _end._node+1, new_begin+old_nodes);
			}
		}
		else  //空间不够，需要重新分配
		{
			size_type new_map_size = _map_size + (max(_map_size, nodes_to_add)) + 2;
			map_pointer new_map = map_alloc::allocate(new_map_size);   //分配新的map节点空间
			new_begin = new_map + (new_map_size - new_nodes) / 2+(add_at_front?nodes_to_add:0); //确定新的起始位置
			copy(_begin._node, _end._node+1, new_begin);
			map_alloc::deallocate(_map, _map_size);
			_map = new_map;
			_map_size = new_map_size;
		}
		_begin.set_node(new_begin);
		_end.set_node(new_begin + old_nodes - 1);//将原有的节点拷贝到新节点处完毕
	}

	template<class T,class alloc=allocator<T>>
	void deque<T,alloc>::reserve_map_at_back(size_type nodes_to_add = 1)
	{
		if ((nodes_to_add + 1) > (_map_size - (_end._node-_map)))
		{
			reallocate_map(nodes_to_add, false);
		}
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	void deque<T,alloc>::reserve_map_at_front(size_type nodes_to_add = 1)
	{
		if ((difference_type)nodes_to_add > (_begin._node-_map))
		{
			reallocate_map(nodes_to_add, true);
		}
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	void deque<T, alloc>::push_back_aux(const value_type&value)
	{
		reserve_map_at_back();    //首先分配map节点
		*(_end._node + 1) = alloc::allocate(deque_buf_size(sizeof(value_type)));//为新的map节点分配存储区
		construct(_end._cur, value); 
		_end.set_node(_end._node + 1);
		_end._cur = _end._first;
	
	}
	template<class T,class alloc=MyCppSTL::allocator<T>>
	void deque<T, alloc>::push_front_aux(const value_type&value)
	{
		reserve_map_at_front();
		*(_begin._node - 1) = alloc::allocate(deque_buf_size(sizeof(value_type)));
		_begin.set_node(_begin._node - 1);
		_begin._cur = _begin._last - 1;
		construct(_begin._cur, value);
		//--_begin;
	}

	template < class T,class alloc=MyCppSTL::allocator<T> >
	typename deque<T,alloc>::iterator deque<T,alloc>::insert_dispatch(iterator pos, size_type count, const T&value)
	{
		size_type index = pos - _begin;    //计算当前要插入的位置离哪一端近
		bool at_front = (index > (size() / 2)) ? false : true;

		//移动前部/后部
		if (at_front)
		{
			return insert_at_begin(pos, count, value);
		}
		else
		{
			return insert_at_end(pos, count, value);
		}
		
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	typename deque<T,alloc>::iterator deque<T,alloc>::insert_at_begin(iterator pos, size_type count, const T& value)
	{
		difference_type index = pos - _begin;
		auto n = count;
		while (n--)
		{
			push_front(value);
		}
		auto it1 = _begin + count;
		auto it2 = _begin + count + index;
		
		auto _copy_begin = MyCppSTL::copy(it1, it2, _begin); //copy
		uninitialized_fill_n(_copy_begin,count, value);
		return pos;

	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	typename deque<T, alloc>::iterator deque<T, alloc>::insert_at_end(iterator pos, size_type count, const T& value)
	{
		difference_type index= _end - pos;
		auto n = count;
		while (n--)
		{
			push_back(value);
		}
	   
		auto it1 = _end - count-index;
		auto it2 = _end - count;
		auto _copy_begin = MyCppSTL::copy_backward(it1, it2, _end);
		uninitialized_fill_n(it1, count, value);
		return pos;
	}

	template<class T,class alloc>
	template<class integer>
	typename deque<T, alloc>::iterator deque<T, alloc>::insert_aux(iterator pos, integer count, integer value, std::true_type)
	{
		return (insert_dispatch(pos, count, value));
	}

	template<class T,class alloc>
	template<class InputIter>
	typename deque<T, alloc>::iterator deque<T, alloc>::insert_aux(iterator pos, InputIter first, InputIter last, std::false_type)
	{
		difference_type distance = pos - _begin;
		for (; first != last;)
		{
			insert_dispatch(_begin+distance, 1 ,*first);
			++first;
			++distance;
		
		}
		return pos-(last-first);
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	typename deque<T, alloc>::iterator deque<T, alloc>::erase_dispatch(iterator first, iterator last)
	{
		size_type distance = (size_type)(first - _begin);
		bool at_front = (distance > (size() / 2)) ? false : true;
		if (at_front)
		{
			for (; first != last;)
			{
				erase_at_begin(first);
				++first;
			}
			return first;
		}
		else
		{
			auto tmp = first;
			for (; first != last;)
			{
				erase_at_end(tmp);
				++first;
			}
			return first;
		}
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	typename deque<T, alloc>::iterator deque<T, alloc>::erase_at_begin(iterator pos)
	{
		auto tmp = _begin;
		if(pos!=_begin)
		   MyCppSTL::copy_backward(_begin, pos, pos+1);
		++_begin;
		alloc::destroy(tmp._cur);
		return pos;
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	typename deque<T, alloc>::iterator deque<T, alloc>::erase_at_end(iterator pos)
	{
		auto tmp = _end-1;
		if (pos != _end-1)
			MyCppSTL::copy(pos+1, _end, pos);
		--_end;
		alloc::destroy(tmp._cur);
		return pos;
	}


	//非成员函数

	template<class T, class alloc=MyCppSTL::allocator<T>>
	bool operator==(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs)
	{
		if (lhs.size() != rhs.size())return false;
		auto first = lhs.begin();
		auto second = rhs.begin();
		for (; first!=lhs.end();++first,++second)
		{
			if (*first != *second)return false;
		}

		return true;
	}

	template<class T, class alloc=MyCppSTL::allocator<T>>
	bool operator!=(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs)
	{
		return (!(lhs == rhs));
	}

	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator<(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs)
	{
		return (lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}
	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator>(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs)
	{
		return (rhs < lhs);
	}
	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator<=(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs)
	{
		return (!(lhs>rhs));
	}
	template<class T, class alloc = MyCppSTL::allocator<T>>
	bool operator>=(const deque<T, alloc>&lhs, const deque<T, alloc>&rhs)
	{
		return !(lhs < rhs);
	}


}  




#endif  //end of deque
