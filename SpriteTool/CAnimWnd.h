#pragma once

#include <Windows.h>

#include "CBWnd.h"

class CAnimWnd :
	public CBWnd
{
public:
	CAnimWnd(HINSTANCE _hInstance);
	~CAnimWnd();

	bool Create(int _width, int _height, int nCmdShow) override;
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

