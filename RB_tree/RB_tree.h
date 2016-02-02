#ifndef RB_TREE_H
#define RB_TREE_H

#include"Allocator.h"
#include"Iterator.h"

namespace MyCppSTL
{
	enum { RED = 0,BLACK = 1};
	template<typename T>    //红黑树节点的定义
	struct rb_node
	{
		using _Node= rb_node < T >;
		using _NodePtr= rb_node<T>*;
		using value_type= T;
		rb_node():left(NULL), right(NULL), parent(NULL), value(T()), COLOR(RED) {}
		_NodePtr left;
		_NodePtr right;
		_NodePtr parent;
		T value;
		int COLOR;

	};

	//迭代器设计
	template<typename T>
	struct RB_Tree_const_iterator :public iterator<bidirectional_iterator_tag, T>
	{
		//内嵌型别定义
		using value_type = const T;
		using reference = const T&;
		using pointer = const T*;
		using myIter = RB_Tree_const_iterator<T>;
		//成员
		rb_node<T>*_node;

		//构造函数
		RB_Tree_const_iterator() {}
		RB_Tree_const_iterator(const rb_node<T>* x) :_node(x) {}
		RB_Tree_const_iterator(myIter&right) :_node(right._node) {}

		//成员方法
		reference operator*() const
		{
			return _node->value;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		myIter& operator++() //前置递增
		{
			if (_node->right->parent == _node) //说明有右孩子
			{
				_node = _node->right;
				while (_node->left->parent == _node)_node = _node->left;
			}
			else //没有右孩子，回溯
			{
				rb_node<T>* temp = _node->parent;
				while (_node == temp->right)
				{
					_node = temp;
					temp = temp->parent;
				}//若寻到根节点，那么temp->right=null !=_node
			}
			return *this;
		}

		myIter operator++(int) //后置递增
		{
			auto temp = *this;
			++*this;
			return temp;
		}

		myIter& operator--() //前置递减
		{
			if (_node->left->parent == _node) //有左孩子
			{
				_node = _node->left;
				while (_node->right->parent == _node)_node = _node->right;
			}
			else
			{
				rb_node<T>* temp = _node->parent;
				while (temp->left == _node)
				{
					_node = temp;
					temp = temp->parent;
				}
			}
			return *this;
		}

		myIter operator--(int) //后置递减
		{
			auto temp = *this;
			--*this;
			return temp;
		}

			
	};

	template<typename T>
	struct RB_Tree_iterator :public RB_Tree_const_iterator<T>
	{
		//内嵌型别定义
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using myIter = RB_Tree_iterator<T>;
		using myBase = RB_Tree_const_iterator<T>;

		RB_Tree_iterator() :RB_Tree_const_iterator(){}
		RB_Tree_iterator(const rb_node<T>* x) :RB_Tree_const_iterator(x) {}
		RB_Tree_iterator(myIter&right) :RB_Tree_const_iterator(right) {}

		//成员函数
		reference operator*() const
		{
			return _node->value;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		myIter& operator++()
		{
			++(myBase)*this;
			return *this;
		}

		myIter operator++(int)
		{
			auto temp = *this;
			++*this;
			return temp;
		}

		myIter& operator--()
		{
			--(myBase)*this;
			return *this;
		}

		myIter operator--(int)
		{
			auto temp = *this;
			--*this;
			return temp;
		}
	};

	template<typename T,typename alloc=MyCppSTL::allocator<T>>
	class RB_Tree
	{
	public:
		using value_type = T;
		using reference = T&;
		using _Node = rb_node<T>;
		using _NodePtr = rb_node<T>*;
	private:
		_NodePtr root;
		_NodePtr nil;    //哨兵节点
	public:
		//
		RB_Tree()
		{
			nil=make_node(T());
			nil->COLOR=BLACK;
			root=nil;
			root->parent = nil;
		}
		void insert(_NodePtr&root,const T& key);   //插入函数
		_NodePtr& getRoot()
		{
			return root;
		}
		void tree_delete(_NodePtr position);
		_NodePtr RB_tree_successor(_NodePtr position) const;  //查找后继
		_NodePtr tree_minnum(_NodePtr position) const;
	private:
		//私有成员函数
		//旋转
		void left_rotate(_NodePtr&position);   //左旋转
		void right_rotate(_NodePtr&position);  //右旋转
		_NodePtr make_node(const T&key);     //构造节点
		void destory_node(_NodePtr position);  //删除节点
		void RB_insert_fixup(_NodePtr& position); //维护红黑树的性质
		void RB_delete_fixup(_NodePtr& position);//
		



	}; //end of class

