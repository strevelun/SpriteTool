#include "CAnimWnd.h"

#include "CApp.h"
#include "CCore.h"

CAnimWnd::CAnimWnd(HINSTANCE hInstance)
{
	m_hInst = hInstance;

}

CAnimWnd::~CAnimWnd()
{
}

bool CAnimWnd::Create(int _width, int _height, int nCmdShow)
{
	if (CBWnd::Create(L"D2DTutWindowClassAnim", _width, _height, nCmdShow) == false)
		return false;

	CCore::GetInst()->CreateRenderTarget(m_hWnd);

	return true;
}

LRESULT CAnimWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONDOWN:
		MessageBox(hWnd, L"Anim", L"Anim", MB_OK);
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
