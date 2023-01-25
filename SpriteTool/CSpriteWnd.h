#pragma once

#include <Windows.h>
#include <d2d1.h>  // header include

#include "CBWnd.h"
#include "CBitmap.h"
#include "CMouse.h"

enum class MouseMode
{
	AutoSlice,
	DragSlice,
	RemoveSprite,
	AddClip,
};

class CSpriteWnd
	: public CBWnd
{
	CMouse* m_pMouse = nullptr;
	MouseMode m_mode = MouseMode::AutoSlice;


public:
	CSpriteWnd(HINSTANCE _hInstance);
	~CSpriteWnd();

	bool Create(int _width, int _height, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Render();
	//D2D1_RECT_F DetectSprite(HWND _hWnd, int _startPosX, int _startPosY, int _endPosX, int _endPosY);
};

