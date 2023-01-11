#include "CApp.h"
#include "CCore.h"
#include "CBWnd.h"
#include "CAnimWnd.h"
#include "CSpriteWnd.h"

#include <windowsx.h>

CApp* CApp::m_inst = nullptr;

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	CBWnd* pWnd = nullptr;

	if (_message == WM_NCCREATE)
	{
		LPCREATESTRUCT pCS = (LPCREATESTRUCT)_lParam;
		SetLastError(0);
		pWnd = (CBWnd*)pCS->lpCreateParams;

		if (!SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)pWnd))
		{
			if (GetLastError() != 0)
				return E_FAIL;
		}
	}
	else
	{
		pWnd = reinterpret_cast<CBWnd*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));
	}

	if (pWnd)
		return pWnd->Proc(_hWnd, _message, _wParam, _lParam);

	return DefWindowProc(_hWnd, _message, _wParam, _lParam);
}

HRESULT CApp::Init(HINSTANCE hInstance, int nCmdShow)
{
	m_animWnd = new CAnimWnd(hInstance);
	if(m_animWnd->Create(800, 600, nCmdShow) == false)
		return E_FAIL;

	m_spriteWnd = new CSpriteWnd(hInstance);
	if (m_spriteWnd->Create(300, 200, nCmdShow) == false)
		return E_FAIL;

	return S_OK;
}

int CApp::Run()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}

	CCore::GetInst()->CleanupDevice();
	CCore::DeleteInst();

	return (int)msg.wParam;
}

