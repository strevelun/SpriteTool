#pragma once

#include <Windows.h>
#include <d2d1.h>  // header include

#include "CBWnd.h"
#include "CBitmap.h"
#include "CMouse.h"


class CSpriteWnd
	: public CBWnd
{
	CBitmap* m_pImage = nullptr;
	CMouse* m_pMouse = nullptr;

public:
	CSpriteWnd(HINSTANCE _hInstance);
	~CSpriteWnd();

	bool Create(int _width, int _height, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Render();
};

