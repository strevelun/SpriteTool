#pragma once

#include <d2d1.h>

class CSprite
{
private:
	float m_pivotX = 1.0f, m_pivotY = 0.5f;

	D2D1_RECT_F m_size = {};
	ID2D1Bitmap* m_bitmap = nullptr;

public:
	CSprite();
	CSprite(D2D1_RECT_F _rect);
	~CSprite();

	void SetSize(D2D1_RECT_F _rect) { m_size = _rect; }
	void SetPivotX(float _pivotX) { m_pivotX = _pivotX; }
	void SetPivotY(float _pivotY) { m_pivotY = _pivotY; }

	D2D1_RECT_F GetSize() const { return m_size; }
	float GetPivotX() const { return m_pivotX; }
	float GetPivotY() const { return m_pivotY; }
};

