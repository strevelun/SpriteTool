#pragma once

#include <vector>
#include <d2d1.h>
#include "CSprite.h"

class CCamera;

class CAnimationClip
{

private:
	std::vector<CSprite*> m_vecSprite;
	std::vector<CSprite*> m_vecClip;

public:
	CAnimationClip();
	~CAnimationClip();


	void RenderSprite(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x = 0.f, float _y = 0.f);
	void RenderClip(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x = 0.f, float _y = 0.f, bool _pivot = false);


	unsigned int GetVecSpriteSize() const { return m_vecSprite.size(); }
	unsigned int GetVecClipSize() const { return m_vecClip.size(); }
	CSprite* GetVecSprite(int idx) const;
	CSprite* GetVecClip(int idx);
	CSprite* GetClipInPos(int _xpos, int _ypos, D2D1_RECT_F& _r, CCamera* _camera);

	void AddSprite(CSprite* _sprite);
	void AddClip(ID2D1HwndRenderTarget* _pRenderTarget, CCamera* _camera, int _xpos, int _ypos);
	void AddClip(CSprite* _sprite) {	m_vecClip.push_back(_sprite); }

	void EraseClip(unsigned int idx);
	void EraseSprite(unsigned int idx);
	void ClearVecSpriteAndClip();
};

