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
		typedef MyCppSTL::list_iterator<T, alloc>	iterator;
		typedef MyCppSTL::const_list_iterator<T, alloc> const_iterator;
		typedef MyCppSTL::allocator<__list_node<T>>	alloc_type;   //分配节点用
		typedef list<T, alloc>					    mylist;
		typedef T*									pointer;
		typedef const T*							const_pointer;
		typedef __list_node<T>*						NodePtr;
		typedef __list_node<T>  					ListNode;
	private:
		NodePtr cur;	//节点指针
		NodePtr end;    //尾后指针
	public:
		list() //默认构造函数
		{
			cur = creat_node(T());
			cur->_pre = cur;
			cur->_next = cur;
		}   
		list(size_type count, const T& value) //创建count个节点，每个节点的值为value
		{
			cur = creat_node(count, value);
			end = cur->_pre;
		}
		explicit list(size_type count) //创建count个节点，使用默认值
		{
			cur= creat_node(count, T());
			end = cur->_pre;
		}
		template< class InputIt >
		list(InputIt first, InputIt last)   //接受一对范围，要与接受两个整数想区别
		{
			list_aux(first, last, std::is_integral<InputIt>::type());
		}
		//赋值构造函数,使用迭代器来进行拷贝
		list(const list& other)
		{

		}
		//移动构造函数
		list(list&& other) :cur(other.cur), end(other.end)
		{
			other.cur = other.end = 0;
		}
		//列表初始化
		list(std::initializer_list<T> init)
		{
			list_aux(std::begin(init), std::end(init), std::false_type());
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
			NodePtr head = cur_tmp;
			while (count--)     //建立链表
			{
				cur = creat_node(x);
				cur->_pre = cur_tmp;
				cur_tmp->_next = cur;
				cur_tmp = cur;
			}
			cur->_next = head;
			head->_pre = cur;
			return head->_next;
		}
	
		//销毁一个节点
		void destory_node(const NodePtr&p)
		{
			alloc_type::destroy(&(p->data));
			alloc_type::deallocate(p);
		}
		//list接受一对迭代器范围构造函数
		template< class InputIt >
		void list_aux(InputIt first, InputIt last,std::false_type)   
		{
			NodePtr cur_tmp = creat_node(T());  //尾后节点
			NodePtr head = cur_tmp;
			for (; first != last; ++first)
			{
				cur = creat_node(*first);
				cur->_pre = cur_tmp;
				cur_tmp->_next = cur;
				cur_tmp = cur;
			}
			cur->_next = head;
			head->_pre = cur;
			cur = head->_next;
			end = cur->_pre;
		}

		template< class InputIt >
		void list_aux(InputIt count, InputIt value, std::true_type)
		{
			cur=creat_node(count, value);
			end = cur->_pre;
		}




	};


}






#endif
