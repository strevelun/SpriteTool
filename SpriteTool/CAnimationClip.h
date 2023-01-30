#pragma once

#include <vector>
#include <d2d1.h>

class CSprite;
class CCamera;

class CAnimationClip
{
	static CAnimationClip* m_inst;

	CAnimationClip();
	~CAnimationClip();
private:
	std::vector<CSprite*> m_vecSprite;
	std::vector<CSprite*> m_vecClip;

public:
	static CAnimationClip* GetInst()
	{
		if (m_inst == nullptr)
			m_inst = new CAnimationClip();
		return m_inst;
	}

	static void DestroyInst()
	{
		if (m_inst != nullptr)
			delete m_inst;
		m_inst = nullptr;
	}

	void RenderSprite(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x = 0.f, float _y = 0.f);
	void RenderClip(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x = 0.f, float _y = 0.f, bool _pivot = false);


	unsigned int GetVecSpriteSize() const { return m_vecSprite.size(); }
	unsigned int GetVecClipSize() const { return m_vecClip.size(); }
	CSprite* GetVecSprite(unsigned int idx) const;
	CSprite* GetVecClip(unsigned int idx) const;
	CSprite* GetClipInPos(int _xpos, int _ypos, D2D1_RECT_F& _r, CCamera* _camera);

	void AddSprite(CSprite* _sprite);
	void AddClip(CCamera* _camera, int _xpos, int _ypos);

	void EraseClip(unsigned int idx);
	void EraseSprite(unsigned int idx);
	void ClearVecSpriteAndClip();
};

