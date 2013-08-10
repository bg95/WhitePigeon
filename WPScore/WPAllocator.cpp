#include "WPAllocator.h"

template <class T> WPAllocator <T>::WPAllocator()
{
	I = - 1;
	J = 0;
	Memo.clear();
}

template <class T> WPAllocator <T>::~WPAllocator()
{
	Recycle();
}

template <class T> T *WPAllocator <T>::Allocate()
{
	// Please LOCK!
	++ J;
	if (I == - 1 || J == (1 << I))
	{
		J = 0;
		++ I;
		Memo.push_back(new T[1 << I]);
	}
	return Memo[I] + J;
}

template <class T> void WPAllocator <T>::Recycle()
{
	// Please LOCK!
	I = - 1;
	J = 0;
	while (!Memo.empty())
	{
		delete[] Memo.back();
		Memo.pop_back();
	}
}
