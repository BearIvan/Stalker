#pragma once
template <class T>
class destructor
{
	T* ptr;
public:
	destructor(T* p)	{ ptr=p;			}
	~destructor()		{ xr_delete(ptr);	}
	IC T& operator() ()
	{	return *ptr; }
};