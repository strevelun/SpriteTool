#include "CSpriteWnd.h"

#include "CApp.h"
#include "CCore.h"
#include "resource.h"
#include "CAnimWnd.h"
#include "CSprite.h"

#include <stack>
#include <windowsx.h>

#pragma comment( lib, "d2d1.lib " ) 

CSpriteWnd::CSpriteWnd(HINSTANCE hInstance)
{
	m_hInst = hInstance;

}

CSpriteWnd::~CSpriteWnd()
{
	if (m_pRenderTarget) m_pRenderTarget->Release();
	delete m_pMouse;
}

bool CSpriteWnd::Create(int _width, int _height, int nCmdShow)
{
	if (CBWnd::Create(L"D2DTutWindowClassSprite", _width, _height, nCmdShow) == false)
		return false;

	m_pMouse = new CMouse();

	//CCore::GetInst()->LoadBitmapFromFile(L"player.png", m_pRenderTarget, &m_bitmap);

	InvalidateRgn(m_hWnd, NULL, true);
		
	return true;
}

LRESULT CSpriteWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_LOAD_IMAGE:

			CBitmap::GetInst()->OpenFile(hWnd, m_pRenderTarget);
			/*
			CAnimWnd* animWnd = CApp::GetInst()->GetAnimWnd();
			animWnd->GetBitmap()->SetBitmap(m_pImage->GetBitmap());
			InvalidateRgn(animWnd->GetHwnd(), NULL, true);
			*/
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Render();

		EndPaint(hWnd, &ps);
		break;

	case WM_MOUSEMOVE:
	{
		m_pMouse->UpdateMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		InvalidateRgn(m_hWnd, NULL, false);
		break;

	}
	case WM_LBUTTONDOWN: 
		m_pMouse->UpdateMouseStartPos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		m_pMouse->UpdateClickState(true);
		break;

	case WM_LBUTTONUP: 
#ifdef _DEBUG
		char str[100];
		sprintf_s(str, "%d %d -> %d %d\n", m_pMouse->GetStartMouseX(), m_pMouse->GetStartMouseY(), m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
		OutputDebugStringA(str);
#endif

		m_pMouse->UpdateClickState(false);
		InvalidateRgn(m_hWnd, NULL, false);
		D2D1_RECT_F rect = DetectSprite(hWnd, m_pMouse->GetStartMouseX(), m_pMouse->GetStartMouseY(),
			m_pMouse->GetMouseX(), m_pMouse->GetMouseY());

		if (rect.right == 0)
		{
			CBitmap::GetInst()->ClearVecSprite();
			break;
		}

		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->DrawRectangle(rect, m_pBlackBrush);
		m_pRenderTarget->EndDraw();

		break;

	case WM_MOUSEWHEEL:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void CSpriteWnd::Render()
{
	if (!m_pRenderTarget) return;
	if (!m_pMouse) return;

	//if (!m_pImage)
	//	return;

	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	//m_pImage->Render(m_pRenderTarget);
	CBitmap::GetInst()->Render(m_pRenderTarget);
	m_pMouse->Render(m_pRenderTarget, m_pBlackBrush);

	for(int i=0; i< CBitmap::GetInst()->GetVecSpriteSize(); i++)
	{
		CSprite * sprite = CBitmap::GetInst()->GetVecSprite(i);
		D2D1_RECT_F rect = sprite->GetSize();
		m_pRenderTarget->DrawRectangle(rect, m_pBlackBrush);
	}


	m_pRenderTarget->EndDraw();
}

D2D1_RECT_F CSpriteWnd::DetectSprite(HWND _hWnd, int _startPosX, int _startPosY, int _endPosX, int _endPosY)
{
	if (_startPosX > _endPosX)
	{
		int t = _startPosX;
		_startPosX = _endPosX;
		_endPosX = t;
	}

	if (_startPosY > _endPosY)
	{
		int t = _startPosY;
		_startPosY = _endPosY;
		_endPosY = t;
	}

	HDC hdc = GetDC(_hWnd);
	COLORREF rgb;
	D2D1_RECT_F rect = {0};

	int minX = 999999, minY = 999999;
	int maxX = 0, maxY = 0;

	for (int i = _startPosY+1; i < _endPosY-1; i++)
	{
		for (int j = _startPosX+1; j < _endPosX-1; j++)
		{
			rgb = GetPixel(hdc, j, i);

			if (rgb != 16777215)
			{
				if (minX > j)
					minX = j;
				if (maxX < j)
					maxX = j;
				if (minY > i)
					minY = i;
				if (maxY < i)
					maxY = i;
			}
		}
	}

	if (minX < maxX)
	{
		rect.left = minX;
		rect.top = minY;
		rect.right = maxX;
		rect.bottom = maxY;
	}

	CSprite* sprite = new CSprite(rect);
	CBitmap::GetInst()->AddSprite(sprite);

	ReleaseDC(_hWnd, hdc);
	return rect;
}
