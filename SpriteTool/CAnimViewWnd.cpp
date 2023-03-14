#include "CAnimViewWnd.h"
#include "CBitmap.h"
#include "CSprite.h"
#include "CAnimationClip.h"
#include "ToolManager.h"

CAnimViewWnd::CAnimViewWnd(HINSTANCE _hInstance)
{
    m_hInst = _hInstance;
}

CAnimViewWnd::~CAnimViewWnd()
{
    if (m_pRenderTarget) m_pRenderTarget->Release();
}

bool CAnimViewWnd::Create(int _width, int _height, int nCmdShow)
{
	if (CBWnd::Create(L"D2DTutWindowClassAnimView", _width, _height, nCmdShow,  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, NULL) == false)
		return false;

	InvalidateRgn(m_hWnd, NULL, true);
		
	return true;
}

LRESULT CAnimViewWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 500, NULL);
		break;
	case WM_TIMER:
		InvalidateRgn(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Render();

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		//PostQuitMessage(0);
		//KillTimer(hWnd, 1);
		ShowWindow(m_hWnd, SW_HIDE);

		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void CAnimViewWnd::Render()
{
	if (!m_pRenderTarget)	return;

	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	int size = ToolManager::GetInst()->GetAnimClip()->GetVecClipSize();
	if (size <= 0) return;

	m_curIdx = (m_curIdx + 1) % size;

	ToolManager::GetInst()->GetAnimClip()->RenderClip(m_pRenderTarget, m_curIdx, 200, 200, true);

	m_pRenderTarget->EndDraw();
}
