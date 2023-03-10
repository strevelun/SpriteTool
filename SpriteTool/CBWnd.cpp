#include "CBWnd.h"
#include "CApp.h"
#include "CCore.h"
#include <d2d1.h>
#include "resource.h"

#pragma comment(lib, "dwrite.lib")

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

bool CBWnd::Create(LPCWSTR _lpszClassName, int _width, int _height, int nCmdShow, DWORD _dwStyle, int _menu)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(_menu);
	wcex.lpszClassName = _lpszClassName;
	wcex.hIconSm = LoadIcon(m_hInst, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, _width, _height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(_lpszClassName, L"SpriteTool", _dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_hInst,
		this);
	if (!m_hWnd) return false;

	m_pRenderTarget = CCore::GetInst()->CreateRenderTarget(m_hWnd);
	
	HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
	if (FAILED(hr)) return false;

	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
	if (FAILED(hr)) return false;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (FAILED(hr)) return hr;

	static const WCHAR fontName[] = L"Arial";
	const FLOAT fontSize = 30.0f;

	hr = m_pDWriteFactory->CreateTextFormat(fontName, NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"en-us", &m_pDWTextFormat);
	if (FAILED(hr)) return hr;

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return true;
}

void CBWnd::Render()
{
}


LRESULT CBWnd::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
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
