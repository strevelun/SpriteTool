#include "CSpriteWnd.h"

#include "CApp.h"
#include "CCore.h"
#include "resource.h"
#include "CAnimWnd.h"
#include "CSprite.h"
#include "CAnimationClip.h"

#include <stack>
#include <windowsx.h>
#include <string>
#include <queue>

#pragma comment( lib, "d2d1.lib " ) 

void CSpriteWnd::Find(std::vector<std::vector<bool>>& _visited, int _curX, int _curY)
{
	int curX = _curX, curY = _curY;
	int searchX, searchY;
	//int dir[8][2] = { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1} }; // {x, y}
	int dir[4][2] = { {0,-1}, {1,0}, {0,1},{-1,0} };
	std::queue<std::pair<int, int>*> q;

	q.push(new std::pair<int, int>(curY, curX));
	_visited[curY][curX] = true;

	int minX = 999999, minY = 999999;
	int maxX = 0, maxY = 0;
	D2D1_RECT_F rect = { 0 };

	D2D1_SIZE_F size = CBitmap::GetInst()->GetBitmapSize();
	DWORD* bitmapPixel = CBitmap::GetInst()->GetBitmapPixel();

	while (!q.empty())
	{
		std::pair<int, int>* p = q.front();
		curY = p->first;
		curX = p->second;
		q.pop();
		delete p;



		for (int i = 0; i < 4; i++)
		{
			int nextX = curX + dir[i][0];
			int nextY = curY + dir[i][1];

			if (nextX < 0 || nextX >= size.width || nextY < 0 || nextY >= size.height)
				continue;

			if (!_visited[nextY][nextX] && bitmapPixel[nextY * (int)size.width + nextX] != 0xffffffff)
			{
				q.push(new std::pair<int, int>(nextY, nextX));
				_visited[nextY][nextX] = true;



				if (minX > nextX)
					minX = nextX;
				if (maxX < nextX)
					maxX = nextX;
				if (minY > nextY)
					minY = nextY;
				if (maxY < nextY)
					maxY = nextY;
			}
		}
	}

	if (minX < maxX)
	{
		rect.left = minX;
		rect.top = minY;
		rect.right = maxX;
		rect.bottom = maxY;
	}

	CSprite* sprite = new CSprite(rect);
	CAnimationClip::GetInst()->AddSprite(sprite);
}

void CSpriteWnd::AutoSliceSprite()
{
	ID2D1Bitmap* bitmap = CBitmap::GetInst()->GetBitmap();

	if (!bitmap) return;

	DWORD* bitmapPixel = CBitmap::GetInst()->GetBitmapPixel();
	D2D1_SIZE_F size = CBitmap::GetInst()->GetBitmapSize();

	std::vector<std::vector<bool>> visited(size.height, std::vector<bool>(size.width, false));

	// if(m_bitmapPixel[i * (int)m_size.width + j] & 0xff000000)

	for (int i = 0; i < size.height; i++)
	{
		for (int j = 0; j < size.width; j++)
		{
			if (!visited[i][j] && bitmapPixel[i * (int)size.width + j] != 0xffffffff)  // 0xffff00000
			{
				Find(visited, j, i);

			}
		}
	}
}

void CSpriteWnd::DragSprite(int _startPosX, int _startPosY, int _endPosX, int _endPosY)
{
	D2D1_SIZE_F size = CBitmap::GetInst()->GetBitmapSize();
	DWORD* bitmapPixel = CBitmap::GetInst()->GetBitmapPixel();

	if (_endPosX >= size.width || _endPosY >= size.height) return;
	if (_startPosX >= size.width || _startPosY >= size.height) return;

	if (_startPosX > _endPosX)
	{
		int t = _startPosX;
		_startPosX = _endPosX;
		_endPosX = t;
	}

	if (_startPosY > _endPosY)
	{
		int t = _startPosY;
		_startPosY = _endPosY;
		_endPosY = t;
	}

	D2D1_RECT_F rect = { 0 };
	int minX = 999999, minY = 999999;
	int maxX = 0, maxY = 0;

	for (int i = _startPosY + 1; i < _endPosY - 1; i++)
	{
		for (int j = _startPosX + 1; j < _endPosX - 1; j++)
		{
			if (bitmapPixel[i * (int)size.width + j] != 0xffffffff)
			{
				if (minX > j)
					minX = j;
				if (maxX < j)
					maxX = j;
				if (minY > i)
					minY = i;
				if (maxY < i)
					maxY = i;
			}
		}
	}

	if (minX < maxX)
	{
		rect.left = minX;
		rect.top = minY;
		rect.right = maxX;
		rect.bottom = maxY;
	}

	CSprite* sprite = new CSprite(rect);
	CAnimationClip::GetInst()->AddSprite(sprite);
}

