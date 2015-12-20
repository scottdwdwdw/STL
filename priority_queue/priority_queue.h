#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H


#include<functional>
#include"Vector.h"
#include"Algorithm.h"


namespace MyCppSTL
{

	template<class T,class Cont=MyCppSTL::vector<T>,class Comp=std::less<typename Cont::value_type>>
	class priority_queue
	{
	public:
		//内嵌型别
		typedef typename Cont::value_type value_type;
		typedef typename Cont::reference  reference;
		typedef typename Cont::const_reference const_reference;
		typedef typename Cont::size_type  size_type;
	public:
		priority_queue(const Comp& compare, const Cont& cont) :cp(compare), c(cont)
		{
			MyCppSTL::make_heap(c.begin(), c.end());
		}
		explicit priority_queue(const Comp& compare = Comp(), Cont&& cont = Cont()) :cp(compare), c(cont)
		{
			MyCppSTL::make_heap(c.begin(), c.end());
		}
		priority_queue(const priority_queue& other) :cp(other.cp), c(other.c)
		{
			MyCppSTL::make_heap(c.begin(), c.end());
		}
		priority_queue(const priority_queue&& other) :cp(std::move(other.cp)), c(std::move(other.c))
		{
			MyCppSTL::make_heap(c.begin(), c.end());
		}
		template<class InputIt>
		priority_queue(InputIt first, InputIt last, const Comp& compare, const Cont& cont):cp(compare),c(cont)
		{
			c.insert(c.end(), first, last);
			MyCppSTL::make_heap(c.begin(), c.end());
		}
		template< class InputIt >
		priority_queue(InputIt first, InputIt last, const Comp& compare = Comp(), Cont&& cont = Cont()):cp(compare),c(std::move(cont))
		{
			c.insert(c.end(), first, last);
			MyCppSTL::make_heap(c.begin(), c.end());
		}

		~priority_queue()
		{
		}

		//copy assignment
		priority_queue<T>&operator=(const priority_queue<T>&other)
		{
			c = other.c;
			cp = other.cp;
			//MyCppSTL::make_heap(c.begin(), c.end());
			return *this;
		}
		priority_queue<T>&operator=(priority_queue<T>&&other)
		{
			c = std::move(other.c);
			cp = std::move(other.cp);
			return *this;
		}

		//top
		const_reference top() const
		{
			return c.front();
		}
		//empty
		bool empty() const
		{
			return c.empty();
		}
		//size
		bool size() const
		{
			return c.size();
		}
		//push
		void push(const value_type& value)
		{
			c.push_back(value);
			MyCppSTL::push_heap(c.begin(), c.end());
		}
		//
		void push(value_type&& value)
		{
			c.push_back(std::move(value));
			MyCppSTL::push_heap(c.begin(), c.end());
		}
		//pop
		void pop()
		{
			MyCppSTL::pop_heap(c.begin(), c.end());
			c.pop_back();
		}

		void swap(priority_queue<T>&other)
		{
			using std::swap;
			c.swap(other.c);   //因为清楚，该容器有swap,暂时采取这样的做法，
			swap(cp, other.cp);
		}

	private:
		Comp cp;
		Cont c;
	};//end of priority_queue
	//非成员函数
	template<class T,class Container= MyCppSTL::vector<T>,class Compare=std::less<T>>
	void swap(priority_queue<T, Container, Compare>&lhs, priority_queue<T, Container, Compare>&rhs)
	{
		lhs.swap(rhs);
	}
}//end of namespace

#endif   //end of priority_queue.h file