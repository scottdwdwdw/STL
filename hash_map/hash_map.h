#ifndef  _HASH_MAP_H
#define _HASH_MAP_H

#include<utility>
#include"hash_function.h"    //可以在这个头文件中添加自己定义的hash函数来处理特殊的输入类型
#include"function.h"
#include"Allocator.h"
#include"hash_table.h"

namespace MyCppSTL
{
	template<typename key,
			typename value,
			typename HashFcn = MyCppSTL::hash<value>,
			typename EqualKey = MyCppSTL::equal_to<value>,
			typename alloc = MyCppSTL::allocator<value>>
	class hash_map
	{
	private:
		using ht = MyCppSTL::hash_table<std::pair<const key,value>, key, HashFcn, MyCppSTL::selectlst<std::pair<const key, value>>, EqualKey>;
		ht Rep;
	public://内嵌型别定义
		using data_type = value;
		using mapped_type = value;
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
		using iterator = typename ht::iterator;
		
		using const_iterator = typename ht::const_iterator;

	public:
		//构造函数
		hash_map() :Rep(100, hasher(), key_equal(), MyCppSTL::selectlst<std::pair<const key, value>>())
		{}
		explicit hash_map(size_type n) :Rep(n, hasher(), key_equal(), MyCppSTL::selectlst<std::pair<const key, value>>())
		{}
		template<typename InputIt>
		hash_map(InputIt first, InputIt last) : Rep(100, hasher(), key_equal(), MyCppSTL::selectlst<std::pair<const key, value>>())
		{
			Rep.insert_unique(first, last);
		}
		template<typename InputIt>
		hash_map(InputIt first, InputIt last,size_type n) : Rep(n, hasher(), key_equal(), MyCppSTL::selectlst<std::pair<const key, value>>())
		{
			Rep.insert_unique(first, last);
		}

		//成员函数
		size_type size() const { return Rep.size(); }
		size_type max_size() const { return Rep.max_size(); }
		bool empty() const { return Rep.empty(); }

		iterator begin() { return Rep.begin(); }
		iterator end() { return Rep.end(); }
		const_iterator begin() const { return Rep.begin(); }
		const_iterator end() const { return Rep.end(); }
		//
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

		void insert(const_iterator first, const_iterator last)
		{
			Rep.insert_unique(first, last);
		}

		iterator find(const key_type&key) { return Rep.find(key); }
		const_iterator find(const key_type&key)const { return Rep.find(key); }

		value& operator[](const key_type&key)
		{
			return Rep.find_or_insert(value_type(key, value())).second;
		}
		size_type count() const { return Rep.count(); }
		std::pair<iterator, iterator> equal_range(const key_type&key)
		{
			return Rep.equal_range(key);
		}
		std::pair<const_iterator, const_iterator> equal_range(const key_type&key)const
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

}  // 1MyCppSTL


#endif // ! _HASH_MAP_H

