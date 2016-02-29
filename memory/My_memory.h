#ifndef _MEMORY_H
#define _MEMORY_H

#include<assert.h>
#include<utility>

namespace MyCppSTL
{

	class shared_count_base  //基类
	{
		//成员数据
	private:
		unsigned int _count;
	public://构造函数
		shared_count_base() :_count(1) {}
		virtual ~shared_count_base() {}
		virtual void dispose() = 0;  //释放资源，当计数值为0时
		virtual void destory()       //对象释放
		{
			delete this;
		}
		void add_ref_count() { ++_count; }
		void decr_ref_count() { --_count; }  //暂时这样
		long use_count() { return _count; }
		bool expired() { return _count == 0; }

	};

	template<typename T>
	class shared_count_ptr :public shared_count_base
	{
		//成员数据
	private:
		T* _ptr;
	public:
		//构造函数
		shared_count_ptr() :_ptr(0) {}
		shared_count_ptr(T* p) :_ptr(p) {}
		//成员
		virtual void dispose(){ delete _ptr;}
		virtual void destory() { delete this; }
		virtual ~shared_count_ptr() {}
	};

	template<typename T,typename Deleter>
	class shared_count_deleter :public shared_count_base
	{
	private:
		T*_ptr;
		Deleter _D;
	public:
		shared_count_deleter(T*p, Deleter d) :_ptr(p), _D(d) {}
		//成员函数
		virtual void dispose() { _D(_ptr); } //使用用户提供的删除器
		virtual void destory() { delete this; }
		virtual ~shared_count_deleter() {}

	};


	template<typename T>
	class shared_count      //引用计数
	{
	 public: //内嵌型别
		 using value_type = T;
	private: //成员数据
		shared_count_base *_count_ptr;
	public: //构造函数
		shared_count() :_count_ptr(0) {}
		shared_count(T*p) :_count_ptr(0)
		{
			_count_ptr = new shared_count_ptr<T>(p);
		}
		template<typename Tp=T,typename Deleter>
		shared_count(Tp*p, Deleter d) : _count_ptr(0)
		{
			_count_ptr = new shared_count_deleter<Tp, Deleter>(p, d);
		}

		~shared_count()
		{
			if (_count_ptr != NULL)
			{
				if (_count_ptr->use_count() != 1)
				{
					_count_ptr->decr_ref_count();
				}
				else
					_count_ptr->dispose();
			}
		}

		shared_count(shared_count&rhs) :_count_ptr(rhs._count_ptr)  //copy construct
		{
			if (_count_ptr != NULL)_count_ptr->add_ref_count();   //增加引用计数
		}

		shared_count&operator=(shared_count&rhs)  //assignment
		{
			if (this != &rhs)
			{
				if (_count_ptr != NULL)_count_ptr->decr_ref_count();
				if (_count_ptr->use_count == 0)_count_ptr->dispose();   //释放指针资源
				if (rhs._count_ptr != NULL)rhs._count_ptr->add_ref_count();
				_count_ptr = rhs._count_ptr;
			}

			return *this;
		}

		void swap(shared_count&rhs)
		{
			shared_count_base* temp = rhs._count_ptr;
			rhs._count_ptr = _count_ptr;
			_count_ptr = temp;
		}

		long use_count() const
		{
			return _count_ptr == NULL ? 0 : _count_ptr->use_count();
		}
		bool unique() const
		{
			return this->use_count == 0;
		}

	};

	template<typename T>
	class shared_ptr
	{
		//内嵌型别
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using const_pointer = const T*;
		using const_reference = const T&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
	private: //成员
		T* _Ptr;
		shared_count<T> count;
	public:  //构造函数
		shared_ptr() :_Ptr(0), count() {}
		shared_ptr(T* p) : _Ptr(p), count(p) {}
		template<typename _Tp=T,typename Deleter>   //接受一个删除函数
		shared_ptr(_Tp* p, Deleter d) : _Ptr(p), count(p, d) {}
		shared_ptr(shared_ptr&& rhs) :_Ptr(rhs._Ptr), count()
		{
			count.swap(rhs.count);
			rhs._Ptr = NULL;
		}
		shared_ptr(shared_ptr&rhs) = default;
		~shared_ptr() = default;
		shared_ptr&operator=(shared_ptr&&rhs)
		{
			shared_ptr(std::move(rhs)).swap(*this); //采用移动赋值后，右边的值会处于不确定状态
			return *this;
		}
		shared_ptr&operator=(shared_ptr&rhs)
		{
			_Ptr = rhs._Ptr;
			count = rhs.count;
			return *this;
		}

		void reset()
		{
			shared_ptr().swap(*this);
		}

		reference operator*() const 
		{
			assert(_Ptr != NULL);
			return *_Ptr;
		}

		pointer operator->() const
		{
			assert(_Ptr != NULL);
			return &(operator*());
		}

		pointer get() const 
		{
			return _Ptr;
		}

		explicit operator bool() const
		{
			return _Ptr == NULL ? false : true;
		}

		bool unique() const
		{
			return count.unique();
		}

		long use_count() const
		{
			return count.use_count();
		}


