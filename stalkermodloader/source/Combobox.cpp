#include "stdafx.h"

Modloader::Combobox::Combobox(
#ifdef WINDOWS
	HWND
#elif LINUX
	void*
#endif
	 Handle, const bchar * text, bsize x, bsize y, bsize width, bsize hieght)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(0);
	m_handle = CreateWindow("COMBOBOX", text,
		WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWNLIST,
		x, y, width, hieght, 
		Handle, 0, hInstance, 0);

}


Modloader::Combobox::~Combobox()
{
	DestroyWindow(m_handle);
}
