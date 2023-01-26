#include "CAnimWnd.h"

#include "CApp.h"
#include "CCore.h"
#include "CBitmap.h"
#include "CSprite.h"
#include "resource.h"
#include "CAnimViewWnd.h"

CAnimWnd::CAnimWnd(HINSTANCE hInstance)
{
	m_hInst = hInstance;
}

CAnimWnd::~CAnimWnd()
{
	if (m_pRenderTarget) m_pRenderTarget->Release();
}

bool CAnimWnd::Create(int _width, int _height, int nCmdShow)
{
	if (CBWnd::Create(L"D2DTutWindowClassAnim", _width, _height, nCmdShow,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, IDR_MENU2) == false)
		return false;

	InvalidateRgn(m_hWnd, NULL, true);

	return true;
}

LRESULT CAnimWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_PIVOT:
			break;
		case ID_ANIMATION:
			m_viewWnd = new CAnimViewWnd(NULL);
			m_viewWnd->Create(500, 500, SW_SHOWNORMAL);
			break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Render();

		EndPaint(hWnd, &ps);
		break;

	case WM_MOUSEMOVE:
		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_RBUTTONDOWN:
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

void CAnimWnd::Render()
{
	if (!m_pRenderTarget)	return;
	//if (!m_pImage)			return;

	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	//m_pImage->Render(m_pRenderTarget);

	int pos = 0;

	for (int i = 0; i < CBitmap::GetInst()->GetVecClipSize(); i++)
	{
		CSprite* sprite = CBitmap::GetInst()->GetVecClip(i);
		CBitmap::GetInst()->RenderClip(m_pRenderTarget, i, pos);
		D2D1_RECT_F rect = sprite->GetSize();
		rect.left = pos;
		rect.bottom -= rect.top;
		rect.top = 0;
		pos += sprite->GetSize().right - sprite->GetSize().left;
		rect.right = pos;
		m_pRenderTarget->DrawRectangle(rect, m_pBlackBrush);


	}

	m_pRenderTarget->EndDraw();
}
