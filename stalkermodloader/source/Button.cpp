#include "stdafx.h"

Modloader::Button::Button(
#ifdef WINDOWS
	HWND
#elif LINUX
	void*
#endif
	Handle, const bchar * text, bsize x, bsize y, bsize width, bsize height)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(0);
	m_handle = CreateWindow(TEXT("button"),text, WS_CHILD | WS_VISIBLE, x, y, width, height, Handle, NULL, hInstance, NULL);

}


Modloader::Button::~Button()
{

	DestroyWindow(m_handle);
}
