#include "CSpriteWnd.h"

#include "CApp.h"
#include "CCore.h"
#include "resource.h"
#include "CAnimWnd.h"

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
	delete m_pImage;
	delete m_pMouse;
}

bool CSpriteWnd::Create(int _width, int _height, int nCmdShow)
{
	if (CBWnd::Create(L"D2DTutWindowClassSprite", _width, _height, nCmdShow) == false)
		return false;

	m_pImage = new CBitmap();
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

			m_pImage->OpenFile(hWnd, m_pRenderTarget);
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
		m_pMouse->UpdateMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		InvalidateRgn(m_hWnd, NULL, false);
		break;

	case WM_LBUTTONDOWN: 
		m_pMouse->UpdateMouseStartPos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		m_pMouse->UpdateClickState(true);
		break;

	case WM_LBUTTONUP: 
		m_pMouse->UpdateClickState(false);
		InvalidateRgn(m_hWnd, NULL, false);
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
	if (!m_pRenderTarget)
		return;

	if (!m_pImage)
		return;

	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	m_pImage->Render(m_pRenderTarget);
	m_pMouse->Render(m_pRenderTarget, m_pBlackBrush);

	m_pRenderTarget->EndDraw();
}
