#ifndef RB_TREE_H
#define RB_TREE_H

#include"Allocator.h"
#include"Iterator.h"
#include<utility>

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
		RB_Tree_const_iterator(rb_node<T>* x) :_node(x) {}
		RB_Tree_const_iterator(myIter&right) :_node(right._node) {}

		rb_node<T>* getNode() const
		{
			return _node;
		}
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
				_node = temp;
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
				//不用表示begin
			}
			return *this;
		}

		myIter operator--(int) //后置递减
		{
			auto temp = *this;
			--*this;
			return temp;
		}

		//逻辑操作
		bool operator==(myIter&right) const
		{
			return _node == right._node;
		}
		bool operator!=(myIter&right) const
		{
			return !(*this == right);
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
		RB_Tree_iterator(rb_node<T>* x) :RB_Tree_const_iterator(x) {}   //接受一个rb_node节点的类型转换
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
			++*(myBase*)this;
			//++((myBase)this);
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
			--*(myBase*)this;
			return *this;
		}

		myIter operator--(int)
		{
			auto temp = *this;
			--*this;
			return temp;
		}
	};


	/*
	有一个nil节点，节点的parent指向NULL,令left指向最右的元素，考虑end()的实现。
	*/
	template<typename T,typename alloc=MyCppSTL::allocator<T>>
	class RB_Tree
	{
	public:
		using value_type = T;
		using key_type = T;     //需修改
		using reference = T&;
		using const_reference = const T&;
		using _Node = rb_node<T>;
		using _NodePtr = rb_node<T>*;
		using iterator = RB_Tree_iterator<T>;
		using rb_tree_node_alloc = MyCppSTL::allocator<rb_node<T>>;
		using const_iterator = RB_Tree_const_iterator<T>;
		using size_type = std::size_t;
		using difference = std::ptrdiff_t;
		using myTree = RB_Tree<T>;
		using pointer = T*;
		using const_pointer = const T*;
		
	private:
		_NodePtr root;         //数根节点
		_NodePtr nil;          //哨兵节点
		size_type node_count;  //节点计数
		
	public:
		//
		RB_Tree()
		{
			nil=make_node(T());
			nil->COLOR=BLACK;
			root=nil;
			root->parent = nil;
		}
		RB_Tree(RB_Tree<T>&right) //copy construct
		{
			nil = make_node(T());
			nil->COLOR = BLACK;
			root = nil;
			root->parent = nil;
			tree_clone(right.getRoot(), nil);
			node_count = right.node_count;
		}

		~RB_Tree()
		{
			clear();
			if (nil)
			{
				destory_node(nil);
			}
		}


		//拷贝操作符
		myTree operator=(const myTree&right);

		//树为空
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }

		
		_NodePtr& getRoot() const 
		{  
			return _NodePtr(root); //为什么
		}
		void tree_delete(_NodePtr position);
		iterator insert_equal(const T&key);    //插入元素，允许有重复元素
		std::pair<iterator,bool> insert_unique(const T& key);  //插入元素，不允许重复元素
		template<typename InputIter>
		void insert_unique(InputIter first, InputIter last);
		void erase(iterator position);
		size_type erase(const key_type&x);
		void erase(iterator first, iterator last);
		_NodePtr RB_tree_successor(_NodePtr position) const;  //查找后继
		_NodePtr tree_minnum(_NodePtr position) const;  

		//迭代器
		iterator begin() { return iterator(tree_minnum(getRoot())); }
		const_iterator begin() const 
		{ 
			auto res = tree_minnum(getRoot());
			return const_iterator(res); 
		}
		const_iterator cbegin() const {return const_iterator(tree_minnum(getRoot()));}
		iterator end() { return iterator(nil); }
		const_iterator end() const { return const_iterator(nil); }
		const_iterator cend() const { return const_iterator(nil); }

		//swap
		void swap(RB_Tree<T, alloc>&right)
		{
			std::swap(root, right.root);
			std::swap(nil, right.nil);
			std::swap(node_count, right.node_count);
		}

		//查找,树的查找
		iterator find(const T&value) 
		{
			iterator it = begin();
			while (it != end())
			{
				if (*it == value)return it;
				++it;
			}

			return end();
		}

	private:
		//私有成员函数
		//旋转
		iterator __insert(_NodePtr&root,const T& key);   //插入元素
		void left_rotate(_NodePtr&position);   //左旋转
		void right_rotate(_NodePtr&position);  //右旋转
		_NodePtr make_node(const T&key);     //构造节点
		void destory_node(_NodePtr position);  //删除节点
		void RB_insert_fixup(_NodePtr& position); //维护红黑树的性质
		void RB_delete_fixup(_NodePtr& position);//
		_NodePtr node_clone(_NodePtr position); //节点的拷贝
		_NodePtr tree_clone(_NodePtr position, _NodePtr parent); //树的拷贝
		
		void clear();  //树的销毁(非const版本)


	}; //end of class

	template<typename T,typename alloc>
	inline typename RB_Tree<T, alloc>::_NodePtr RB_Tree<T, alloc>::node_clone(_NodePtr position)
	{
		_NodePtr temp = make_node(position->value);
		temp->COLOR = position->COLOR;
		temp->left = temp->right = temp->parent = NULL;
		return temp;
	}

	template<typename T, typename alloc>
	typename RB_Tree<T,alloc>::_NodePtr RB_Tree<T,alloc>::make_node(const T&key)
	{
		/*
		_NodePtr temp=new _Node;
		temp->left=temp->right=temp->parent=NULL;
		temp->COLOR=RED;
		temp->value=key;
		*/
		_NodePtr temp = rb_tree_node_alloc::allocate();
		temp->left = temp->right = temp->parent = NULL;
		temp->COLOR = RED;
		MyCppSTL::construct(&temp->value,key);
		return temp;
	}

	template<typename T, typename alloc>
	void RB_Tree<T,alloc>::destory_node(_NodePtr position)
	{
		if(position)
		{
			//free(position);
			MyCppSTL::destroy(&position->value);
			rb_tree_node_alloc::deallocate(position);
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
	typename RB_Tree<T,alloc>::iterator RB_Tree<T,alloc>::__insert(_NodePtr&root, const T& key)  //插入元素
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
		
		

		return iterator(insert_node);

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
    	if(position->parent == position||position->left==nil )return position;   //更改，防止树为空的情况
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


	//插入元素，允许有重复元素
	template<typename T,typename alloc>
	inline typename RB_Tree<T, alloc>::iterator RB_Tree<T, alloc>::insert_equal(const T&key)
	{

		auto it = __insert(getRoot(), key);
		++node_count;
		return it;
	}

	template<typename T,typename alloc>
	inline std::pair<typename RB_Tree<T, alloc>::iterator, bool> RB_Tree<T, alloc>::insert_unique(const T&key)
	{
		auto it = find(key);
		if (it == end())//可以插入
		{
			it = __insert(getRoot(), key);
			++node_count;
			return std::make_pair(it,true);
		}
		else //不可以插入
		{
			return std::make_pair(it, false);   //返回的是该元素在该树中的位置的迭代器
		}
	}


	//树的拷贝
	template<typename T,typename alloc>
	typename RB_Tree<T, alloc>::_NodePtr RB_Tree<T, alloc>::tree_clone(_NodePtr position, _NodePtr p)
	{
		_NodePtr temp = node_clone(position);
		temp->parent = p;      //设置父节点
		if (root == nil)
			root = temp;   //设置根节点
	

		if (position->right->parent == position)  //有右子树
		{
			temp->right = tree_clone(position->right, temp);
		}
		else
		{
			temp->right = nil;  //没有右子树，加上叶子节点
		}
		p = temp;
		//position = position->left;   //转到左子树
	
		while (position->left->parent == position)
		{
			position = position->left;
			_NodePtr temp2 = tree_clone(position, p);
			p->left = temp2;
			temp2->parent = p;
			if (position->right->parent==position)
				temp2->right = tree_clone(position->right, temp2);
			p = temp2;
		//	position = position->left;
		}
		p->left = nil;//没有左子树了，加上叶子节点


	
		return temp; //注意返回的节点是temp,而不是p
	}

	//树拷贝运算符
	
	template<typename T,typename alloc>
	typename RB_Tree<T, alloc>::myTree RB_Tree<T, alloc>::operator=(const myTree&right)
	{
		if (this != &right)
		{
			clear(begin());   //销毁整棵树
			tree_clone(right.root, nil);
			node_count = right.node_count;
		}
		return *this;
	}
	

	//树的销毁
	template<typename T,typename alloc>
	void RB_Tree<T, alloc>::clear()
	{
		auto it = begin();
		auto it_pre = it++;
		while (it != end())  //红黑树的实现有误
		{
			erase(it_pre);
			it_pre = it++;	
		}

	}

	template<typename T,typename alloc>
	template<typename InputIter>
	void RB_Tree<T, alloc>::insert_unique(InputIter first, InputIter last)
	{
		for (; first != last; ++first)
		{
			insert_unique(*first);
		}
	}

	//删除节点
	template<typename T,typename alloc>
	inline void RB_Tree<T, alloc>::erase(iterator position)
	{
		tree_delete(position.getNode());  //
	}

	/*
	template<typename T,typename alloc>
	inline RB_Tree<T, alloc>::size_type RB_Tree<T, alloc>::erase(const key_type&x)
	{
		
	}
	*/
	template<typename T,typename alloc>
	inline void RB_Tree<T, alloc>::erase(iterator first, iterator last)
	{
		auto it = first;
		while (it != last)erase(it++);
	}



} //end of namespace





#endif
