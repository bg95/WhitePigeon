#ifndef _WPALLOCATOR_H
#define _WPALLOCATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

template <class T> class WPAllocator
{
	public:
		WPAllocator();
		~WPAllocator();
		T *Allocate();
		void Recycle();
	private:
		long I, J;
		std::vector <T *> Memo;
};

#endif

