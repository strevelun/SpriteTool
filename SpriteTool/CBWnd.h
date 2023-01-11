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
	~CBWnd();

	virtual bool Create(int _width, int _height, int nCmdShow) = 0;
	virtual LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

