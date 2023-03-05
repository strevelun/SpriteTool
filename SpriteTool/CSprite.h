#pragma once

#include <d2d1.h>

class CBitmap;

enum class Type
{
	Tile,
	Block,
	Character,
	None
};

class CSprite
{
private:
	float m_pivotX = 0.5f, m_pivotY = 0.5f;

	D2D1_RECT_F m_rect = {};
	D2D1_SIZE_F m_size = {};
	DWORD* m_pixel = nullptr;

	ID2D1Bitmap* m_bitmap = nullptr;
	Type m_type;

public:
	CSprite();
	CSprite(const CSprite& sprite);
	CSprite(D2D1_RECT_F _rect);
	~CSprite();

	void SetRect(D2D1_RECT_F _rect) { m_rect = _rect; }
	void SetPivotX(float _pivotX) { m_pivotX = _pivotX; }
	void SetPivotY(float _pivotY) { m_pivotY = _pivotY; }
	void SetSize(D2D1_SIZE_F _size) { m_size = _size; }

	D2D1_RECT_F GetRect() const { return m_rect; }
	D2D1_SIZE_F GetSize() const { return m_size; }
	float GetPivotX() const { return m_pivotX; }
	float GetPivotY() const { return m_pivotY; }

	void CopyPixel(ID2D1HwndRenderTarget* _pRenderTarget);
	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
	DWORD* GetPixel() const { return m_pixel; }
	void SetPixel(DWORD* _pixel) { m_pixel = _pixel; }
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }

	void SetType(Type _type) { m_type = _type; }
};

