#ifndef STRING_H
#define STRING_H

#include"Allocator.h"
#include"Iterator.h"
#include"Algorithm.h"
#include<initializer_list>
#include<iostream>

namespace MyCppSTL
{
//#define max(a,b) ((a)>(b))?(a):(b)

	class string_const_iterator:public iterator<MyCppSTL::random_access_iterator_tag,char>
	{
	public: //内嵌型别定义
		typedef const char value_type;
		typedef const char& reference;
		typedef const char* pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef string_const_iterator _MyIter;
	private: //数据
		pointer _p; //指向当前位置
	public:
		//构造函数
		string_const_iterator() :_p(0) {}   
		string_const_iterator(pointer p) :_p(p) {}
		string_const_iterator(const _MyIter&other)  //copy
		{
			if (this != &other) 
			{
				_p = other._p;
			}
		}
		//操作
		reference operator*() const
		{
			return *_p;
		}
		reference operator[](size_type n) const
		{
			return *(_p + n);
		}
		pointer operator->() const
		{
			return &*_p;
		}

		_MyIter&operator++() //前置递增
		{
			++_p;
			return *this;
		}
		_MyIter operator++(int) //后置递增
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		_MyIter&operator--() //前置递减
		{
			--_p;
			return *this;
		}
		_MyIter operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		_MyIter&operator+=(size_type n)
		{
			_p += n;
			return *this;
		}
		_MyIter operator+(size_type n)
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}
		_MyIter&operator-=(size_type n)
		{
			_p -= n;
			return *this;
		}
		_MyIter operator-(size_type n)
		{
			auto tmp = *this;
			tmp -= n;
			return tmp;
		}

		difference_type operator-(_MyIter&other)
		{
			return (_p - other._p);
		}

		//逻辑操作
		bool operator==(_MyIter& other)
		{
			return (_p == other._p);
		}
		bool operator!=(_MyIter& other)
		{
			return !(*this == other);
		}
		bool operator<(_MyIter&other)
		{
			return (_p<other._p);
		}

	};


	class string_iterator:public string_const_iterator
	{
	public:
		typedef char value_type;
		typedef char& reference;
		typedef char* pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef string_const_iterator _MyBase;
		typedef string_iterator _MyIter;

		//construct
		string_iterator() :string_const_iterator() {}
		string_iterator(pointer p) :string_const_iterator(p) {}
		string_iterator(string_const_iterator&it) :string_const_iterator(it) {}

		//
		reference operator*()
		{
			return (reference)**(_MyBase*)this;
         }
		reference operator[](size_type n)
		{
			return (reference)(*(_MyBase*)this).operator[](n);
			//reference (*(_MyBase*)this)[n];
		}
		
		//前置递增
		_MyIter&operator++()
		{
			++*(_MyBase*)this;
			return *this;
		}
		//后置递增
		_MyIter operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		//前置递减
		_MyIter operator--()
		{
			--*(_MyBase*)this;
			return *this;
		}
		//后置递减
		_MyIter operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		//复合加
		_MyIter&operator+=(size_type n)
		{
			*(_MyBase*)this += n;
			return *this;
		}
		//加
		_MyIter operator+(size_type n)
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}
		//复合减
		_MyIter&operator-=(size_type n)
		{
			*(_MyBase*)this -= n;
			return *this;
		}
		//减
		
		_MyIter operator-(size_type n)
		{
			auto tmp = *this;
			tmp -= n;
			return tmp;
		}
		
		//迭代器的距离
		difference_type operator-(_MyIter&other)
		{
			return (*(_MyBase*)this - (*(_MyBase*)&other));
		}
		//逻辑操作

	};

