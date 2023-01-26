#pragma once
#include "CBWnd.h"
class CAnimViewWnd :
    public CBWnd
{
	int m_curIdx = 0;

public:
	CAnimViewWnd(HINSTANCE _hInstance);
	~CAnimViewWnd();

	bool Create(int _width, int _height, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Render();
};

