#include "CSprite.h"

CSprite::CSprite()
{
}

CSprite::CSprite(D2D1_RECT_F _rect)
{
	m_size = _rect;
}

CSprite::~CSprite()
{
}
