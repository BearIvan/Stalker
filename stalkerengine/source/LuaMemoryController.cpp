
#include "stdafx.h"
#include "luabind\luabind.hpp"
static LPVOID __cdecl luabind_allocator(
	void* context, void const* pointer, size_t size
)
{
	if (!size) {
		LPVOID	non_const_pointer = const_cast<LPVOID>(pointer);
		xr_free(non_const_pointer);
		return	(0);
	}
	LPVOID	non_const_pointer = const_cast<LPVOID>(pointer);

	return		(BearMemory::Realloc(non_const_pointer, size, "luabind"));
}
void setup_luabind_allocator()
{
	luabind::allocator = &luabind_allocator;
	luabind::allocator_context = 0;
}