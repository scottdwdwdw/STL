#ifndef STACK_H
#define STACK_H

#include"deque.h"

namespace MyCppSTL
{

	template<class T, class Sequence>class stack;

	template<class T, class Sequence>
	bool operator==(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs);

	template<class T, class Sequence>
	bool operator!=(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs);

	template<class T, class Sequence>
	bool operator>(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs);

	template<class T, class Sequence>
	bool operator>=(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs);

	template<class T, class Sequence>
	bool operator<(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs);

	template<class T, class Sequence>
	bool operator<=(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs);

	template<class T,class Sequence=deque<T>>
	class stack
	{
	public:
		friend bool operator==<>(const stack<T>&lhs, const stack<T>&rhs);
		friend bool operator!=<>(const stack<T>&lhs, const stack<T>&rhs);
		friend bool operator> <>(const stack<T>&lhs, const stack<T>&rhs);
		friend bool operator< <>(const stack<T>&lhs, const stack<T>&rhs);
		friend bool operator>=<>(const stack<T>&lhs, const stack<T>&rhs);
		friend bool operator<= <>(const stack<T>&lhs, const stack<T>&rhs);

	public: //内嵌型别
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;


	//构造函数
		explicit stack(const Sequence& cont):container(cont)
		{
		}
		explicit stack(Sequence&& cont = Sequence()):container(std::move(cont))    
		{
		}
		stack(const stack<T>&rhs) :container(rhs.container)
		{
		}
		stack(stack<T>&&rhs) :container(std::move(rhs.container))
		{
		}

		~stack()
		{
		}

		stack<T>& operator=(const stack<T>&rhs)
		{
			if (this != &rhs)
			{
				container = rhs.container;
			}

			return *this;

		}

		reference top()
		{
			return container.back();
		}
		const_reference top() const
		{
			return container.back();
		}

		void push(const value_type& value)
		{
			container.push_back(value);
		}
		void push(const value_type&&value)
		{
			container.push_back(std::move(value));
		}
		void pop()
		{
			container.pop_back();
		}
		bool empty()
		{
			return container.empty();
		}

		size_type size()
		{
			return container.size();
		}

		void swap(stack<T>&rhs)
		{
			auto tmp = rhs.container;
			rhs.container = container;
			container = tmp;
		}



	private:
		Sequence container;
	};  //end of stack

		//非成员函数
	template<class T, class Sequence = MyCppSTL::deque<T>>
	bool operator==(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs)
	{
		return (lhs.container == rhs.container);
	}

	template<class T, class Sequence = MyCppSTL::deque<T>>
	bool operator!=(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs)
	{
		return !(lhs == rhs);
	}

	template<class T, class Sequence = MyCppSTL::deque<T>>
	bool operator<(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs)
	{
		return (lhs.container < rhs.container);
	}

	template<class T, class Sequence = MyCppSTL::deque<T>>
	bool operator>(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs)
	{
		return (rhs<lhs);
	}

	template<class T, class Sequence = MyCppSTL::deque<T>>
	bool operator<=(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs)
	{
		return !(rhs>lhs);
	}

	template<class T, class Sequence = MyCppSTL::deque<T>>
	bool operator>=(const stack<T, Sequence>&lhs, const stack<T, Sequence>&rhs)
	{
		return !(rhs<lhs);
	}

}  //end of namespace



#endif   //end of stack.h