#pragma once
#include <d2d1.h>
#include <vector>

class CSprite;

class CBitmap
{
	ID2D1Bitmap* m_bitmap;

	std::vector<CSprite*> m_vecSprite;

public:
	void OpenFile(HWND _hWnd, ID2D1HwndRenderTarget* _pRenderTarget);
	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
	void Render(ID2D1HwndRenderTarget* _pRenderTarget, float _x = 0.f, float _y = 0.f);
};

