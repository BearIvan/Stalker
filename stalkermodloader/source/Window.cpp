#include "stdafx.h"
LRESULT CALLBACK GlobalOnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;
	}
	Modloader::Window* window = handle ? reinterpret_cast<Modloader::Window*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : NULL;

	if (window)
	{
		window->OnEvent(handle,message, wParam, lParam);
	}
	return DefWindowProc(handle, message, wParam, lParam);
}
Modloader::Window::Window(bsize wigth, bsize height)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(0);
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = GlobalOnEvent;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("BEAR");
	wc.cbSize = sizeof(WNDCLASSEX);


	RegisterClassEx(&wc);
	DWORD Style = WS_OVERLAPPED | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	m_window = CreateWindow(TEXT("BEAR"), TEXT(""), Style, 0, 0, static_cast<int32>(wigth), static_cast<int32>(height), NULL, NULL, hInstance, NULL);
	ShowWindow((HWND)m_window, SW_SHOW);

	uint32 xpos = static_cast<int32>(((uint32)GetSystemMetrics(SM_CXSCREEN) / 2) - (wigth / 2));
	uint32 ypos = static_cast<int32>(((uint32)GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2));

	RECT rectangle = { 0, 0, static_cast<long>(wigth), static_cast<long>(height) };
	AdjustWindowRect(&rectangle, GetWindowLong((HWND)m_window, GWL_STYLE), false);
	int w = rectangle.right - rectangle.left;
	int h = rectangle.bottom - rectangle.top;
	SetWindowPos((HWND)m_window, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
	SetWindowPos((HWND)m_window, NULL, xpos, ypos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	SetWindowLongPtrW(m_window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

}

bool Modloader::Window::Update()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
		return false;
	return true;
}

Modloader::Window::~Window()
{
	DestroyWindow((HWND)m_window);
}

void Modloader::Window::OnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:

	default:
		break;
	}
}


Modloader::Window & Modloader::Window::CreateCombobox(const bchar * name, const bchar * text, bsize x, bsize y, bsize width, bsize height)
{
	Combobox * buttom = BearCore::bear_alloc<Combobox>(1);
	new(buttom)Combobox(GetWindowHandle(), text, x, y, width, height);
	m_comboboxs_map.insert(name, buttom);
	return *this;
}
