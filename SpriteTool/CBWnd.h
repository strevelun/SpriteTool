#pragma once

#include <Windows.h>


class CBWnd
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInst;

public:
	CBWnd();
	CBWnd(HINSTANCE _hInstance);
	virtual ~CBWnd();

	bool Create(LPCWSTR _lpszClassName, int _width, int _height, int nCmdShow) ;

	virtual LRESULT Proc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}

	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
};

