#include "CSpriteWnd.h"

#include "CApp.h"
#include "CCore.h"
#include "resource.h"
#include "CAnimWnd.h"
#include "CSprite.h"

#include <stack>
#include <windowsx.h>
#include <string>

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
	if (CBWnd::Create(L"D2DTutWindowClassSprite", _width, _height, nCmdShow, IDR_MENU1) == false)
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
			break;
		case ID_AUTO_SLICE:
			m_mode = MouseMode::AutoSlice;
			CBitmap::GetInst()->AutoSliceSprite();
			break;
		case ID_DRAG_SLICE:
			m_mode = MouseMode::DragSlice;
			break;
		case ID_ADD_CLIP:
			m_mode = MouseMode::AddClip;
			break;
		case ID_KEY_COLOR:
			break;
		case ID_SPRITE:
			m_mode = MouseMode::RemoveSprite;

			break;
		case ID_ALL_BOXES:
			CBitmap::GetInst()->ClearVecSpriteAndClip();
			InvalidateRgn(m_hWnd, NULL, false);

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
		int xpos = GET_X_LPARAM(lParam);
		int ypos = GET_Y_LPARAM(lParam);
		m_pMouse->UpdateMousePos(xpos, ypos);
		CBitmap::GetInst()->GetPixelColorString(xpos, ypos);

		InvalidateRgn(m_hWnd, NULL, false);
		break;
	}
	case WM_LBUTTONDOWN: 
		m_pMouse->UpdateMouseStartPos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		m_pMouse->UpdateClickState(true);
		break;

	case WM_LBUTTONUP: 
		// LBUTTONDOWN 한 적이 없으면 break. 이미지 열기할 때 들어옴
		if (m_pMouse->GetClickState() == false) break;

		m_pMouse->UpdateClickState(false);

		if (m_mode == MouseMode::DragSlice)
			CBitmap::GetInst()->DragSprite(m_pMouse->GetStartMouseX(), m_pMouse->GetStartMouseY(),
				m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
		else if (m_mode == MouseMode::RemoveSprite)
			CBitmap::GetInst()->RemoveSprite(m_pMouse->GetStartMouseX(), m_pMouse->GetStartMouseY(),
				m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
		else if (m_mode == MouseMode::AddClip)
			CBitmap::GetInst()->AddClip(m_pMouse->GetMouseX(), m_pMouse->GetMouseY());

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
	if (!m_pRenderTarget) return;
	if (!m_pMouse) return;

	//if (!m_pImage)
	//	return;

	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	CBitmap::GetInst()->Render(m_pRenderTarget);

	if(m_mode == MouseMode::DragSlice || m_mode == MouseMode::RemoveSprite)
		m_pMouse->Render(m_pRenderTarget, m_pBlackBrush);

	for(int i=0; i< CBitmap::GetInst()->GetVecSpriteSize(); i++)
	{
		CSprite * sprite = CBitmap::GetInst()->GetVecSprite(i);
		D2D1_RECT_F rect = sprite->GetSize();
		m_pRenderTarget->DrawRectangle(rect, m_pBlackBrush);
	}

	for (int i = 0; i < CBitmap::GetInst()->GetVecClipSize(); i++)
	{
		CSprite* sprite = CBitmap::GetInst()->GetVecClip(i);
		D2D1_RECT_F rect = sprite->GetSize();
		m_pRenderTarget->DrawRectangle(rect, m_pRedBrush);
	}

	std::wstring wstr = CBitmap::GetInst()->GetPixelColorString(m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
	const WCHAR* strColor = wstr.c_str();
	m_pRenderTarget->DrawTextW(strColor, wstr.length(), m_pDWTextFormat, D2D1::RectF(100, 500, 250, 550), m_pBlackBrush);

	m_pRenderTarget->EndDraw();
}
/*
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
*/