	template<typename T, typename alloc>
	typename RB_Tree<T,alloc>::_NodePtr RB_Tree<T,alloc>::make_node(const T&key)
	{
		_NodePtr temp=new _Node;
		temp->left=temp->right=temp->parent=NULL;
		temp->COLOR=RED;
		temp->value=key;

		return temp;
	}

	template<typename T, typename alloc>
	void RB_Tree<T,alloc>::destory_node(_NodePtr position)
	{
		if(position)
		{
			free(position);
		}
	}

    //左旋转
	template<typename T, typename alloc>
	void RB_Tree<T,alloc>::left_rotate(_NodePtr&position)   //参照图来理解
	{
		_NodePtr rotate_temp=position->right;   //其中position->right是一定存在的，否则也就没必要旋转了

		position->right = rotate_temp->left; 
		if(rotate_temp->left!=nil)rotate_temp->left->parent=position;   //将y的左子树接到position上

		rotate_temp->parent=position->parent;
		if(position->parent==nil)root=rotate_temp;  //设置根节点
		else if(position==position->parent->left)position->parent->left=rotate_temp;
		else if(position==position->parent->right)position->parent->right=rotate_temp;
		

		rotate_temp->left=position;
		position->parent=rotate_temp;


	}

	//右旋转
	template<typename T, typename alloc >
	void RB_Tree<T,alloc>::right_rotate(_NodePtr&position)
	{
		_NodePtr rotate_temp=position->left;

		position->left=rotate_temp->right;
		if(rotate_temp->right!=nil)rotate_temp->right->parent=position;

		rotate_temp->parent=position->parent;
		if(position->parent==nil)root=rotate_temp;
		else if(position==position->parent->left)position->parent->left=rotate_temp;
		else if(position==position->parent->right)position->parent->right=rotate_temp;

		rotate_temp->right=position;
		position->parent=rotate_temp;
	}

	template<typename T, typename alloc = MyCppSTL::allocator<T>>
	void RB_Tree<T,alloc>::insert(_NodePtr&root,const T& key)  //插入元素
	{
		_NodePtr pre=root;
		_NodePtr cur=root;  
		_NodePtr insert_node=make_node(key);
		while(cur!=nil)
		{
			pre=cur;
			if(key<cur->value)cur=cur->left;
			else
				cur=cur->right;
		}
		insert_node->parent=pre;
		if(pre==nil)root=insert_node;
		else if(key<pre->value)pre->left=insert_node;
		else 
		{
			pre->right=insert_node;
		}
		insert_node->left=nil;
		insert_node->right=nil;
		insert_node->COLOR=RED;
		//插入完成，下面维护红黑树的性质
		RB_insert_fixup(insert_node);

	}

	template<typename T, typename alloc >   //维护树的性质
	void RB_Tree<T,alloc>::RB_insert_fixup(_NodePtr& position)
	{
		while(position->parent->COLOR==RED)  //这里能够保证根节点的颜色是黑色的
		{
			if(position->parent==position->parent->parent->left)  
			{
				if(position->parent->parent->right->COLOR==RED)  //case1,叔节点也是红色
				{
					position->parent->parent->right->COLOR=BLACK;
					position->parent->COLOR=BLACK;
					position->parent->parent->COLOR=RED;
					position=position->parent->parent;   //进入下一轮的循环
				}
				else if(position==position->parent->right)      //case 2 要将其转换为case3
				{
					position=position->parent;
					left_rotate(position);  //左旋转转换为了case3
				}
				else if(position==position->parent->left)//叔节点肯定是黑色的
				{
					position->parent->COLOR=BLACK;
					position->parent->parent->COLOR=RED;
					position=position->parent->parent;
					right_rotate(position);    //右旋转
				}
			}
			else if(position->parent=position->parent->parent->right)
			{
				if(position->parent->parent->left->COLOR==RED) //叔节点
				{
					 position->parent->parent->left->COLOR=BLACK;
					 position->parent->parent->COLOR=RED;
					 position->parent->COLOR=BLACK;
					 position=position->parent->parent;
				}
				else if(position==position->parent->left)   //case 2
				{
					position=position->parent;
					right_rotate(position);    //右旋转  ,转换为case3
				}
				else if(position==position->parent->right)
				{
					position->parent->COLOR=BLACK;
					position->parent->parent->COLOR=RED;
					position=position->parent->parent;
					left_rotate(position);   //左旋
				}
			}
		}

		root->COLOR=BLACK; //保持根为黑色
	}

