#include "CSpriteWnd.h"

#include "CApp.h"
#include "CCore.h"

CSpriteWnd::CSpriteWnd(HINSTANCE hInstance)
{
	m_hInst = hInstance;

}

CSpriteWnd::~CSpriteWnd()
{
}

bool CSpriteWnd::Create(int _width, int _height, int nCmdShow)
{
	if (CBWnd::Create(L"D2DTutWindowClassSprite", _width, _height, nCmdShow) == false)
		return false;

	CCore::GetInst()->CreateRenderTarget(m_hWnd);

	ID2D1Bitmap* bitmap;
	CCore::GetInst()->LoadBitmapFromFile(L"player.png", &bitmap);

	Render()

	return true;
}

LRESULT CSpriteWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		MessageBox(hWnd, L"Sprite", L"Sprite", MB_OK);
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
