#pragma once

#include <Windows.h>

class CBWnd
{
	HWND m_hWnd;
	HINSTANCE m_hInst;

public:
	CBWnd();
	~CBWnd();

	void Create(int _width, int _height, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

