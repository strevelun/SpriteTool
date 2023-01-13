#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>



class CCore
{
private:
	static CCore* m_inst;

	CCore();
	~CCore();

	ID2D1Factory* m_pD2DFactory = nullptr;
	ID2D1SolidColorBrush* m_pBlackBrush = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	IWICImagingFactory* m_pWICFactory = nullptr;

private:
	HRESULT InitDevice();

public:

	static CCore* GetInst()
	{
		if (m_inst == nullptr)
			m_inst = new CCore();
		return m_inst;
	}

	static void DeleteInst()
	{
		if (m_inst != nullptr)
		{
			delete m_inst;
			m_inst = nullptr;
		}
	}

	bool Init();

	HRESULT CreateRenderTarget(HWND _hWnd);

	void CleanupDevice();
	HRESULT LoadBitmapFromFile(PCWSTR _wcFileName, ID2D1Bitmap** _pBitmap);
};