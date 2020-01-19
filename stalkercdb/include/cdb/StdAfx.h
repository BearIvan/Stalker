// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma once

#include "tools/xrCore.h"



#	define CNEW(type)			new ( xr_alloc<type>(1) ) type 
#	define CDELETE(ptr)			xr_delete(ptr)
#	define CFREE(ptr)			xr_free(ptr)
#	define CMALLOC(size)		xr_malloc(size)
#	define CALLOC(type,count)	xr_alloc<type>(count)



template <class T>
IC	void cdelete		(T* &ptr)
{
	if (ptr)
	{
		if constexpr (xr_is_polymorphic<T>::value)
		{
			void* _real_ptr = dynamic_cast<void*>(ptr);
			ptr->~T();
			BearMemory::Free(_real_ptr);
		}
		else
		{
			ptr->~T();
			BearMemory::Free(ptr);
		}
		((T*&)ptr) = NULL;
	}
}

#include "opcode.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

