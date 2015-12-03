#ifndef LIST_H
#define LIST_H

#include<initializer_list>
#include"Allocator.h"
#include"Iterator.h"
#include"Construct.h"

namespace MyCppSTL
{



	template<class T,class alloc=MyCppSTL::allocator<__list_node<T>>>
	class list
	{
		//内嵌型别
	public:
		typedef T									value_type;
		typedef T&									reference;
		typedef const T&							const_reference;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef MyCppSTL::list_iterator<T>			iterator;
		typedef MyCppSTL::const_list_iterator<T>	const_iterator;
		typedef MyCppSTL::allocator<__list_node<T>>	alloc_type;   //分配节点用
		typedef list<T, alloc>					    mylist;
		typedef T*									pointer;
		typedef const T*							const_pointer;
		typedef __list_node<T>*						NodePtr;
		typedef __list_node<T>  					ListNode;
	private:
		NodePtr head;	//头节点指针
		NodePtr tail;    //尾后指针
	public:
		list() //默认构造函数
		{
			head = creat_node(T());
			head->_pre = head;
			head->_next = head;
			tail = head;
		}   
		list(size_type count, const T& value) //创建count个节点，每个节点的值为value
		{
			head = creat_node(count, value);
			tail = head->_pre;
		}
		explicit list(size_type count) //创建count个节点，使用默认值
		{
			head = creat_node(count, T());
			tail = head->_pre;
		}
		template< class InputIt >
		list(InputIt first, InputIt last)   //接受一对范围，要与接受两个整数想区别
		{
			list_aux(first, last, std::is_integral<InputIt>::type());
		}
		//赋值构造函数,使用迭代器来进行拷贝
		list(const list& other)
		{
			list_aux(other.begin(), other.end(), std::false_type());
		}
		//移动构造函数
		list(list&& other) :head(other.cur), tail(other.tail)
		{
			other.head = other.tail = 0;
		}
		//列表初始化
		list(std::initializer_list<T> init)
		{
			list_aux(std::begin(init), std::end(init), std::false_type());
		}

		//析构函数,使用迭代器来销毁
		~list()
		{
			free();
		}

		//拷贝赋值
		mylist& operator=(const mylist&rhs)
		{
			if (this != &rhs)
			{
				auto tmp = rhs;  //使用赋值构造函数
				free();
				list_aux(tmp.begin(), tmp.end(), std::false_type());
			}
			return *this;
		}

		//移动赋值
		mylist& operator=(mylist&&rhs)
		{
			if (this != &rhs)
			{
				this->head = rhs.head;
				this->tail = rhs.tail;
				rhs.head = rhs.tail = 0;
			}
			return *this;
		}
		//列表赋值
		mylist& operator=(std::initializer_list<T> init)
		{
			list_aux(std::begin(init), std::end(init), std::false_type());
			return *this;
		}

		//迭代器
		
		iterator begin(){return iterator(head);}
		const_iterator begin() const{return const_iterator(head);}
		const_iterator cbegin() const{return const_iterator(head);}

		iterator end() { return iterator(tail); }
		const_iterator end() const { return const_iterator(tail); }
		const_iterator cend() const { return const_iterator(tail); }

		//元素获取
		reference front()
		{
			return head->data;
		}

		const_reference front() const
		{
			return head->data;
		}

		reference back()
		{
			return tail->_pre->data;
		}

		const_reference back()const
		{
			return tail->_pre->data;
		}
		
		//容量
		bool empty() const
		{
			return (head == tail);
		}

		size_type size() const
		{
			size_type count = 0;
			auto cur = head;
			while (head != tail)
			{
				++count;
				cur = cur->_next;
			}
			return count;
		}

		size_type max_size() const
		{
			return size();
		}
	private://辅助函数
		//构造一个节点
		NodePtr get_node()    
		{
			return alloc_type::allocate();  //分配一个节点
		}
		//创建节点
		NodePtr creat_node(const T&x) 
		{
			auto nPtr = get_node();
			construct(&(nPtr->data), x);  //使用全局函数construct
			return nPtr;
		}
		NodePtr creat_node(size_type count, const T&x)  //创建n个节点
		{
			NodePtr cur_tmp= creat_node(T());  //哨兵节点,也是尾后节点
			NodePtr cur = cur_tmp;
			while (count--)     //建立链表
			{
				head = creat_node(x);
				head->_pre = cur_tmp;
				cur_tmp->_next = head;
				cur_tmp = head;
			}
			head->_next = cur;
			cur->_pre = head;
			return cur->_next;
		}
	
		//销毁一个节点
		void destory_node(const NodePtr&p)
		{
			alloc_type::destroy(p);
			alloc_type::deallocate(p);
		}
		//销毁所有节点
		void free()
		{
			if (head)
			{
				auto cur = head;
				auto tmp = head->_next; //tmp指向下一个节点
				for (; tmp != tail; tmp = tmp->_next)
				{
					destory_node(cur);
					cur = tmp;
				}
				destory_node(cur);
			}
		}
		//
		//list接受一对迭代器范围构造函数
		template< class InputIt >
		void list_aux(InputIt first, InputIt last,std::false_type)   
		{
			NodePtr cur_tmp = creat_node(T());  //尾后节点
			NodePtr cur = cur_tmp;
			for (; first != last; ++first)
			{
				head = creat_node(*first);
				head->_pre = cur_tmp;
				cur_tmp->_next = head;
				cur_tmp = head;
			}
			head->_next = cur;
			cur->_pre = head;
			head = cur->_next;
			tail = head->_pre;
		}

		template< class InputIt >
		void list_aux(InputIt count, InputIt value, std::true_type)
		{
			head =creat_node(count, value);
			tail = head->_pre;
		}




	};


}






#endif
