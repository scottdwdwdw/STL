#ifndef _HASH_SET_H
#define _HASH_SET_H

#include<utility>
#include"hash_function.h"    //可以在这个头文件中添加自己定义的hash函数来处理特殊的输入类型
#include"function.h"
#include"Allocator.h"
#include"hash_table.h"

namespace MyCppSTL
{
	
	template<typename value,
			 typename HashFcn=MyCppSTL::hash<value>,
		     typename EqualKey=MyCppSTL::equal_to<value>,
			 typename alloc=MyCppSTL::allocator<value>>
	class hash_set
	{
	public:
		template<typename value, typename HashFcn, typename EqualKey, typename alloc>
		friend bool operator==(const hash_set<value, HashFcn, EqualKey, alloc>&lhs, const  hash_set<value, HashFcn, EqualKey, alloc>&rhs);
	private:
		using ht = MyCppSTL::hash_table<value, value, HashFcn, MyCppSTL::identity<value>, EqualKey, alloc>;
		ht Rep;
	public:
		using key_type = typename ht::key_type;
		using value_type = typename ht::value_type;
		using hasher = typename ht::hasher;
		using key_equal = typename ht::key_equal;
		using size_type = typename ht::size_type;
		using difference_type = typename ht::difference_type;
		using pointer = typename ht::pointer;
		using const_pointer = typename ht::const_pointer;
		using reference = typename ht::reference;
		using const_reference = typename ht::const_reference;
		using iterator = typename ht::const_iterator;
		using const_iterator = typename ht::const_iterator;
	public:
		//构造函数
		hash_set() :Rep(100, hasher(), key_equal(), MyCppSTL::identity<value>()) {}
		explicit hash_set(const size_type n) :Rep(n, hasher(), key_equal(), MyCppSTL::identity<value>()){}
		//
		template<typename InputIt>
		hash_set(InputIt first, InputIt last) : Rep(100, hasher(), key_equal(), MyCppSTL::identity<value>())
		{
			Rep.insert_unique(first, last);
		}
		template<typename InputIt>
		hash_set(InputIt first, InputIt last, size_type n) : Rep(n, hasher(), key_equal(), MyCppSTL::identity<value>())
		{
			Rep.insert_unique(first, last);
		}

		//
		size_type size() const { return Rep.size(); }
		size_type max_size() const { return Rep.max_size(); }
		bool empty() const { return Rep.empty(); }
		
		iterator begin() { return Rep.begin(); }
		iterator end() { return Rep.end(); }

		std::pair<iterator, bool> insert(const value_type& value)
		{
			std::pair<typename ht::iterator, bool> res = Rep.insert_unique(value);
			return std::pair<iterator, bool>(res.first, res.second);
		}

		template<typename InputIt>
		void insert(InputIt first, InputIt last)
		{
			Rep.insert_unique(first, last);
		}

		iterator find(const key_type&key)const { return Rep.find(key); }
		size_type count() const { return Rep.count(); }
		std::pair<iterator, iterator> equal_range(const key_type&key)const
		{
			return Rep.equal_range(key);
		}
		size_type erase(const key_type&key)
		{
			return Rep.erase(key);
		}
		void erase(iterator pos) { return Rep.erase(pos); }
		void erase(iterator first, iterator last) { return Rep.erase(first, last); }
		void clear() { return Rep.clear(); }

		void resize(size_type n) { return Rep.resize(n); }
		void bucket_count() const { return Rep.buckets_count(); }
		void elems_in_bucket(size_type n) const { return Rep.elems_in_bucket(n); }

	};

	//逻辑操作
	template<typename value,typename HashFcn,typename EqualKey,typename alloc>
	bool operator==(const hash_set<value, HashFcn, EqualKey, alloc>&lhs, const  hash_set<value, HashFcn, EqualKey, alloc>&rhs)
	{
		return lhs.Rep == rhs.Rep;
	}

	template<typename value, typename HashFcn, typename EqualKey, typename alloc>
	bool operator!=(const hash_set<value, HashFcn, EqualKey, alloc>&lhs, const  hash_set<value, HashFcn, EqualKey, alloc>&rhs)
	{
		return !(lhs == rhs);
	}
}


#endif // !_HASH_SET_H

