#include "CBWnd.h"
#include "CApp.h"


LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

CBWnd::CBWnd()
{
}

CBWnd::CBWnd(HINSTANCE hInstance)
{
	m_hInst = hInstance;

}

CBWnd::~CBWnd()
{
}