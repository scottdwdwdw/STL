//RB_tree.h的实现文件

namespace MyCppSTL
{
	void MyCppSTL:: _rb_tree_base_iterator::increment()
	{
		if(node->right!=nil)//有右孩子，找最小
		{
			node=node->right;
			while(node->left!=nil)node=node->left;
		}
		else  //没有右孩子，找其父节点，满足父节点的右孩子不是该节点
		{
			base_ptr y=node->parent;
			while(y->right!=node)
			{
				node=y;
				y=y->parent;
			}
			if(y==nil)node=nil;
		}
	}

	void MyCppSTL::_rb_tree_base_iterator::decrement()
	{
		if(node->left!=nil) //左子节点不是nil,找左子树的最大值
		{
			node=node->left;
			while(node->right!=nil)node=node->right;
		}
		else    //回溯找父节点，且满足，其父节点的左孩子不是该节点
		{
			base_ptr y=node->parent;
			while(node!=y->right)
			{
				node=y;
				y=y->parent;
			}
			if(y==nil)node=nil;   //
		}
	}
}