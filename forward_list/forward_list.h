#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include"Allocator.h"
#include"Iterator.h"
#include<initializer_list>

namespace MyCppSTL
{
	//链表节点
	template<class T>
	struct _node
	{
		typedef T value_type;
		_node<value_type>*_next;
		value_type data;
	};

	//链表的迭代器,常量版本
	template<class T>
	class forward_list_const_iterator:public iterator<MyCppSTL::forward_iterator_tag,T>
	{
	public: //内嵌型别
		typedef const T value_type;
		typedef const T& reference;
		typedef const T* pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef MyCppSTL::forward_iterator_tag iterator_category;
		typedef forward_list_const_iterator<T> _MyIter;
		typedef _node<T>*  _nodePtr;
	protected: //数据成员
		_node<T>* node;
	public:
		//构造函数
		forward_list_const_iterator() :node() {}
		explicit forward_list_const_iterator(const _nodePtr&n) :node(n) {}
		forward_list_const_iterator(const _MyIter&other) :node(other.node) {}
		//
		_MyIter&operator=(_MyIter&other)
		{
			node = other.node;
			return *this;
		}
		//
		//迭代器操作
		reference operator*() const
		{
			return node->data;
		}
		pointer operator->()const
		{
			return &(operator*())
		}
		_MyIter&operator++() //前置递增
		{
			node = node->_next;
			return *this;
		}
		_MyIter operator++(int)
		{
			auto tmp =*this;
			++*this;
			return tmp;
		}
		//逻辑操作
		bool operator==(const _MyIter&rhs)
		{
			return (node == rhs.node);
		}
		bool operator!=(const _MyIter&rhs)
		{
			return !(*this == rhs);
		}
	};

	//迭代器，非常量版本
	template<class T>
	class forward_list_iterator :public forward_list_const_iterator<T>
	{
	public://内嵌型别
		typedef T value_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef forward_list_iterator<T> _MyIter;
		typedef forward_list_const_iterator<T> _MyBase;
		typedef _node<T>*  nodePtr;
	public: //构造函数
		forward_list_iterator() :forward_list_const_iterator() {}
		explicit forward_list_iterator(const nodePtr&n) :forward_list_const_iterator(n) {}
		forward_list_iterator(const _MyIter&other) :forward_list_const_iterator(other.node) {}
		//
		_MyIter&operator=(const _MyIter&rhs)
		{
			node = rhs.node;
			return *this;
		}
		//迭代器操作
		_MyIter&operator++()
		{
			++*((_MyBase*)this);
			return *this;
		}
		_MyIter operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

	};


	//forward_list 模板定义
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class forward_list
	{
		//内嵌型别定义
	public:
		typedef T          value_type;
		typedef T&         reference;
		typedef const T&   const_reference;
		typedef T*         pointer;
		typedef const T*   const_pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t diference_type;
		
		typedef forward_list_const_iterator<T> const_iterator;
		typedef forward_list_iterator<T>  iterator;
		typedef MyCppSTL::allocator<_node<T>> _node_alloc;      //节点分配器

		typedef _node<T>* _nodePtr;
	public:
		explicit forward_list()
		{
			cur = creat_node();
			head = tail = cur;
		}
		forward_list(size_type count, const T&value)
		{
			creat_node(count, value, head, tail);
		}
		explicit forward_list(size_type count)
		{
			creat_node(count, T(), head, tail);
		}
		template<class InputIt>
		forward_list(InputIt first, InputIt last)
		{
			_forward_list_dispatch(first, last, head, tail, std::is_integral<InputIt>::type());
		}
		//copy construct
		forward_list(const forward_list<T>&other)
		{
			_forward_list_dispatch(other.begin(), other.end(), head, tail, std::false_type());
		}
		forward_list(const std::initializer_list<int>&other)
		{
			_forward_list_dispatch(other.begin(), other.end(), head, tail, std::false_type());
		}

		//析构
		~forward_list()
		{
			
		}
		//迭代器
		iterator before_begin() { return iterator(--head); }
		iterator begin(){return iterator(head);}
		const_iterator begin() const{return const_iterator(head);}
		const_iterator cbegin() const{return const_iterator(head);}
		iterator end(){return iterator(tail);}
		const_iterator end() const { return const_iterator(tail); }
		const_iterator cend() const { return const_iterator(tail); }
		//辅助函数
	private: 
		_nodePtr creat_node()//构造链表节点
		{
			_nodePtr node_tmp = _node_alloc::allocate();
			return node_tmp;
		}
		_nodePtr creat_node(const T&value)
		{
			_nodePtr tmp = creat_node();
			MyCppSTL::construct(&(tmp->data), value);
			return tmp;
		}
		void destroy_node(_nodePtr node)
		{
			_node_alloc::destroy(node);
			_node_alloc::deallocate(node);
		}
		_nodePtr creat_node(size_type count, const T&value,_nodePtr& head,_nodePtr& tail)
		{
			_nodePtr head_tmp = creat_node(value);
			_nodePtr cur_tmp = head_tmp;
			_nodePtr tmp = cur_tmp;
			while (--count)
			{
				tmp = creat_node(value);
				cur_tmp->_next = tmp;
				cur_tmp = tmp;
			}
			tmp = creat_node();   //
			cur_tmp->_next = tmp;
			tmp->_next = NULL;  //这里是因为在分配空间的时候，分配器也是一个链表，内存池
			head = head_tmp;
			tail = tmp;

			return head_tmp;
		}
		template<class Integer>
		void _forward_list_dispatch(Integer count, Integer value, _nodePtr& head,_nodePtr& tail,std::true_type)
		{
			creat_node(count, value, head, tail);
		}
		template<class InputIter>
		void _forward_list_dispatch(InputIter first, InputIter last, _nodePtr& head, _nodePtr& tail,std::false_type)
		{
			_nodePtr head_tmp = creat_node(*first++);
			_nodePtr cur_tmp = head_tmp;
			_nodePtr tmp = cur_tmp;
			for(; first != last; ++first)
			{
				tmp=creat_node(*first);
				cur_tmp->_next = tmp;
				cur_tmp = tmp;
			}
			tmp = creat_node();   //
			cur_tmp->_next = tmp;
			tmp->_next = NULL; 
			head = head_tmp;
			tail = tmp;
		}

	private:
		_nodePtr head;     
		_nodePtr tail;
		_nodePtr cur; //指向链表节点的指针

	};



}// end of namespace

#endif   //end of forward_list.h file