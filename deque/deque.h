#ifndef DEQUE_H
#define DEQUE_H

#include"Allocator.h"
#include"Iterator.h"

namespace MyCppSTL
{
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class deque {
	public:
		typedef  T							value_type;
		typedef  T&							reference_type;
		typedef  T*							pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque_const_iterator<T>		const_iterator;
		typedef deque_iterator<T>			iterator;
		typedef pointer*					map_pointer;
		typedef deque<T>					_MyDeque;
	public:
		//¹¹Ôìº¯Êý

	private:
		iterator _start;
		iterator _finish;
		map_pointer	_Map;
		size_type _MapSize;

	};


}




#endif
