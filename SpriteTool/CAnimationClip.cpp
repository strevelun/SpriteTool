#include "CAnimationClip.h"
#include "CSprite.h"
#include "CBitmap.h"
#include "CCamera.h"

CAnimationClip* CAnimationClip::m_inst = nullptr;

CAnimationClip::CAnimationClip()
{
}

CAnimationClip::~CAnimationClip()
{
}

void CAnimationClip::RenderSprite(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x, float _y)
{
	ID2D1Bitmap* bitmap = CBitmap::GetInst()->GetBitmap();
	if (!bitmap) return;
	if (idx >= m_vecSprite.size()) return;

	_pRenderTarget->DrawBitmap(bitmap, D2D1::RectF(_x, _y, m_vecSprite[idx]->GetSize().right - m_vecSprite[idx]->GetSize().left + _x, m_vecSprite[idx]->GetSize().bottom - m_vecSprite[idx]->GetSize().top + _y), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(m_vecSprite[idx]->GetSize().left, m_vecSprite[idx]->GetSize().top, m_vecSprite[idx]->GetSize().right, m_vecSprite[idx]->GetSize().bottom));
}

void CAnimationClip::RenderClip(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x, float _y, bool _pivot)
{
	ID2D1Bitmap* bitmap = CBitmap::GetInst()->GetBitmap();
	if (!bitmap) return;
	if (idx >= CAnimationClip::GetInst()->GetVecClipSize()) return;

	CSprite* sprite = CAnimationClip::GetInst()->GetVecClip(idx);

	if (!_pivot)
		_pRenderTarget->DrawBitmap(bitmap, D2D1::RectF(_x, _y,
			sprite->GetSize().right - sprite->GetSize().left + _x,
			sprite->GetSize().bottom - sprite->GetSize().top + _y),
			1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(sprite->GetSize().left, sprite->GetSize().top, sprite->GetSize().right, sprite->GetSize().bottom));
	else
		_pRenderTarget->DrawBitmap(bitmap, D2D1::RectF(
			_x - sprite->GetPivotX() * (sprite->GetSize().right - sprite->GetSize().left),
			_y - sprite->GetPivotY() * (sprite->GetSize().bottom - sprite->GetSize().top),
			sprite->GetSize().right - sprite->GetSize().left + _x - sprite->GetPivotX() * (sprite->GetSize().right - sprite->GetSize().left),
			sprite->GetSize().bottom - sprite->GetSize().top + _y - sprite->GetPivotY() * (sprite->GetSize().bottom - sprite->GetSize().top)),
			1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(sprite->GetSize().left, sprite->GetSize().top, sprite->GetSize().right, sprite->GetSize().bottom));
}

void CAnimationClip::AddSprite(CSprite* _sprite)
{
	m_vecSprite.push_back(_sprite);
}

void CAnimationClip::AddClip(CCamera* _camera, int _xpos, int _ypos)
{
	if (!_camera) return;

	for (int i = 0; i < m_vecSprite.size(); i++)
	{
		CSprite* sprite = m_vecSprite[i]; // AnimWnd에 있는 클립들의 Rect는 다름
		D2D1_RECT_F rect = sprite->GetSize();
		rect.left += _camera->GetXPos();
		rect.right += _camera->GetXPos();
		rect.top += _camera->GetYPos();
		rect.bottom += _camera->GetYPos();
		if (rect.left <= _xpos && _xpos <= rect.right && rect.top <= _ypos && rect.bottom >= _ypos)
		{
			m_vecClip.push_back(sprite);
			break;
		}
	}
}

CSprite* CAnimationClip::GetVecSprite(unsigned int idx) const
{
	if (idx >= m_vecSprite.size()) return nullptr;
	return m_vecSprite[idx];
}

CSprite* CAnimationClip::GetVecClip(unsigned int idx) const
{
	if (idx >= m_vecClip.size()) return nullptr;
	return m_vecClip[idx];
}

CSprite* CAnimationClip::GetClipInPos(int _xpos, int _ypos, D2D1_RECT_F& _r, CCamera* _camera)
{
	size_t size = m_vecClip.size();
	if (size <= 0) return {};

	CSprite* sprite = nullptr;
	int totalWidth = 0;

	for (int i = 0; i < size; i++)
	{
		sprite = m_vecClip[i];
		D2D1_RECT_F rect = sprite->GetSize();

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		// top은 항상 0이고 left는 totalWidth

		if (totalWidth <= _xpos && totalWidth + width >= _xpos && 0 <= _ypos && height >= _ypos)
		{
			_r.left = totalWidth;
			_r.right = totalWidth + width;
			_r.top = 0;
			_r.bottom = height;
			break;
		}
		totalWidth += width;
	}

	return sprite;
}

void CAnimationClip::EraseClip(unsigned int idx)
{
	if (idx >= m_vecClip.size())	return;
	m_vecClip.erase(m_vecClip.begin() + idx);
}

void CAnimationClip::EraseSprite(unsigned int idx)
{
	if (idx >= m_vecSprite.size())	return;
	m_vecSprite.erase(m_vecSprite.begin() + idx);
}

void CAnimationClip::ClearVecSpriteAndClip()
{
	int size = m_vecSprite.size();

	if (size <= 0) return;

	for (int i = 0; i < size; i++)
		delete m_vecSprite[i];
	m_vecSprite.clear();
	m_vecClip.clear();
}
