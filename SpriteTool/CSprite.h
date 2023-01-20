#pragma once

#include <d2d1.h>

class CSprite
{
private:
	D2D1_RECT_F m_size = {};
	ID2D1Bitmap* m_bitmap = nullptr;

public:
	CSprite(D2D1_RECT_F _rect);
	~CSprite();

	D2D1_RECT_F GetSize() const { return m_size; }
};

