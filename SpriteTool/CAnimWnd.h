#pragma once

#include <Windows.h>

#include "CBWnd.h"

class CBitmap;

class CAnimWnd : public CBWnd
{
	CBitmap* m_pImage = nullptr;

public:
	CAnimWnd(HINSTANCE _hInstance);
	~CAnimWnd();

	bool Create(int _width, int _height, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	CBitmap* GetBitmap() const { return m_pImage; }
	HWND GetHwnd() const { return m_hWnd; }
	void Render();
};

