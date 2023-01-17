#pragma once

#include <windows.h>

#define MAX_LOADSTRING 100
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

class CSpriteWnd;
class CAnimWnd;

class CApp
{
private:
	static CApp* m_inst;

	WCHAR szTitle[MAX_LOADSTRING] = {};
	WCHAR szWindowClass[MAX_LOADSTRING] = {};

	HINSTANCE m_hInst = nullptr;
	HWND m_hWnd = nullptr;

	int m_mouseX, m_mouseY;
	int m_mag = 100; // 확대, 축소 배율

	CAnimWnd* m_animWnd;
	CSpriteWnd* m_spriteWnd;

private:
	CApp() { }
	~CApp() {}


public:
	static CApp* GetInst()
	{
		if (m_inst == nullptr)  m_inst = new CApp();
		return m_inst;
	}

	HRESULT Init(HINSTANCE hInstance, int nCmdShow);
	int Run();

	CAnimWnd* GetAnimWnd() const { return m_animWnd; }
	CSpriteWnd* GetSpriteWnd() const { return m_spriteWnd; }

	LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};