void CSpriteWnd::RemoveSprite(int _startPosX, int _startPosY, int _endPosX, int _endPosY)
{
	D2D1_SIZE_F size = CBitmap::GetInst()->GetBitmapSize();

	if (_endPosX >= size.width || _endPosY >= size.height) return;
	if (_startPosX >= size.width || _startPosY >= size.height) return;

	if (_startPosX > _endPosX)
	{
		int t = _startPosX;
		_startPosX = _endPosX;
		_endPosX = t;
	}

	if (_startPosY > _endPosY)
	{
		int t = _startPosY;
		_startPosY = _endPosY;
		_endPosY = t;
	}

	for (int i = 0; i < CAnimationClip::GetInst()->GetVecSpriteSize(); i++)
	{
		CSprite* sprite = CAnimationClip::GetInst()->GetVecSprite(i);
		D2D1_RECT_F r = sprite->GetSize();
		if (_startPosX <= r.left && r.right <= _endPosX && _startPosY <= r.top && _endPosY >= r.bottom)
		{
			for (int j = 0; j < CAnimationClip::GetInst()->GetVecClipSize(); j++)
			{
				D2D1_RECT_F r2 = CAnimationClip::GetInst()->GetVecClip(j)->GetSize();
				if (r.left == r2.left && r.top == r2.top && r.right == r2.right && r.bottom == r2.bottom)
				{
					CAnimationClip::GetInst()->EraseClip(j);
					break;
				}
			}

			delete sprite;
			CAnimationClip::GetInst()->EraseClip(i);
			i--; // 벡터 원소 지우면서 크기 변화때문에 스킵하는 원소 발생 방지
		}
	}
}

CSpriteWnd::CSpriteWnd(HINSTANCE hInstance)
{
	m_hInst = hInstance;

}

CSpriteWnd::~CSpriteWnd()
{
	if (m_pRenderTarget) m_pRenderTarget->Release();
	delete m_pMouse;
	delete m_camera;
}

bool CSpriteWnd::Create(int _width, int _height, int nCmdShow)
{
	if (CBWnd::Create(L"D2DTutWindowClassSprite", _width, _height, nCmdShow,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, IDR_MENU1) == false)
		return false;

	m_pMouse = new CMouse();
	m_camera = new CCamera();

	//CCore::GetInst()->LoadBitmapFromFile(L"player.png", m_pRenderTarget, &m_bitmap);

	InvalidateRgn(m_hWnd, NULL, true);
		
	return true;
}