		template<typename Tp1>
		void reset(Tp1*p)  //Tp1要与T类型兼容
		{
			shared_ptr(p).swap(*this);
		}

		void swap(shared_ptr<T>&rhs)
		{
			std::swap(_Ptr, rhs._Ptr);
			count.swap(rhs.count);
		}

	};

	//逻辑操作
	template<typename T>
	bool operator==(shared_ptr<T>&lhs, shared_ptr<T>&rhs)
	{
		return lhs.get() == rhs.get();
	}

	template<typename T>
	bool operator!=(shared_ptr<T>&lhs,shared_ptr<T>&rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T>
	bool operator<(shared_ptr<T>&lhs,shared_ptr<T>&rhs)
	{
		return lhs.get()<rhs.get();
	}
	template<typename T>
	bool operator>(shared_ptr<T>&lhs,shared_ptr<T>&rhs)
	{
		return rhs<lhs;
	}
	template<typename T>
	bool operator>=(shared_ptr<T>&lhs,shared_ptr<T>&rhs)
	{
		return !(lhs < rhs);
	}

	template<typename T>
	bool operator<=(shared_ptr<T>&lhs,shared_ptr<T>&rhs)
	{
		return !(lhs > rhs);
	}

	template<typename T>
	void swap(shared_ptr<T>&lhs, shared_ptr<T>&rhs)
	{
		lhs.swap(rhs);
	}

	template<typename T,typename ...T2>
	shared_ptr<T> make_shared(T2...args)
	{
		return shared_ptr<T>(::new T(std::forward<T2>(args)...));
	}



	//unqiue_ptr
	template<typename Ty>
	struct default_deleter
	{
		void operator()(Ty* ptr)
		{
			//assert(ptr != NULL);
			if(ptr!=NULL)delete ptr;
		}
	};

	//特例化
	template<typename Ty>
	struct default_deleter<Ty[]>
	{
		void operator()(Ty*ptr)
		{
			//assert(ptr != NULL);
			if(ptr!=NULL)delete[]ptr;
		}
	};

	//unique_ptr
	template<typename Ty,typename Deleter=default_deleter<Ty>>
	class unique_ptr
	{
	public://内嵌型别
		using value_type = Ty;
		using reference = Ty&;
		using const_reference = const Ty&;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using size_type = std::size_t;
	private://内置数据
		Ty* _Ptr;
		Deleter Del;
	public:
		//构造函数
		unique_ptr() :_Ptr(NULL), Del(Deleter()) {}
		unique_ptr(pointer p) :_Ptr(p), Del(Deleter()) {}
		unique_ptr(pointer p, Deleter&&d) :_Ptr(p), Del(std::move(d)) {}
		unique_ptr(unique_ptr&&rhs):_Ptr(std::move(rhs._Ptr)),Del(std::move(rhs.Del))
		{
			rhs._Ptr = NULL;
		}
		unique_ptr&operator=(unique_ptr&&rhs)
		{
			_Ptr = std::move(rhs._Ptr);
			Del = std::move(rhs.Del);
			rhs._Ptr = NULL;
		}
		
		~unique_ptr()
		{
			Del(_Ptr);
		}

		unique_ptr(unique_ptr&rhs) = delete;
		unique_ptr&operator=(unique_ptr&rhs) = delete;
		reference operator*() const
		{
			return *_Ptr;
		}
		reference operator*()
		{
			assert(_Ptr != NULL);
			return *_Ptr;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		pointer get() const
		{
			return _Ptr;
		}

		pointer release()
		{
			pointer temp = _Ptr;
			_Ptr = NULL;
			return temp;
		}

		void reset(pointer p = NULL)
		{
			pointer temp = _Ptr;
			_Ptr = p;
			if (temp)Del(temp);
		}

		void swap(unique_ptr&rhs)
		{
			std::swap(_Ptr, rhs._Ptr);
			std::swap(Del, rhs.Del);
		}
	};


	//逻辑操作
	template<typename Ty>
	bool operator==(unique_ptr<Ty>&rhs, unique_ptr<Ty>&lhs)
	{
		return (rhs.get() == lhs.get());
	}

	template<typename Ty>
	bool operator!=(unique_ptr<Ty>&rhs, unique_ptr<Ty>&lhs)
	{
		return !(rhs == lhs);
	}

	template<typename Ty>
	bool operator<(unique_ptr<Ty>&rhs, unique_ptr<Ty>&lhs)
	{
		return (rhs.get() < lhs.get());
	}

	template<typename Ty>
	bool operator>(unique_ptr<Ty>&rhs, unique_ptr<Ty>&lhs)
	{
		return (lhs < rhs);
	}

	template<typename Ty>
	bool operator<=(unique_ptr<Ty>&rhs, unique_ptr<Ty>&lhs)
	{
		return !(rhs > rhs);
	}

	template<typename Ty>
	bool operator>=(unique_ptr<Ty>&rhs, unique_ptr<Ty>&lhs)
	{
		return !(rhs<lhs);
	}

	template<typename Ty>
	void swap(unique_ptr<Ty>&rhs, unique_ptr<Ty>&lhs)
	{
		rhs.swap(lhs);
	}

}

#endif // !_MEMORY_H

