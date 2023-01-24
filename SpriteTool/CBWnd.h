#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

class CBWnd
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInst;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	ID2D1SolidColorBrush* m_pBlackBrush = nullptr;

	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pDWTextFormat = nullptr;

public:
	CBWnd();
	CBWnd(HINSTANCE _hInstance);
	virtual ~CBWnd();

	bool Create(LPCWSTR _lpszClassName, int _width, int _height, int nCmdShow) ;
	virtual void Render();

	virtual LRESULT Proc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}

	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
};

