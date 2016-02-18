#ifndef _FUNCTION_H
#define _FUNCTION_H


#include<utility>

namespace MyCppSTL
{

	template<typename Arg,typename Result>
	struct unary_function
	{
		using argument_type = Arg;
		using result_type = Result;
	};

	template<typename Arg1,typename Arg2,typename Result>
	struct binary_function
	{
		using first_argument_type = Arg1;
		using second_argument_type = Arg2;
		using result_type = Result;
	};


	template<typename T>
	struct identity :public unary_function<T, T>
	{
		const T& operator()(const T&x) const { return x; }
	};

	template<typename Pair>
	struct selectlst :public unary_function<Pair, typename Pair::first_type>
	{
		const typename Pair::first_type&operator()(const Pair&x)const
		{
			return x.first;
		}
	};

}   // end of namespace


#endif // !_FUNCTION_H

