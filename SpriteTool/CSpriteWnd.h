#pragma once

#include <Windows.h>

#include "CBWnd.h"
#include <d2d1.h>  // header include



#pragma comment( lib, "d2d1.lib " ) 

class CSpriteWnd
	: public CBWnd
{
public:
	CSpriteWnd(HINSTANCE _hInstance);
	~CSpriteWnd();

	bool Create(int _width, int _height, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	void Render(ID2D1HwndRenderTarget* _pRenderTarget, ID2D1Bitmap* _bitmap)
	{
		_pRenderTarget->DrawBitmap(_bitmap, D2D1::RectF(0, 0, 100, 100), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(0.0f, 0.0f, _bitmap->GetSize().width, _bitmap->GetSize().height));
	}
};

