#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include"Allocator.h"
#include"Iterator.h"
#include<initializer_list>
#include<type_traits>

namespace MyCppSTL
{

	template<class T, class alloc>class forward_list; //前向声明

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
	public:
		friend forward_list<T, MyCppSTL::allocator<T>>;
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
		reference operator*()
		{
			return (reference)**(_MyBase*)this;
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
		typedef forward_list<T, alloc> _MyList;
		typedef MyCppSTL::allocator<_node<T>> _node_alloc;      //节点分配器


		typedef _node<T>* _nodePtr;
	public:
		explicit forward_list()
		{
			cur = creat_node();
			head = cur;
			head->_next = NULL;
		}
		forward_list(size_type count, const T&value)
		{
			creat_node(count, value, head);
		}
		explicit forward_list(size_type count)
		{
			creat_node(count, T(), head);
		}
		template<class InputIt>
		forward_list(InputIt first, InputIt last)
		{
			_forward_list_dispatch(first, last, head,std::is_integral<InputIt>::type());
		}
		//copy construct
		forward_list(const forward_list<T>&other)
		{
			_forward_list_dispatch(other.begin(), other.end(), head,std::false_type());
		}
		forward_list(const std::initializer_list<int>&other)
		{
			_forward_list_dispatch(other.begin(), other.end(), head,std::false_type());
		}
		
		_MyList operator=(const _MyList&other)
		{
			if (this != &other)
			{
				destroy_all(head); //
				_forward_list_dispatch(other.begin(), other.end(), head, std::false_type());
			}
			return *this;
		}
		_MyList operator=(_MyList&&other)
		{
			head = other.head;
			cur = other.cur;
			other.head = NULL;
			other.cur = NULL;
		}
		_MyList &operator=(std::initializer_list<T> ilist)
		{
			_forward_list_dispatch(ilist.begin(), ilist.end(), std::false_type());
			return *this;
		}
		//析构
		~forward_list()
		{
			destroy_all(head);
		}
		//迭代器
		iterator before_begin() { return iterator(head); }
		iterator begin(){return iterator(head->_next);}
		const_iterator begin() const{return const_iterator(head->_next);}
		const_iterator cbegin() const{return const_iterator(head->_next);}
		iterator end(){return iterator();}
		const_iterator end() const { return const_iterator(); }
		const_iterator cend() const { return const_iterator(); }
		//access
		reference front()
		{
			return *begin();
		}
		const_reference front() const
		{
			return *begin();
		}
		bool empty() const
		{
			return (head->_next == NULL);
		}
		void clear()
		{
			destroy_all(head);
		}

		
		iterator insert_after(const_iterator pos, const T&value)
		{
			return insert_after_aux(pos, value);
		}

		/* 关于move语义。。。
		iterator insert_after(const_iterator pos, T&&value)
		{
			return insert_after_aux(pos, value);
		}
		*/
		iterator insert_after(const_iterator pos, size_type count, const T&value)
		{
			auto tmp = pos;
			while (count--)
			{
				insert_after(pos, value);
				pos++;
			}
			return iterator(tmp.node->_next);
		}
		template<class InputIt>
		iterator insert_after(const_iterator pos, InputIt first, InputIt last)
		{
			return (insert_after_dispatch(pos, first, last, std::is_integral<InputIt>::type()));
		}

		iterator insert_after(const_iterator pos, std::initializer_list<T>ilist)
		{
			return (insert_after(pos, ilist.begin(), ilist.end()));
		}

		void push_front(const T&value)
		{
			insert_after(before_begin(), value);
		}

		//move版本
		/**/
		iterator erase_after(const_iterator&pos)
		{
			auto tmp = pos.node->_next;
			pos.node->_next = pos.node->_next->_next;
			tmp->_next = NULL;
			destroy_node(tmp);
			return iterator(pos.node->_next);
		}

		iterator erase_after(const_iterator&first, const_iterator&last)
		{
			
			for (;  first.node->_next!= last.node;)
			{
				erase_after(first);
			}
			return iterator(first.node);
		}

		void pop_front()
		{
			erase_after(before_begin());
		}

		size_type size()
		{
			size_type count = 0;
			while (head->_next != NULL)
			{
				++count;
			}
			return(count);
		}

		void resize(size_t count,const value_type&value)
		{
			size_t len = 0;
			auto it = before_begin();
			for (; next(it)!=end() && len < count; ++it, ++len) 
			{

			}
			if (len == count) //当前长度长于count
			{
				erase_after(it,end());
			
			}
			else //长度大于count
			{
				while (len != count)
				{
					insert_after(it,value);
					++it;
					++len;
				}
			}
		}

		void resize(size_t count)
		{
			resize(count, T());
		}

		void swap(_MyList&other)
		{
			auto tmp = head;
			auto tmp_cur = cur;
			head = other.head;
			cur = other.cur;
			other.head = tmp;
			other.cur = tmp_cur;
		}
		
		template<class...Args>
		iterator emplace_after(iterator pos, Args&& ...args)
		{
			return insert_after_aux(pos, std::forward<Args>(args)...);
		}

		template<class...Args>
		iterator emplace_front(Args&&...args)
		{
			return insert_after_aux(before_begin(), std::forward<Args>(args)...);
		}
		
		//辅助函数
	private: 

		iterator next(iterator it)
		{
			return ++it;
		}
	
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
		void destroy_node(_nodePtr &node)
		{
			_node_alloc::destroy(node);
			_node_alloc::deallocate(node);
		}
		void destroy_all(_nodePtr&head)
		{
			auto before_head = head;
			head = head->_next;
			auto tmp = head;
			while (tmp != NULL)
			{
				tmp = tmp->_next;
				destroy_node(head);
				
				head = tmp;
			}
			_node_alloc::deallocate(before_head);
		}
		_nodePtr creat_node(size_type count, const T&value,_nodePtr& head)
		{
			_nodePtr head_tmp = creat_node();
			_nodePtr cur_tmp = head_tmp;
			_nodePtr tmp = cur_tmp;
			while (count--)
			{
				tmp = creat_node(value);
				cur_tmp->_next = tmp;
				cur_tmp = tmp;
			}
			tmp->_next = NULL; 
			head = head_tmp;
			return head_tmp;
		}
		template<class Integer>
		void _forward_list_dispatch(Integer count, Integer value, _nodePtr& head,std::true_type)
		{
			creat_node(count, value, head, tail);
		}
		template<class InputIter>
		void _forward_list_dispatch(InputIter first, InputIter last, _nodePtr& head,std::false_type)
		{
			_nodePtr head_tmp = creat_node();
			_nodePtr cur_tmp = head_tmp;
			_nodePtr tmp = cur_tmp;
			for(; first != last; ++first)
			{
				tmp=creat_node(*first);
				cur_tmp->_next = tmp;
				cur_tmp = tmp;
			}
			tmp->_next = NULL; 
			head = head_tmp;
		}

		iterator insert_after_aux(const_iterator pos, const T&value)
		{
			auto insert_node = creat_node(value);
			insert_node->_next = pos.node->_next;
			pos.node->_next = insert_node;
			return iterator(pos.node->_next);
		}
		template<class InputIt>
		iterator insert_after_dispatch(const_iterator pos, InputIt first, InputIt last,std::false_type)
		{
			auto tmp = pos;
			for(; first != last; ++first)
			{
				insert_after_aux(pos, *first);
				++pos;
			}
			return iterator(tmp.node->_next);
		}

		template<class Integral>
		iterator insert_after_dispatch(const_iterator pos, Integral count, Integral value, std::true_type)
		{
			auto tmp = pos;
			while (count--)
			{
				insert_after_aux(pos, value);
				++pos;
			}
			return iterator(tmp.node->_next);
		}

	private:
		_nodePtr head;     
		_nodePtr cur; //指向链表节点的指针

	};



}// end of namespace

#endif   //end of forward_list.h file