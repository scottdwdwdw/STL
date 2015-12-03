#ifndef TIMECOUNT_H
#define TIMECOUNT_H

#include<Windows.h>
#include<iostream>
namespace MyCppSTL
{
	class timeCount
	{

	private:
		static LARGE_INTEGER nfreq;
		static LARGE_INTEGER begin;
		static LARGE_INTEGER end;
	public:
		timeCount()
		{
			
		}
	   static void timeBegin()
		{
			QueryPerformanceFrequency(&nfreq);
			QueryPerformanceCounter(&begin);
		}
	  static void timeEnd()
		{
			QueryPerformanceCounter(&end);
			double time = ((end.QuadPart - (double)begin.QuadPart)*1000) / (double)nfreq.QuadPart;
			std::cout << "time is: " << time << "  ms"<<std::endl;
		}

		~timeCount() {}
	};


	 LARGE_INTEGER timeCount::nfreq=LARGE_INTEGER();
	 LARGE_INTEGER timeCount::begin = LARGE_INTEGER();
	 LARGE_INTEGER timeCount::end = LARGE_INTEGER();

}



#endif