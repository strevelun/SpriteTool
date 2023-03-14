#pragma once
#include <d2d1.h>
#include <vector>
#include <string>

class CSprite;
class CCamera;
enum class Type;

class CBitmap
{
private:
	ID2D1Bitmap* m_bitmap = nullptr;
	DWORD* m_bitmapPixel = nullptr;
	D2D1_SIZE_F m_size;


private:

public:
	CBitmap();
	~CBitmap();

	void OpenImageFile(HWND _hWnd, ID2D1HwndRenderTarget* _pRenderTarget);
	void SaveImageFile(HWND _hWnd);
	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
	D2D1_SIZE_F GetBitmapSize() const { return m_size; }
	DWORD* GetBitmapPixel() const { 
		return m_bitmapPixel; 
	}

	void SetBitmap(ID2D1Bitmap* _bitmap) { m_bitmap = _bitmap; }
	void Render(ID2D1HwndRenderTarget* _pRenderTarget, CCamera* _camera, float _x = 0.f, float _y = 0.f);
	
	std::wstring GetPixelColorString(unsigned int _xpos, unsigned int _ypos);
	void KeyColor(DWORD _keyColor);

	void SaveClip(HWND _hWnd, Type _type);
	void LoadClip(HWND _hWnd, ID2D1HwndRenderTarget* _pRenderTarget);
};

