#include "stdafx.h"
#include "api/XrGameVersionController.h"
#include <CommCtrl.h>
#define ID_BUTTON_START 100
#define ID_BUTTON_STOP 200
int ItemIndex = 0;
LRESULT CALLBACK GlobalOnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CLOSE:
		exit(0);
		return 0;
		break;
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_BUTTON_START:
			PostQuitMessage(0);
			return 0;
			break;
		case ID_BUTTON_STOP:
			exit(0);
			break;
		default:
			break;
		}
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
		}
	}

	}
	return DefWindowProc(handle, message, wParam, lParam);
};

void Modloader::Run()
{
	UINT Width = 400;
	UINT Height = 100;


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
	/*uint32 xpos = static_cast<int32>(((uint32)GetSystemMetrics(SM_CXSCREEN) / 2) - (Width / 2));
	uint32 ypos = static_cast<int32>(((uint32)GetSystemMetrics(SM_CYSCREEN) / 2) - (Height / 2));*/
	HWND window = CreateWindow(TEXT("BEAR"), TEXT(""), Style, 0, 0, static_cast<int32>(Width), static_cast<int32>(Height), NULL, NULL, hInstance, NULL);
	
	{
		uint32 xpos = static_cast<int32>(((uint32)GetSystemMetrics(SM_CXSCREEN) / 2) - (Width / 2));
		uint32 ypos = static_cast<int32>(((uint32)GetSystemMetrics(SM_CYSCREEN) / 2) - (Height / 2));

		RECT rectangle = { 0, 0, static_cast<long>(Width), static_cast<long>(Height) };
		AdjustWindowRect(&rectangle, GetWindowLong((HWND)window, GWL_STYLE), false);
		int w = rectangle.right - rectangle.left;
		int h = rectangle.bottom - rectangle.top;
		SetWindowPos((HWND)window, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
		SetWindowPos((HWND)window, NULL, xpos, ypos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		
	}

	HWND start = CreateWindow(WC_BUTTON, TEXT("запустить"), WS_CHILD | WS_VISIBLE, 210, 60, 180, 30, window,(HMENU)ID_BUTTON_START, hInstance, NULL);
	HWND cancel = CreateWindow(WC_BUTTON, TEXT("отмена"), WS_CHILD | WS_VISIBLE, 10, 60, 180, 30, window, (HMENU)ID_BUTTON_STOP, hInstance, NULL);

	HWND combobox = CreateWindow(WC_COMBOBOX, TEXT(""),  WS_CHILD | WS_OVERLAPPED | WS_VISIBLE|CBS_DROPDOWNLIST|CBS_DISABLENOSCROLL, 10, 20, 380, 30*4, window, (HMENU)3, hInstance, 0);
	
	SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)TEXT("S.T.A.L.K.E.R. SOC(1.0.04)"));
	SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)TEXT("S.T.A.L.K.E.R. SOC(1.0.06)"));
	SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)TEXT("S.T.A.L.K.E.R. CS"));
	SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)TEXT("S.T.A.L.K.E.R. COP"));
	SendMessage(combobox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	ShowWindow((HWND)window, SW_SHOW);

	MSG msg;
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;
	}
	switch (ItemIndex)
	{
	case 0:

		gameVersionController = xr_new<GameVersionController>(GameVersionController::SOC_1004);
		break;
	case 1:

		gameVersionController = xr_new<GameVersionController>(GameVersionController::SOC_1007);
		break;
	case 2:

		gameVersionController = xr_new<GameVersionController>(GameVersionController::CS_1510);
		break;
	case 3:

		gameVersionController = xr_new<GameVersionController>(GameVersionController::COP_1602);
		break;
	default:
		break;
	}
	DestroyWindow(combobox);
	DestroyWindow(start);
	DestroyWindow(cancel);
	DestroyWindow(window);


}