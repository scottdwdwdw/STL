#ifndef _HASH_FUNCTION_H
#define _HASH_FUNCTION_H


namespace MyCppSTL
{

	template<typename Key> struct hash { };

	inline size_t string_hash(const char*s)
	{
		unsigned long h = 0;
		for (; *s; ++s)
		{
			h = 5 * h + *s;
		}
		return size_t(h);
	}

	//Ä£°åÌØÀý»¯
	template<>
	struct hash<char*> 
	{
		size_t operator()(const char* s) const { return string_hash(s); }
	};

	template<>
	struct hash<const char*>
	{
		size_t operator()(const char*s) const { return string_hash(s); }
	};

	template<>
	struct hash<char>
	{
		size_t operator()(char c) const { return size_t(c); }
	};

	template<>
	struct hash<signed char>
	{
		size_t operator()(unsigned char c) const { return size_t(c); }
	};

	template<>
	struct hash<unsigned char>
	{
		size_t operator()(unsigned char c) const { return size_t(c); }
	};

	template<>
	struct hash<short>
	{
		size_t operator()(short n) const { return size_t(n); }
	};

	template<>
	struct hash<unsigned short>
	{
		size_t operator()(unsigned short n) const { return size_t(n); }
	};

	template<>
	struct hash<int>
	{
		size_t operator()(int n) const { return size_t(n); }
	};

	template<>
	struct hash<unsigned int>
	{
		size_t operator()(unsigned int n) const { return size_t(n); }
	};

	template<>
	struct hash<long>
	{
		size_t operator()(long n) const { return size_t(n); }
	};

	template<>
	struct hash<unsigned long>
	{
		size_t operator()(unsigned long n) const { return size_t(n); }
	};



}// end of namespace



#endif
