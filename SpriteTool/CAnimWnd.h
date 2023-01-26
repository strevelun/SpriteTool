#pragma once

#include <Windows.h>

#include "CBWnd.h"

class CBitmap;
class CAnimViewWnd;
class CSprite;

class CAnimWnd : public CBWnd
{
	CBitmap* m_pImage = nullptr;
	CAnimViewWnd* m_viewWnd = nullptr;
	CSprite* m_curClickedSprite = nullptr;

public:
	CAnimWnd(HINSTANCE _hInstance);
	~CAnimWnd();

	bool Create(int _width, int _height, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	CBitmap* GetBitmap() const { return m_pImage; }
	HWND GetHwnd() const { return m_hWnd; }
	void Render();
};

