#pragma once

#include <Windows.h>

#include "CBWnd.h"

class CSpriteWnd
	: public CBWnd
{
public:
	CSpriteWnd(HINSTANCE _hInstance);
	~CSpriteWnd();

	bool Create(int _width, int _height, int nCmdShow) override;
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

