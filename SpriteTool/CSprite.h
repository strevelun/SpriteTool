#pragma once

#include <d2d1.h>

class CBitmap;

class CSprite
{
private:
	float m_pivotX = 0.5f, m_pivotY = 0.5f;

	D2D1_RECT_F m_size = {};
	DWORD* m_pixel = nullptr;

	ID2D1Bitmap* m_bitmap = nullptr;

public:
	CSprite();
	CSprite(const CSprite& sprite);
	CSprite(D2D1_RECT_F _rect);
	~CSprite();

	void SetSize(D2D1_RECT_F _rect) { m_size = _rect; }
	void SetPivotX(float _pivotX) { m_pivotX = _pivotX; }
	void SetPivotY(float _pivotY) { m_pivotY = _pivotY; }

	D2D1_RECT_F GetSize() const { return m_size; }
	float GetPivotX() const { return m_pivotX; }
	float GetPivotY() const { return m_pivotY; }

	void CopyPixel(ID2D1HwndRenderTarget* _pRenderTarget);
	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
	DWORD* GetPixel() const { return m_pixel; }
	void SetPixel(DWORD* _pixel) { m_pixel = _pixel; }
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
};

