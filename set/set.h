#ifndef SET_H
#define SET_H


#include "Allocator.h"
#include "RB_tree.h"
#include<utility>

namespace MyCppSTL
{
	template<typename Key,typename alloc=MyCppSTL::allocator<Key>>
	class set
	{
		//内嵌型别
	public:
		using key_type = Key;
		using value_type = Key;
	public:
		template<typename Key,typename alloc>
		friend bool operator==(const set<Key, alloc>&lhs, const set<Key, alloc>&rhs);
		template<typename Key,typename alloc>
		friend bool operator<(const set<Key, alloc>&lhs, const set<Key, alloc>&rhs);
	private:
		using rep_type = MyCppSTL::RB_Tree<Key>;
		rep_type t;//red-black tree representing set
	public:
		using pointer = typename rep_type::const_pointer;
		using const_pointer = typename rep_type::const_pointer;
		using reference = typename rep_type::const_reference;
		using const_reference = typename rep_type::const_reference;
		using iterator = typename rep_type::const_iterator;
		using const_iterator = typename rep_type::const_iterator;   //set 不允许改变键/值
		using size_type = typename rep_type::size_type;
		using difference = typename rep_type::difference;


		//构造函数
		set() :t() {}
		template <typename InputIter>
		set(InputIter first, InputIter last) : t()
		{
			t.insert_unique(first, last);  //插入
		}

		//copy construct
		set(const set<Key, alloc>&right) :t(right.t) {}  
		//copy operator
		set<Key, alloc>& operator=(const set<Key, alloc>& right)
		{
			t = right.t;
			return *this;
		}

		//iteratoror
		iterator begin() const { return t.begin(); }
		iterator end() const { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		
		void swap(set<Key, alloc>&right) { t.swap(right.t); }

		//insert/erase
		std::pair<iterator, bool> insert(const value_type&value)
		{
			std::pair<typename rep_type::iterator, bool> _p = t.insert_unique(value);
			return std::pair<iterator, bool>(_p.first, _p.second);
		}

		template<typename InputIter>
		void insert(InputIter first, InputIter last)
		{
			t.insert_unique(first, last);
		}

		void erase(iterator position)
		{
			using rep_iterator = typename rep_type::iterator;
			t.erase((rep_iterator&)position);
		}

		size_type erase(const key_type&value)
		{
			return t.erase(value);
		}

		void erase(iterator first, iterator last)
		{
			using rep_iterator = typename rep_type::iterator;
			t.erase((rep_iterator&)first, (rep_iterator&)last);
		}

		void clear() { t.clear(); }

		iterator find(const key_type&_key) const
		{
			return t.find(_key);
		}
		size_type count(const key_type&_key) const
		{
			return t.find(_key) == end() ? 0 : 1;
		}

		iterator lower_bound(const key_type&_key) const
		{
			return t.lower_bound(_key);
		}

		iterator upper_bound(const key_type&_key)const
		{
			return t.upper_bound(_key);
		}

		std::pair<iterator, iterator> equal_range(const key_type&_key) const
		{
			return std::pair<iterator, iterator>(lower_bound(_key), upper_bound(_key));
		}

		//


	};//end of set

	//逻辑操作
	template<typename Key,typename alloc=MyCppSTL::allocator<Key>>
	bool operator==(const set<Key, alloc>&lhs, const set<Key, alloc>&rhs)
	{
		return (lhs.t == rhs.t);
	}

	template<typename Key, typename alloc = MyCppSTL::allocator<Key>>
	bool operator!=(const set<Key, alloc>&lhs, const set<Key, alloc>&rhs)
	{
		return !(lhs==rhs);
	}

	template<typename Key, typename alloc = MyCppSTL::allocator<Key>>
	bool operator<(const set<Key, alloc>&lhs, const set<Key, alloc>&rhs)
	{
		return (lhs.t < rhs.t);
	}

	template<typename Key, typename alloc = MyCppSTL::allocator<Key>>
	bool operator>(const set<Key, alloc>&lhs, const set<Key, alloc>&rhs)
	{
		return (lhs.t > rhs.t);
	}

	template<typename Key, typename alloc = MyCppSTL::allocator<Key>>
	bool operator<=(const set<Key, alloc>&lhs, const set<Key, alloc>&rhs)
	{
		return (lhs.t <= rhs.t);
	}

	template<typename Key, typename alloc = MyCppSTL::allocator<Key>>
	bool operator>=(const set<Key, alloc>&lhs, const set<Key, alloc>&rhs)
	{
		return (lhs.t >= rhs.t);
	}

}





#endif