/**
*   string 的定义
*
*/
	class string
	{

	//友元函数声明
	public:
	//	friend string operator+(const string&,const string&); //这里由于忘记同步参数类型，导致一个
	public:
		typedef char         value_type;
		typedef char*        pointer;
		typedef const char*  const_pointer;
		typedef  char&       reference;
		typedef  const char& const_reference;
		typedef std::size_t   size_type;
		typedef std::ptrdiff_t difference_type;

		typedef MyCppSTL::allocator<char>  alloc;     //空间分配器
		typedef string_const_iterator    const_iterator;
		typedef string_iterator          iterator;

		
	private: //member
		pointer _start;     //指向字符串的开始
		pointer _finish;    //指向字符串末尾
		pointer _end_of_storage; //指向存储空间末尾.
		enum { _default_size = 8 };
	public:
		static const size_type npos;
		//construct
		explicit string():_start(0),_finish(0),_end_of_storage(0) //default construct,empty string
		{
			allocate_block(_default_size);
		}
		string(size_type count, char value) //count value
		{
			allocate_block(count+1);   //分配空间
			_finish = _start + count;  //由于uninitialized_fill是传值而不是引用，所以采用这种方法
			uninitialized_fill(_start, _finish, value);  //
			*_finish = char();
		}
		string(const string&other, size_type pos, size_type count = string::npos)
		{
			if (pos > size())
			{
				std::out_of_range("out of range");
			}
			range_initializer(other._start + pos, other._start + pos+min(count,other.size()-pos));
		}

		string(const char*s, size_type count)
		{
			range_initializer(s, s + count);
		}

		string(const char*s)
		{
			int count=0;
			while(s[count++]!=NULL);  //
			range_initializer(s,s+count-1);
		}

		template<class InputIter>
		string(InputIter first,InputIter last)
		{
			string_aux(first,last,std::is_integral<InputIter>::type());  //指派辅助函数处理
		}
		//copy construct
		string(const string&other)
		{
			if(this!=&other)
			{
				string_aux(other.begin(),other.end(),std::false_type());
			}
		}

		//move construct
		string(string&&other):_start(other._start),
									_finish(other._finish),
									_end_of_storage(other._end_of_storage)
		{
			other._start=other._finish=other._end_of_storage=0;
		}

		//initializer_list
		string(std::initializer_list<char>clist)
		{
			string_aux(clist.begin(),clist.end(),std::false_type());
		}

		//copy operator

		// string&operater=(const string&other)
		// {
		// 	if(this!=&other)
		// 	{
				
		// 	}
		// 	return *this;
		// }
		// string&operator=(string&&other)
		// {
		// 	if(this!=&other)
		// 	{ 
		// 		//要销毁原先的
		// 		_start=other._start;
		// 		_finish=other._finish;
		// 		_end_of_storage=other._end_of_storage;
		// 		other._start=NULL;
		// 		other._finish=NULL;
		// 		other._end_of_storage=NULL;;
		// 	}
		// 	return *this;
		// }

		// string&operator=(const char*s)
		// {
		// 	string tmp(s);
		// 	*this=s; 
		// 	return *this;
		// }
		// string&operator=(std::initializer_list<char> clist)
		// {
		// 	string tmp(clist);
		// 	*this=tmp;
		// 	return *this;
		// }

         //at
		reference at(size_type pos)
		{
            check_size(pos);
            return *(_start+pos);
		}
		const_reference at(size_type pos)const
		{
			check_size(pos);
			return *(_start+pos);
		}
		//operator[]
		reference operator[](size_type pos)
		{
			return *(_start+pos);
		}
		const_reference operator[](size_type pos) const
		{
			return *(_start + pos);
		}
		//
		char front()
		{
			return *_start;
		}
		const char front() const
		{
			return *_start;
		}
		char back()
		{
			return *(_finish-1);
		}
		const char back() const
		{
			return *(_finish-1);
		}

		const char*data() const
		{
			return _start;
		}
		const char*c_str() const
		{
			return _start;
		}

        //capacity
        bool empty()
        {
        	return (_start==_finish);
        }

        size_type max_size() const
        {
			return ((size_type(-1) / sizeof(char)) - 1);
        }

        size_type capacity() const
        {
        	return (_end_of_storage-_start);
        }

        //如果比当前容量小，就不分配
        void reserve(size_type new_cap=0) 
        {
        	if(new_cap>capacity())
        	{
        		auto _start_tmp=_start;
        		auto _finish_tmp=_finish;
        		auto _end_of_storage_tmp=_end_of_storage;
        		allocate_block(new_cap);
        		MyCppSTL::uninitialized_copy(_start_tmp,_finish_tmp,_start);
				_finish += (_finish_tmp - _start_tmp);
        		//销毁
        		deallocate_block(_start_tmp,_end_of_storage_tmp-_start_tmp);
        	}
        	
        }
        void shrink_to_fit()
        {
        	auto _end_of_storage_tmp=_end_of_storage;
        	_end_of_storage=_finish+1;
			deallocate_block(_end_of_storage,_end_of_storage_tmp-_end_of_storage);
        }


        //operation
        string&erase(size_type index=0,size_type count=npos)
        {
              auto end=min(count,size()-index);
              for(size_type i=0;i<end;++i)
              {
              	 MyCppSTL::copy(_start+index+1,_finish,_start+index);
              }
              _finish-=count;
              return *this;
        }

        iterator erase(iterator pos)
        {
        	auto index=pos-begin();
        	erase(index,1);
        	return pos;
        }

        const_iterator erase(const_iterator pos)
        {
        	auto index=pos-cbegin();
        	erase(index,1);
        	return pos;
        }

        iterator erase(iterator first,iterator last)
        {
        	auto count=last-first;
        	while(count--)
        	{
        		first=erase(first);
        	}
        	return first;
        }
        const_iterator erase(const_iterator first,const_iterator last)
        {
        	auto count=last-first;
        	while(count--)
        	{
        		first=erase(first);
        	}
        	return first;
        }

        void clear()
        {
        	erase(begin(),end());
        }

		
        iterator insert(iterator pos, size_type count, char ch )
        {
        	if(count!=0)
        	{
				pointer _pos = _start + (pos - begin());
        		if(size_type(_end_of_storage-_finish)>=(count+1))//有空间
        		{
        			//事实上，数据是char,是POD，所以可以省略构造函数
        			//MyCppSTL::copy_backward(_pos,_finish,_pos+count);
					copy_back(_finish - 1, _pos, _finish + count-1);
					_finish += count;
					assign(_pos, _pos + count, ch);
					*_finish=char();
					return iterator(_pos);
        		}
        		else  //空间不够
        		{
        			auto _old_size = capacity();
        			auto _new_size=_old_size+2*count+1; //重新分配空间
        			auto _old_start=_start;
        			auto _old_finish=_finish;
        			allocate_block(_new_size);
        			MyCppSTL::uninitialized_copy(_old_start,_old_finish,_start);
        			_finish+=(_old_finish-_old_start);
					_pos = _start+(_pos - _old_start);
			  		copy_back(_finish - 1, _pos, _finish + count-1);
					_finish += count;
					assign(_pos, _pos + count, ch);
					*_finish=char();
					return iterator(_pos);
        		}
        	}
        	return pos;
        }

        iterator insert( iterator pos, char ch)
        {
        	return insert(pos,1,ch);
        }

        template<class InputIt>
        void insert(iterator pos,InputIt first,InputIt last )
        {
        	insert_aux(pos,first,last,std::is_integral<InputIt>::type());
        }

        iterator insert( iterator pos, std::initializer_list<char> ilist )
        {
        	return insert_aux(pos,ilist.begin(),ilist.end(),std::false_type());
        }

        void push_back(char ch)
        {
        	insert(end(),ch);
        }

        void pop_back()
        {
        	erase(end()-1);
        }

        string& append(size_type count,char ch)
        {
        	insert(end(),count,ch);
        	return *this;
        }

        string& append(const string& str )
        {
        	insert(end(),str.begin(),str.end());
			return *this;
        }
        string& append( const char* s,size_type count)
        {
        	insert(end(),s,s+count);
        	return *this;
        }

        string& append(const char* s)
        {
        	size_type count=0;
        	int i=0;
        	while(s[i++]!='\0')++count;
        	return append(s,count);
        }

        template< class InputIt >
        string& append(InputIt first, InputIt last)
        {
        	return append_aux(first,last,std::is_integral<InputIt>::type());
        }

        string& append( std::initializer_list<char> ilist )
        {
        	return append(ilist.begin(),ilist.end());
        }

        string& operator+=(const string& str)
        {
        	return append(str.begin(),str.end());
        }

        string& operator+=(char ch)
        {
        	return append(1,ch);
        }

        string& operator+=(const char* s )
        {
        	return append(s);
        }

        string& operator+=(std::initializer_list<char>ilist)
        {
        	return append(ilist);
        }

        string& replace( size_type pos, size_type count, char ch )
        {
        	if(pos>size())throw std::out_of_range("out of range");
        	if((size_type)(size()-pos)<count)erase(begin()+pos,end());
        	else
        	{
        		erase(begin()+pos,begin()+(pos+count));
        	}
        	insert(begin()+pos,count,ch);
        	return *this;
        }

        string& replace( size_type pos, size_type count,const string& str)
        {
        	int i=0;
        	while(count--)
        	{
        		replace(pos,1,str[i]);
        		++pos;++i;
        	}
        	return *this;
        }

        string& replace(size_type pos,size_type count,const string& str,size_type pos2,size_type count2)
        {
        	if(pos>size())throw std::out_of_range("out of range");
        	if(pos2>str.size())throw std::out_of_range("out of range");
        	if(size_type(size()-pos)<count)erase(begin()+pos,end());
        	else erase(begin()+pos,begin()+(pos+count));
        	insert(begin()+pos,str.begin()+pos2,str.begin()+pos2+count2);
        	return *this;
        }

        template< class InputIt >
        string& replace(iterator first, iterator last,InputIt first2, InputIt last2)
        {
        	return replace_aux(first,last,first2,last2,std::is_integral<InputIt>::type());
        }

        string substr( size_type pos = 0,size_type count = npos ) const
        {
        	if(count>size())count=size();
        	return string(_start+pos,_start+pos+count);
        }
        size_type copy( char* dest,size_type count,size_type pos = 0) const
        {
        	if(pos>size())throw std::out_of_range("out of range");
        	size_type len=min(size()-pos,count);
        	for(size_type i=0;i<len;++i)
        	{
        		dest[i]=*(_start+pos+i);
        	}
        	return len;
        }

        void resize(size_type count)
        {
        	resize(count,char());
        }

        void resize( size_type count, char ch)
        {
        	if(count>size())
        	{
        		int addition=count-size();
        		append(addition,ch);
        	}
        	else
        	{
        		int erase_num=size()-count;
        		erase(begin()+count,end());
        	}
        }

        void swap(string& other)
        {
        	auto tmp_start=_start;
        	auto tmp_finish=_finish;
        	auto tmp_end_of_storage=_end_of_storage;
        	_start=other._start;
        	_finish=other._finish;
        	_end_of_storage=other._end_of_storage;
        	other._start=tmp_start;
        	other._finish=tmp_finish;
        	other._end_of_storage=tmp_end_of_storage;
        }

        //search
        size_type find(const char* s,size_type pos,size_type count) const
        {
        	if((pos+count)>size())return npos;
        	else
        	{
        		pointer res_it=MyCppSTL::search(_start,_finish,s,s+count);
        		return (res_it!=_finish?res_it-_start:npos);
        	}
        }

        size_type find( const string& str, size_type pos = 0 ) const
        {
        	return find(&str[0],pos,str.size());
        }

        size_type find( const char* s, size_type pos = 0 ) const   //s terminal '\n'
        {
        	size_type count=0;
        	while(s[count++]!=0);
            return find(s,pos,count);
        }

        size_type rfind( const char* s, size_type pos,size_type count) const
        {
        	size_type len=size();
        	if(count>len)return npos;
        	else if(count==0)return min(pos,len);
        	else
        	{
        		pointer res_it=rsearch(_start,_finish,s,s+count);
        		return (res_it==(_start-1))?npos:res_it-_start;
				
        	}
        }

        size_type rfind(const char*s,size_type pos=npos) const
        {
        	size_type count =0;
        	while(s[count]!='\0')++count;
        	return rfind(s,pos,count);  //从string的最后一个字符开始查找
        }

        size_type rfind(char ch,size_type pos=npos) const
        {
        	char temp=ch;
        	return rfind(&temp,pos,1);
        }

        size_type rfind( const string& str, size_type pos = npos ) const
        {
        	return rfind(&str[0],pos,str.size());
        }

        size_type find_first_of( const char* s, size_type pos, size_type count) const
        {
        	if(pos>size())return npos;
        	else if(count==0)return min(pos,size());  //
        	for(size_type i=pos;i<size();++i)
        	{
        		for(size_type j=0;j<count;++j)
        		{
        			if(_start[i]==s[j])return i;
        		}
        	}

        	return npos;  //没找到
        }

        size_type find_first_of( const char* s, size_type pos = 0 ) const
        {
        	size_type count=0;
        	while(s[count])++count;
        	return find_first_of(s,pos,count);
        }

        size_type find_first_of(const string& str, size_type pos = 0 ) const
        {
        	return find_first_of(&str[0],pos,str.size());
        }

        size_type find_first_of( char ch, size_type pos = 0 ) const
        {
        	char temp=ch;
        	return find_first_of(&temp,pos,1);
        }

        size_type find_first_not_of( const char* s, size_type pos, size_type count ) const
        {
        	if(pos>npos)return npos;
        	else if(count==0)return min(npos,size());
        	for(size_type i=pos;i<size();++i)
        	{
				size_type j = 0;
        		for(;j<count;++j)
        		{
        			if(_start[i]==s[j])break;
        		}
        		if(j==count)return i;
        	}
        	return npos;   //没有这样的情况存在
        }
		size_type find_first_not_of(const string& str, size_type pos = 0) const
		{
			return find_first_not_of(&str[0], pos, str.size());
		}
        size_type find_first_not_of( const char* s, size_type pos = 0) const
        {
        	size_type count=0;
        	while(s[count])++count;
        	return find_first_not_of(s,pos,count);
        }

        size_type find_first_not_of( char ch, size_type pos = 0 ) const
        {
        	char temp=ch;
        	return find_first_not_of(&temp,pos,1);
        }

        size_type find_last_of( const char* s, size_type pos, size_type count ) const
        {
        	if(pos>=size())pos=size()-1;  //限制范围
        	else if(count==0)return max(0,pos);
        	for(int i=(int)(pos);i>=0;--i)
        	{
        		for(int j=count-1;j>=0;--j)
        		{
        			if(_start[i]==s[j])return i;
        		}
        	}
        	return npos;  //没找到
        }

        size_type find_last_of( const char* s, size_type pos = npos ) const
        {
        	size_type count=0;
        	while(s[count])++count;
        	return find_last_of(s,pos,count);
        }

        size_type find_last_of( const string& str, size_type pos = npos ) const
        {
        	return find_last_of(&str[0],pos,str.size());
        }

        size_type find_last_of(char ch, size_type pos = npos ) const
        {
        	char temp=ch;
        	return find_last_of(&ch,pos,1);
        }

        size_type find_last_not_of(const char* s, size_type pos, size_type count) const
        {
        	if(pos>=size())pos=size()-1;
        	else if(count==0)return max(pos,0);
        	for(int i=(int)pos;i>=0;--i)
        	{
        		int j=(int)(count-1);
        		for(;j>=0;--j)
        		{
        			if(_start[i]==s[j])break;
        		}
        		if(j<0)return i;   //找到
        	}

        	return npos;  //未找到
        }

        size_type find_last_not_of( const char* s, size_type pos = npos ) const
        {
        	size_type count=0;
        	while(s[count])++count;
        	return find_last_not_of(s,pos,count);
        }

        size_type find_last_not_of( const string& str, size_type pos = npos ) const
        {
        	return find_last_not_of(&str[0],pos,str.size());
        }

        size_type find_last_not_of( char ch, size_type pos = npos ) const
        {
        	char temp=ch;
        	return find_last_not_of(&temp,pos,1);
        }

		//迭代器
		iterator begin() { return iterator(_start); }
		const_iterator begin()const { return const_iterator(_start); }
		const_iterator cbegin()const { return const_iterator(_start); }
		iterator end() { return iterator(_finish); }
		const_iterator end() const { return const_iterator(_finish); }
		const_iterator cend() const { return const_iterator(_finish); }

		size_type size() const { return _finish - _start; }
		//辅助函数
	private:
		void check_size(size_type pos) const
		{
			if(pos>=size())throw std::out_of_range("out of range");
		}
		void allocate_block(size_type n) //分配空间
		{
			_start = alloc::allocate(n);
			_finish = _start;
			_end_of_storage = _start + n;
		}
		void deallocate_block(pointer begin,size_type count)        //释放空间
		{
			alloc::deallocate(begin, count);
			
		}
		template<class Iterator>
		void range_initializer(Iterator first, Iterator last)
		{
			auto n = last - first;
			allocate_block(n+1); //分配空间
			MyCppSTL::uninitialized_copy(first, last, _start);
			_finish+=n;
			*_finish = char();   //添加结束符
		}

		template<class Integral>   //是整数
		void string_aux(Integral count,Integral value,std::true_type)
		{
            allocate_block(count+1);   //分配空间
			_finish = _start + count;  //由于uninitialized_fill是传值而不是引用，所以采用这种方法
			uninitialized_fill(_start, _finish, value);  //
			*_finish = char();
		}
		template<class InputIter>
		void string_aux(InputIter first,InputIter last,std::false_type)
		{
			range_initializer(first,last);
		}
		//向后拷贝
		void copy_back(pointer last,pointer first,pointer dest)
		{
			while (last >= first) //
			{
				*dest-- = *last--;
			}
		}
		//赋值
		void assign(pointer first, pointer last, char ch)
		{
			while (first != last)
			{
				*first++ = ch;
			}
		}

		template<class InputIt>   //是迭代器
        iterator insert_aux(iterator pos,InputIt first,InputIt last,std::false_type)
        {
        	pointer _pos = _start + (pos - begin());
        	size_type count=(size_type)(last-first);
        	if((count+1)<=(size_type)(_end_of_storage-_finish))
        	{
        		copy_back(_finish - 1, _pos, _finish + count-1);
				_finish += count;
				while(first!=last)
				{
					assign(_pos, _pos+1, *first++);
					++_pos;
				}
				*_finish=char();
				return iterator(_pos);
        	}
        	else
        	{
        		auto _old_size = capacity();
        		auto _new_size=_old_size+2*count+1; //重新分配空间
        		auto _old_start=_start;
        		auto _old_finish=_finish;
        		allocate_block(_new_size);
        		MyCppSTL::uninitialized_copy(_old_start,_old_finish,_start);
        		_finish+=(_old_finish-_old_start);
				_pos = _start+(_pos - _old_start);
			  	copy_back(_finish - 1, _pos, _finish + count-1);
				_finish += count;
				while(first!=last)
				{
					assign(_pos, _pos+1, *first++);
					++_pos;
				}
				*_finish=char();
				return iterator(_pos);
        	}
        }

        template<class Integral>   //不是迭代器
        iterator insert_aux(iterator pos,Integral count,Integral ch,std::true_type)
        {
        	return insert(pos,first,ch);
        }

         template< class InputIt >  //是迭代器
         string& append_aux(InputIt first, InputIt last,std::false_type)
         {
         	while(first!=last)
         	{
         		append(1,*first);
         		++first;
         	}

         	return *this;
         }

         template< class Integral >  //不是迭代器
         string& append_aux(Integral count , Integral ch,std::true_type)
         {
         	insert(end(),count,ch);
        	return *this;
         }

        template< class InputIt >  //是迭代器
        string& replace_aux(iterator first, iterator last,InputIt first2, InputIt last2,std::false_type)
        {
        	erase(first,last);
        	insert(first,first2,last2);
			return *this;
        }

        template< class Integral >  //不是迭代器
        string& replace_aux(iterator first, iterator last,Integral count, Integral ch,std::true_type)
        {
        	erase(first,last);
        	insert(first,count,ch);
			return *this;
        }

        template<class RandomAccessIterator1,class RandomAccessIterator2>  //逆序搜索 [first1,last1),																			
        RandomAccessIterator1 rsearch(RandomAccessIterator1 first1,RandomAccessIterator1 last1, 	//[s_first,s_last)
        	              RandomAccessIterator2 s_first,RandomAccessIterator2 s_last) const
        {
        	--last1;
        	--s_last;
            --first1;
			--s_first;
        	for(;;--last1)
        	{
        		RandomAccessIterator1 it = last1;
        		for(RandomAccessIterator2 s_it=s_last;;--s_it,--it)
        		{
        			if(s_it==s_first)return (last1-(s_last-s_first)+1);   //empty or find
        			if(it==first1)return first1;          //no such substr
        			if(!(*it==*s_it))break;
        		}
        	}

        }

	};  // end-string
    
    //
	const string::size_type string::npos =(string::size_type)-1;
    //Non-member functions
    string operator+(const string&lhs,const string&rhs)
    {
    	return string(lhs).append(rhs);
    }
	
    string operator+(const char*s,const string&rhs)
    {
    	const string res(s);
    	return (res+rhs);
    }
    string operator+(char ch,const string&rhs)
    {
    	const string res(1,ch);
    	return (res+rhs);
    }
    string operator+(const string&lhs,const char*s)
    {
    	const string res(s);
    	return (lhs+res);
    }
    string operator+(const string&lhs,char ch)
    {
    	string res(1,ch);
    	return (lhs+res);
    }
	
	//逻辑运算符
	bool operator==(const string&lhs,const string&rhs)
	{
		if(lhs.size()==rhs.size())
		{
			auto it2=rhs.begin();
			for(auto it=lhs.begin();it!=lhs.end();++it,++it2)
			{
				if(*it!=*it2)return false;
			}
			return true;
		}
		return false;
	}
	bool operator!=(const string&lhs,const string&rhs)
	{
		return !(lhs==rhs);
	}
	bool operator<(const string&lhs,const string&rhs)
	{
		return lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
	}
	bool operator>(const string&lhs,const string&rhs)
	{
		return (rhs<lhs);
	}
	bool operator<=(const string&lhs,const string&rhs)
	{
		return !(lhs>rhs);
	}
	bool operator>=(const string&lhs,const string&rhs)
	{
		return !(lhs<rhs);
	}

	void swap(string&lhs,string&rhs)
	{
		lhs.swap(rhs);
	}

	std::ostream& operator<<(std::ostream&os,MyCppSTL::string&s)
	{
		for(auto e:s)os<<e;
		return os;
	}

    /*****/
	std::istream&operator>>(std::istream&is,MyCppSTL::string&s)
	{
		char ch;         /*需要去掉前导空格，以及遇到空格要结束*/
		while(is>>ch)
		{
			s.push_back(ch);
		}
		return is;
	}

	

}  //end-namespace


#endif  //end-string.h  
