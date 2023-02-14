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

	_pRenderTarget->DrawBitmap(bitmap, D2D1::RectF(_x, _y, m_vecSprite[idx]->GetRect().right - m_vecSprite[idx]->GetRect().left + _x, m_vecSprite[idx]->GetRect().bottom - m_vecSprite[idx]->GetRect().top + _y), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(m_vecSprite[idx]->GetRect().left, m_vecSprite[idx]->GetRect().top, m_vecSprite[idx]->GetRect().right, m_vecSprite[idx]->GetRect().bottom));
}

void CAnimationClip::RenderClip(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x, float _y, bool _pivot)
{
	ID2D1Bitmap* bitmap = CBitmap::GetInst()->GetBitmap();

	if (idx >= CAnimationClip::GetInst()->GetVecClipSize()) return;

	CSprite* sprite = CAnimationClip::GetInst()->GetVecClip(idx);

	if (!_pivot)
		_pRenderTarget->DrawBitmap(sprite->GetBitmap(), 
			D2D1::RectF(_x, _y,
			sprite->GetRect().right - sprite->GetRect().left + _x,
			sprite->GetRect().bottom - sprite->GetRect().top + _y),
			1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(0, 0, sprite->GetRect().right, sprite->GetRect().bottom));
	else
	{
		//if (!bitmap) return;
		_pRenderTarget->DrawBitmap(sprite->GetBitmap(), D2D1::RectF(
			_x - sprite->GetPivotX() * (sprite->GetRect().right - sprite->GetRect().left),
			_y - sprite->GetPivotY() * (sprite->GetRect().bottom - sprite->GetRect().top),
			sprite->GetRect().right - sprite->GetRect().left + _x - sprite->GetPivotX() * (sprite->GetRect().right - sprite->GetRect().left),
			sprite->GetRect().bottom - sprite->GetRect().top + _y - sprite->GetPivotY() * (sprite->GetRect().bottom - sprite->GetRect().top)),
			1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(0, 0, sprite->GetRect().right, sprite->GetRect().bottom));
	}
}

void CAnimationClip::AddSprite(CSprite* _sprite)
{
	m_vecSprite.push_back(_sprite);

}

void CAnimationClip::AddClip(ID2D1HwndRenderTarget* _pRenderTarget, CCamera* _camera, int _xpos, int _ypos)
{
	// AddClip을 하면 기존에 vecClip은 전부 삭제. 

	if (!_camera) return;


	for (int i = 0; i < m_vecSprite.size(); i++)
	{
		CSprite sprite = *m_vecSprite[i]; // AnimWnd에 있는 클립들의 Rect는 다름
		D2D1_RECT_F rect = sprite.GetRect();
		rect.left += _camera->GetXPos();
		rect.right += _camera->GetXPos();
		rect.top += _camera->GetYPos();
		rect.bottom += _camera->GetYPos();
		if (rect.left <= _xpos && _xpos <= rect.right && rect.top <= _ypos && rect.bottom >= _ypos)
		{
			sprite.SetSize(D2D1::SizeF(rect.right - rect.left, rect.bottom - rect.top));
			sprite.CopyPixel(_pRenderTarget);
			CSprite* s = new CSprite(sprite);
			m_vecClip.push_back(s);
			break;
		}
	}
}

CSprite* CAnimationClip::GetVecSprite(int idx) const
{
	if (idx < 0) return nullptr;
	if (idx >= m_vecSprite.size()) return nullptr;
	return m_vecSprite[idx];
}

CSprite* CAnimationClip::GetVecClip(int idx) 
{
	if (idx < 0) return {};
	if (idx >= m_vecClip.size()) return {};
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
		D2D1_RECT_F rect = sprite->GetRect();

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

// TODO 고쳐야 함
void CAnimationClip::ClearVecSpriteAndClip()
{
	int size = m_vecSprite.size();

	if (size <= 0) return;

	for (int i = 0; i < size; i++)
		delete m_vecSprite[i];
	m_vecSprite.clear();
	m_vecClip.clear();
}