	template<typename T, typename alloc>
    typename RB_Tree<T,alloc>::_NodePtr RB_Tree<T,alloc>::tree_minnum(_NodePtr position) const
    {
    	if(position->left==nil)return position;
    	return tree_minnum(position->left);
    }


	template<typename T, typename alloc >
	typename RB_Tree<T,alloc>::_NodePtr RB_Tree<T,alloc>::RB_tree_successor(_NodePtr position) const
	{
		if(position->right!=nil)return tree_minnum(position->right);
		while(position!=nil&&position==position->parent->right)position=position->parent;
		return position;
	}

	template<typename T,typename alloc>
	void RB_Tree<T,alloc>::tree_delete(_NodePtr position)
	{
		_NodePtr del_temp=nil;  //实际删除的节点
		_NodePtr repl_temp=nil;
		if(position->left==nil||position->right==nil)del_temp=position;
		else
		{
			del_temp=RB_tree_successor(position);  //删掉其后继
		}
		if(del_temp->left!=nil)repl_temp=del_temp->left;
		else
			repl_temp=del_temp->right;
		repl_temp->parent=del_temp->parent;     
		if(del_temp->parent==nil)root=repl_temp;
		else if(del_temp==del_temp->parent->left)del_temp->parent->left=repl_temp;
		else if(del_temp==del_temp->parent->right)del_temp->parent->right=repl_temp;

		if(del_temp!=position)
			{
				position->value=del_temp->value;
				//position->COLOR=del_temp->COLOR;  //待确认？
			}

		//下面是颜色的维护
		if(del_temp->COLOR==BLACK)
		{
			RB_delete_fixup(repl_temp);
		}

		//释放节点空间
		destory_node(del_temp);

	}

	template<typename T,typename alloc>
	void RB_Tree<T,alloc>::RB_delete_fixup(_NodePtr& position)
	{
		while(position!=root&&position->COLOR==BLACK) //只对position的color的颜色是black(双重黑色)的讨论，因为如果是red，则可以直接设置为black
		{
			if(position==position->parent->left)
			{
				_NodePtr bro=position->parent->right;
				if(bro->COLOR==RED) //case 1,兄弟节点有两个黑色孩子
				{
					bro->COLOR=BLACK;
					position->parent->COLOR=RED;
					left_rotate(position->parent); //convert to case2/3/4
					bro=position->parent->right;
				}
				if(bro->right->COLOR==BLACK&&bro->left->COLOR==BLACK) //case2,3个都是黑色的，那么position和bro都脱去黑色
				{
					bro->COLOR=RED;
					position=position->parent;//相当于把那一层黑色转移到position的父节点了，不管其父节点是什么颜色，如果是红色，即为红黑，退出循环，如果是黑色，则是黑黑，继续循环，把这个黑色继续往上层移动
					
				}
				else if(bro->right->COLOR==BLACK) //case 3,其左子的颜色是红色的，右子是黑色 convert to case 4
				{
					bro->left->COLOR=BLACK;
					bro->COLOR=RED;
					right_rotate(bro);
					bro=position->parent->right;
				}
				//bro->right->COLOR=RED      case 4  //右孩子是红色的

				bro->COLOR=position->parent->COLOR;
				position->parent->COLOR=BLACK;
				bro->right->COLOR=BLACK;
				left_rotate(position->parent);
				position=root;
			}
			else if(position==position->parent->right)//右孩子
			{
				 _NodePtr bro=position->parent->left;
				 if(bro->COLOR==RED)  //想法是要维护性质5
				 {
				 	bro->COLOR=BLACK;
				 	position->parent->COLOR=RED;
				 	right_rotate(position->parent);
				 	bro=position->parent->left;
				 }
				 if(bro->left->COLOR==BLACK&&bro->right->COLOR==BLACK)//case 2  bro->COLOR==BLACK
				 {
				 	bro->COLOR=RED;
				 	position=position->parent;
				 }
				 else if(bro->left->COLOR==BLACK) //bro->right->COLOR=RED; case 3
				 {
				 	bro->right->COLOR=BLACK;
				 	bro->COLOR=RED;
				 	left_rotate(bro);
				 	bro=position->parent->left;
				 }
				 //bro->left->COLOR==RED,bro->right->COLOR=RED   case 4
				 bro->COLOR=position->parent->COLOR;  //交换兄弟节点的颜色和其父节点的颜色，兄弟节点颜色肯定是黑色的
				 position->parent->COLOR=BLACK;
				 bro->left->COLOR=BLACK;
				 right_rotate(position->parent);
				 position=root;
			}
		}

		position->COLOR=BLACK;

	}


} //end of namespace





#endif
