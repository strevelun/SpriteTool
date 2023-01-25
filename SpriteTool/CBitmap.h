#pragma once
#include <d2d1.h>
#include <vector>
#include <string>

class CSprite;

class CBitmap
{
private:
	static CBitmap* m_inst;
	ID2D1Bitmap* m_bitmap = nullptr;
	DWORD* m_bitmapPixel = nullptr;
	D2D1_SIZE_F m_size;

	std::vector<CSprite*> m_vecSprite;

	CBitmap();
	~CBitmap();

public:
	static CBitmap* GetInst() 
	{
		if (m_inst == nullptr)
			m_inst = new CBitmap();
		return m_inst;
	}

	static void DestroyInst()
	{
		if (m_inst != nullptr)
		{
			delete m_inst;
			m_inst = nullptr;
		}
	}

	void OpenFile(HWND _hWnd, ID2D1HwndRenderTarget* _pRenderTarget);
	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
	void Render(ID2D1HwndRenderTarget* _pRenderTarget, float _x = 0.f, float _y = 0.f);
	void RenderSprite(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x = 0.f, float _y = 0.f);

	void AddSprite(CSprite* _sprite);
	
	CSprite* GetVecSprite(unsigned int idx) const 
	{ 
		if (idx >= m_vecSprite.size()) return nullptr;
		return m_vecSprite[idx]; 
	}

	unsigned int GetVecSpriteSize() const { return m_vecSprite.size(); }
	void ClearVecSprite();

	std::wstring GetPixelColorString(unsigned int _xpos, unsigned int _ypos);

	void AutoSliceSprite();
};

