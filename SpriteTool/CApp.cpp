#include "CApp.h"
#include "CCore.h"
#include "CBWnd.h"

#include <windowsx.h>

CApp* CApp::m_inst = nullptr;



HRESULT CApp::Init(HINSTANCE hInstance, int nCmdShow)
{

	CBWnd wnd(hInstance);
	wnd.Create(800, 600, nCmdShow);



	return S_OK;
}

int CApp::Run()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
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

