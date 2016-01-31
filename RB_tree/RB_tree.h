//红黑树，set map等关联容器的底层实现


#ifndef RB_TREE_H
#define RB_TREE_H

#include"Allocator.h"
#include"Iterator.h"

namespace MyCppSTL
{

	using _rb_tree_color_type=bool;
	const _rb_tree_color_type _rb_tree_red=false;
	const _rb_tree_color_type _rb_tree_black=true;

	struct _rb_tree_node_base
	{
		using color_type = _rb_tree_color_type;
		using base_ptr=_rb_tree_node_base*;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;
	};

	template<typename T>
	struct _rb_tree_node:public _rb_tree_node_base   //分层结构，是因为红黑树的节点与对象值得类型无关。
	{
		using link_type=_rb_tree_node<T>*;
		T value_field;
	};

	_rb_tree_node_base* nil;    //哨兵节点

	// 
	struct _rb_tree_base_iterator
	{
	 	//内嵌型别定义
	 	using base_ptr = _rb_tree_node_base::base_ptr;      //树节点指针
	 	using iterator_category=MyCppSTL::bidirectional_iterator_tag; //迭代器型别
		using difference_type = std::ptrdiff_t;            //差
		base_ptr node;
	 	//
	 	void increment();
	 	void decrement();     
	};



}  //end of MyCppSTL

#include"RB_tree.im.h"

#endif