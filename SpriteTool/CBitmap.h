#pragma once
#include <d2d1.h>
#include <vector>
#include <string>

class CSprite;
class CCamera;

class CBitmap
{
private:
	static CBitmap* m_inst;
	ID2D1Bitmap* m_bitmap = nullptr;
	DWORD* m_bitmapPixel = nullptr;
	D2D1_SIZE_F m_size;


private:
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
	D2D1_SIZE_F GetBitmapSize() const { return m_size; }
	DWORD* GetBitmapPixel() const { return m_bitmapPixel; }

	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
	void Render(ID2D1HwndRenderTarget* _pRenderTarget, CCamera* _camera, float _x = 0.f, float _y = 0.f);
	
	std::wstring GetPixelColorString(unsigned int _xpos, unsigned int _ypos);
	void KeyColor(DWORD _keyColor);
};

