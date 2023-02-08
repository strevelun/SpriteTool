#pragma once

#include <Windows.h>
#include <d2d1.h>  // header include

#include "CBWnd.h"
#include "CBitmap.h"
#include "CMouse.h"
#include "CCamera.h"

enum class MouseMode
{
	AutoSlice,
	DragSlice,
	RemoveSprite,
	AddClip,
};

class CSpriteWnd
	: public CBWnd
{
private:
	CMouse* m_pMouse = nullptr;
	MouseMode m_mode = MouseMode::AutoSlice;
	CCamera* m_camera = nullptr;

	DWORD m_keyColor = 0;
	int m_count = 0;
	int m_addClips = 0;

private:
	void Find(std::vector<std::vector<int>>& _visited, int _curX, int _curY);
	void AutoSliceSprite();
	void DragSprite(CCamera* _camera, int _startPosX, int _startPosY, int _endPosX, int _endPosY);
	void RemoveSprite(CCamera* _camera, int _startPosX, int _startPosY, int _endPosX, int _endPosY, int& _addClips);

public:
	CSpriteWnd(HINSTANCE _hInstance);
	~CSpriteWnd();

	bool Create(int _width, int _height, int nCmdShow);
	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void GridSlice();

	void Render();

};