LRESULT CSpriteWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_LOAD_IMAGE:
			CBitmap::GetInst()->OpenFile(hWnd, m_pRenderTarget);
			break;
		case ID_AUTO_SLICE:
			m_mode = MouseMode::AutoSlice;
			AutoSliceSprite();
			break;
		case ID_DRAG_SLICE:
			m_mode = MouseMode::DragSlice;
			break;
		case ID_ADD_CLIP:
			m_mode = MouseMode::AddClip;
			break;
		case ID_KEY_COLOR:
			CBitmap::GetInst()->KeyColor(m_keyColor);
			break;
		case ID_SPRITE:
			m_mode = MouseMode::RemoveSprite;

			break;
		case ID_ALL_BOXES:
			CAnimationClip::GetInst()->ClearVecSpriteAndClip();
			InvalidateRgn(m_hWnd, NULL, false);

			break;
		}
		break;
	}

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			if (m_camera->GetXPos() < CBitmap::GetInst()->GetBitmapSize().width)
				m_camera->UpdateXPos(5);
			break;
		case VK_RIGHT:
			//if (m_camera->GetXPos() > 0)
				m_camera->UpdateXPos(-5);
			break;
		case VK_UP:
				m_camera->UpdateYPos(5);
			break;
		case VK_DOWN:
			//if(m_camera->GetYPos() > 0)
				m_camera->UpdateYPos(-5);
			break;
		}
		InvalidateRect(hWnd, NULL, false);

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Render();

		EndPaint(hWnd, &ps);
		break;

	case WM_MOUSEMOVE:
	{
		int xpos = GET_X_LPARAM(lParam);
		int ypos = GET_Y_LPARAM(lParam);
		m_pMouse->UpdateMousePos(xpos, ypos);
		CBitmap::GetInst()->GetPixelColorString(xpos, ypos);

		InvalidateRgn(m_hWnd, NULL, false);
		break;
	}
	case WM_LBUTTONDOWN: 
	{
		int xpos = GET_X_LPARAM(lParam);
		int ypos = GET_Y_LPARAM(lParam);

		m_pMouse->UpdateMouseStartPos(xpos, ypos);
		m_pMouse->UpdateClickState(true);

		m_keyColor = CBitmap::GetInst()->GetBitmapPixel()[ypos * (int)CBitmap::GetInst()->GetBitmapSize().width + xpos];

		break;
	}
	case WM_LBUTTONUP: 
	{
		// LBUTTONDOWN 한 적이 없으면 break. 이미지 열기할 때 들어옴
		if (m_pMouse->GetClickState() == false) break;

		m_pMouse->UpdateClickState(false);

		if (m_mode == MouseMode::DragSlice)
			DragSprite(m_pMouse->GetStartMouseX(), m_pMouse->GetStartMouseY(),
				m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
		else if (m_mode == MouseMode::RemoveSprite)
			RemoveSprite(m_pMouse->GetStartMouseX(), m_pMouse->GetStartMouseY(),
				m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
		else if (m_mode == MouseMode::AddClip)
		{
			CAnimationClip::GetInst()->AddClip(m_camera, m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
			InvalidateRgn(CApp::GetInst()->GetAnimWnd()->GetHwnd(), NULL, false);
		}

		InvalidateRgn(m_hWnd, NULL, false);
		break;
	}
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

void CSpriteWnd::Render()
{
	if (!m_pRenderTarget) return;
	if (!m_pMouse) return;
	//if (!m_pImage) return;

	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	CBitmap::GetInst()->Render(m_pRenderTarget, m_camera);

	if(m_mode == MouseMode::DragSlice || m_mode == MouseMode::RemoveSprite)
		m_pMouse->Render(m_pRenderTarget, m_pBlackBrush);

	for(int i=0; i< CAnimationClip::GetInst()->GetVecSpriteSize(); i++)
	{
		CSprite * sprite = CAnimationClip::GetInst()->GetVecSprite(i);
		D2D1_RECT_F rect = sprite->GetSize();
		rect.left += m_camera->GetXPos();
		rect.right += m_camera->GetXPos();
		rect.top += m_camera->GetYPos();
		rect.bottom += m_camera->GetYPos();
		m_pRenderTarget->DrawRectangle(rect, m_pBlackBrush);
	}

	for (int i = 0; i < CAnimationClip::GetInst()->GetVecClipSize(); i++)
	{
		CSprite* sprite = CAnimationClip::GetInst()->GetVecClip(i);
		D2D1_RECT_F rect = sprite->GetSize();
		rect.left += m_camera->GetXPos();
		rect.right += m_camera->GetXPos();
		rect.top += m_camera->GetYPos();
		rect.bottom += m_camera->GetYPos();
		m_pRenderTarget->DrawRectangle(rect, m_pRedBrush);
	}

	std::wstring wstr = CBitmap::GetInst()->GetPixelColorString(m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
	const WCHAR* strColor = wstr.c_str();
	m_pRenderTarget->DrawTextW(strColor, wstr.length(), m_pDWTextFormat, D2D1::RectF(100, 500, 250, 550), m_pBlackBrush);

	m_pRenderTarget->EndDraw();
}