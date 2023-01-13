#include "CApp.h"
#include "CCore.h"
#include "CBWnd.h"
#include "CAnimWnd.h"
#include "CSpriteWnd.h"

#include <windowsx.h>

CApp* CApp::m_inst = nullptr;

HRESULT CApp::Init(HINSTANCE hInstance, int nCmdShow)
{
	CCore::GetInst()->Init();

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
	}

	CCore::GetInst()->CleanupDevice();
	CCore::DeleteInst();

	return (int)msg.wParam;
}

