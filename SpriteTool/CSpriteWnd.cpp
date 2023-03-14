#include "CSpriteWnd.h"

#include "CApp.h"
#include "CCore.h"
#include "resource.h"
#include "CAnimWnd.h"
#include "CSprite.h"
#include "CAnimationClip.h"
#include "ToolManager.h"

#include <windowsx.h>
#include <string>
#include <vector>
#include <windows.h>

#pragma comment( lib, "d2d1.lib " ) 
#pragma comment(lib, "winmm.lib") 

int s_gridX, s_gridY;

INT_PTR CALLBACK GridDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK GridXDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void CSpriteWnd::Find(std::vector<std::vector<int>>& _visited, int _curX, int _curY)
{
	int curX = _curX, curY = _curY;
	int dir[4][2] = { {0,-1}, {1,0}, {0,1},{-1,0} };
	std::vector<std::pair<int, int>> v;

	v.push_back(std::pair<int, int>(curY, curX));
	_visited[curY][curX] = 1;

	int minX = 999999, minY = 999999;
	int maxX = 0, maxY = 0;
	D2D1_RECT_F rect = { 0 };

	D2D1_SIZE_F size = ToolManager::GetInst()->GetBitmap()->GetBitmapSize();
	DWORD* bitmapPixel = ToolManager::GetInst()->GetBitmap()->GetBitmapPixel();

	while (!v.empty())
	{
		std::pair<int, int> p = v.back();
		curY = p.first;
		curX = p.second;
		v.pop_back();
		
		for (int i = 0; i < 4; i++)
		{
			int nextX = curX + dir[i][0];
			int nextY = curY + dir[i][1];

			if (nextX < 0 || nextX >= size.width || nextY < 0 || nextY >= size.height)
				continue;

			if (_visited[nextY][nextX]) continue;

			if (bitmapPixel[nextY * (int)size.width + nextX] & 0xff000000)  // 0xffff00000
			{
				_visited[nextY][nextX] = 1;

				v.push_back({nextY, nextX});
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
	ToolManager::GetInst()->GetAnimClip()->AddSprite(sprite);
	m_count++;
}

void CSpriteWnd::AutoSliceSprite()
{
	ID2D1Bitmap* bitmap = ToolManager::GetInst()->GetBitmap()->GetBitmap();

	if (!bitmap) return;

	DWORD* bitmapPixel = ToolManager::GetInst()->GetBitmap()->GetBitmapPixel();
	D2D1_SIZE_F size = ToolManager::GetInst()->GetBitmap()->GetBitmapSize();

	std::vector<std::vector<int>> visited(size.height, std::vector<int>(size.width, 0));

	DWORD first = timeGetTime();

	for (int i = 0; i < size.height; i++)
	{
		for (int j = 0; j < size.width; j++)
		{
			if (visited[i][j]) continue;

			if ( bitmapPixel[i * (int)size.width + j] & 0xff000000)  // 0xffff00000
			{
				Find(visited, j, i); 
			}
		}
	}
	DWORD second = timeGetTime();
#ifdef _DEBUG
	char str[100];
	sprintf_s(str, "%d개 - %d밀리초\n", m_count, second - first);
	OutputDebugStringA(str);
#endif
}

void CSpriteWnd::DragSprite(CCamera* _camera, int _startPosX, int _startPosY, int _endPosX, int _endPosY)
{
	D2D1_SIZE_F size = ToolManager::GetInst()->GetBitmap()->GetBitmapSize();
	DWORD* bitmapPixel = ToolManager::GetInst()->GetBitmap()->GetBitmapPixel();

	_startPosX -= _camera->GetXPos();
	_endPosX -= _camera->GetXPos();
	_startPosY -= _camera->GetYPos();
	_endPosY -= _camera->GetYPos();

	if ((_startPosX < 0 || _startPosY < 0) && (_endPosX < 0 || _endPosY < 0)) return;
	if ((_startPosX >= size.width || _startPosY >= size.height) && (_endPosX >= size.width || _endPosY >= size.height)) return;

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

	if (_endPosY >= size.height) _endPosY = size.height - 1;
	if (_endPosX >= size.width) _endPosX = size.width - 1;

	D2D1_RECT_F rect = { 0 };
	int minX = 999999, minY = 999999;
	int maxX = 0, maxY = 0;

	for (int i = _startPosY + 1; i < _endPosY - 1; i++)
	{
		for (int j = _startPosX + 1; j < _endPosX - 1; j++)
		{
			if (bitmapPixel[i * (int)size.width + j] & 0xff000000)
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
	ToolManager::GetInst()->GetAnimClip()->AddSprite(sprite);
}

void CSpriteWnd::RemoveSprite(CCamera* _camera, int _startPosX, int _startPosY, int _endPosX, int _endPosY, int& _addClips)
{
	D2D1_SIZE_F size = ToolManager::GetInst()->GetBitmap()->GetBitmapSize();

	_startPosX -= _camera->GetXPos();
	_endPosX -= _camera->GetXPos();
	_startPosY -= _camera->GetYPos();
	_endPosY -= _camera->GetYPos();

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

	for (int i = 0; i < ToolManager::GetInst()->GetAnimClip()->GetVecSpriteSize(); i++)
	{
		CSprite* sprite = ToolManager::GetInst()->GetAnimClip()->GetVecSprite(i);
		D2D1_RECT_F r = sprite->GetRect();
		if (_startPosX <= r.left && r.right <= _endPosX && _startPosY <= r.top && _endPosY >= r.bottom)
		{
			for (int j = 0; j < ToolManager::GetInst()->GetAnimClip()->GetVecClipSize(); j++)
			{
				CSprite* sprite2 = ToolManager::GetInst()->GetAnimClip()->GetVecClip(j);
				D2D1_RECT_F r2 = sprite2->GetRect();
				if (r.left == r2.left && r.top == r2.top && r.right == r2.right && r.bottom == r2.bottom)
				{
					ToolManager::GetInst()->GetAnimClip()->EraseClip(j);
					_addClips++;
					break;
				}
			}

			delete sprite;
			ToolManager::GetInst()->GetAnimClip()->EraseClip(i);
			ToolManager::GetInst()->GetAnimClip()->EraseSprite(i);
			i--; // 벡터 원소 지우면서 크기 변화때문에 스킵하는 원소 발생 방지
		}
	}
}

void CSpriteWnd::GridSlice()
{
	if (s_gridX <= 0 || s_gridY <= 0)
		return;

	D2D1_SIZE_F size = ToolManager::GetInst()->GetBitmap()->GetBitmapSize();
	int tileWidth = size.width / s_gridX;
	int tileHeight = size.height / s_gridY;
	int left = 0, top = 0, right = tileWidth, bottom = tileHeight;
	D2D1_RECT_F rect;

	for (int i = 0; i < size.height / tileHeight; i++)
	{
		for (int j = 0; j < size.width / tileWidth; j++)
		{
			rect.left =  left;
			rect.top =  top;
			rect.right =  right;
			rect.bottom =  bottom;
			CSprite *sprite = new CSprite(rect);
			ToolManager::GetInst()->GetAnimClip()->AddSprite(sprite);

			left += tileWidth;
			right += tileWidth;
		}
		left = 0;
		right = tileWidth;
		top += tileHeight;
		bottom += tileHeight;
	}
}

void CSpriteWnd::AutoXGridSlice(int _gridX)
{
	ID2D1Bitmap* bitmap = ToolManager::GetInst()->GetBitmap()->GetBitmap();

	if (!bitmap) return;

	int gridCount = 0;

	DWORD* bitmapPixel = ToolManager::GetInst()->GetBitmap()->GetBitmapPixel();
	D2D1_SIZE_F size = ToolManager::GetInst()->GetBitmap()->GetBitmapSize();

	std::vector<std::vector<int>> visited(size.height, std::vector<int>(size.width, 0));

	DWORD first = timeGetTime();

	for (int i = 0; i < size.height; i++)
	{
		for (int j = 0; j < size.width; j++)
		{
			if (visited[i][j]) continue;

			if (bitmapPixel[i * (int)size.width + j] & 0xff000000)  // 0xffff00000
			{
				int curGrid = j / (ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width / _gridX);
				int curX = j;
				int curY = i;
				int dir[4][2] = { {0,-1}, {1,0}, {0,1},{-1,0} };
				std::vector<std::pair<int, int>> v;

				v.push_back(std::pair<int, int>(curY, curX));
				visited[curY][curX] = 1;

				int minX = 999999, minY = 999999;
				int maxX = 0, maxY = 0;
				D2D1_RECT_F rect = { 0 };

				D2D1_SIZE_F size = ToolManager::GetInst()->GetBitmap()->GetBitmapSize();
				DWORD* bitmapPixel = ToolManager::GetInst()->GetBitmap()->GetBitmapPixel();

				while (!v.empty())
				{
					std::pair<int, int> p = v.back();
					curY = p.first;
					curX = p.second;
					v.pop_back();

					for (int i = 0; i < 4; i++)
					{
						int nextX = curX + dir[i][0];
						int nextY = curY + dir[i][1];

						if (nextX < 0 || nextX >= size.width || nextY < 0 || nextY >= size.height)
							continue;

						if (visited[nextY][nextX]) continue;
						
						int temp = ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width / _gridX;
						
						if (curGrid * temp <= nextX && nextX <= curGrid * temp + temp
							&& bitmapPixel[nextY * (int)size.width + nextX] & 0xff000000)  // 0xffff00000
						{
							visited[nextY][nextX] = 1;

							v.push_back({ nextY, nextX });
							//if (maxX - minX < ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width / _gridX)
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
					CSprite* sprite = new CSprite(rect);
					ToolManager::GetInst()->GetAnimClip()->AddSprite(sprite);
				}

			}

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
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, IDR_MENU1) == false)
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
			ToolManager::GetInst()->GetAnimClip()->ClearVecSpriteAndClip();
			m_addClips = 0;
			ToolManager::GetInst()->GetBitmap()->OpenImageFile(hWnd, m_pRenderTarget);
			InvalidateRgn(CApp::GetInst()->GetAnimWnd()->GetHwnd(), NULL, false);
			break;

		case ID_SAVE_IMAGE:
			ToolManager::GetInst()->GetBitmap()->SaveImageFile(hWnd);
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

		case ID_GRID_SLICE:
			DialogBox(m_hInst,                   // application instance
				MAKEINTRESOURCE(IDD_DIALOG1), // dialog box resource
				hWnd,                          // owner window
				GridDialogProc // dialog box window procedure
			);
			break;

		case ID_GRID_X:
			DialogBox(m_hInst,                   // application instance
				MAKEINTRESOURCE(IDD_DIALOG1), // dialog box resource
				hWnd,                          // owner window
				GridXDialogProc // dialog box window procedure
			);
			break;

		case ID_KEY_COLOR:
		{
			ToolManager::GetInst()->GetBitmap()->KeyColor(m_keyColor);
			D2D1_BITMAP_PROPERTIES bpp;
			bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
			bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
			bpp.dpiX = (FLOAT)0;
			bpp.dpiY = (FLOAT)0;

			ToolManager::GetInst()->GetBitmap()->GetBitmap()->Release();
			ID2D1Bitmap* bitmap;
			m_pRenderTarget->CreateBitmap(D2D1::SizeU(ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width,
				ToolManager::GetInst()->GetBitmap()->GetBitmapSize().height),
				ToolManager::GetInst()->GetBitmap()->GetBitmapPixel(),
				ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width * 4, &bpp, &bitmap);
			ToolManager::GetInst()->GetBitmap()->SetBitmap(bitmap);
			InvalidateRgn(hWnd, NULL, false);
			break;
		}
		case ID_SPRITE:
			m_mode = MouseMode::RemoveSprite;

			break;
		case ID_ALL_BOXES:
			ToolManager::GetInst()->GetAnimClip()->ClearVecSpriteAndClip();
			InvalidateRgn(m_hWnd, NULL, false);
			InvalidateRgn(CApp::GetInst()->GetAnimWnd()->GetHwnd(), NULL, false);
			m_addClips = 0;
			break;
		}
		break;
	}

	case WM_SIZE:
	{
		int w = LOWORD(lParam);
		int h = HIWORD(lParam);
		D2D1_SIZE_U size;
		size.width = w;
		size.height = h;
		m_pRenderTarget->Resize(size);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			if (m_camera->GetXPos() < ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width)
				m_camera->UpdateXPos(15);
			break;
		case VK_RIGHT:
			//if (m_camera->GetXPos() > 0)
				m_camera->UpdateXPos(-15);
			break;
		case VK_UP:
				m_camera->UpdateYPos(15);
			break;
		case VK_DOWN:
			//if(m_camera->GetYPos() > 0)
				m_camera->UpdateYPos(-15);
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
		//Input.OnMouseMove(xPos, yPos);

		int xpos = GET_X_LPARAM(lParam);
		int ypos = GET_Y_LPARAM(lParam);
		m_pMouse->UpdateMousePos(xpos, ypos);
		ToolManager::GetInst()->GetBitmap()->GetPixelColorString(xpos, ypos);

		InvalidateRgn(m_hWnd, NULL, false);
		break;
	}
	case WM_LBUTTONDOWN: 
	{
		int xpos = GET_X_LPARAM(lParam);
		int ypos = GET_Y_LPARAM(lParam);

		m_pMouse->UpdateMouseStartPos(xpos, ypos);
		m_pMouse->UpdateClickState(true);

		DWORD* pixel = ToolManager::GetInst()->GetBitmap()->GetBitmapPixel();
		if (!pixel)
			break;
		if (xpos >= ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width) break;
		if (ypos >= ToolManager::GetInst()->GetBitmap()->GetBitmapSize().height) break;

		m_keyColor = pixel[ypos * (int)ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width + xpos];

		break;
	}
	case WM_LBUTTONUP: 
	{
		// LBUTTONDOWN 한 적이 없으면 break. 이미지 열기할 때 들어옴
		if (m_pMouse->GetClickState() == false) break;

		m_pMouse->UpdateClickState(false);

		if (m_mode == MouseMode::DragSlice)
		{
			DragSprite(m_camera, m_pMouse->GetStartMouseX(), m_pMouse->GetStartMouseY(),
				m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
		}
		else if (m_mode == MouseMode::RemoveSprite)
		{
			RemoveSprite(m_camera, m_pMouse->GetStartMouseX(), m_pMouse->GetStartMouseY(),
				m_pMouse->GetMouseX(), m_pMouse->GetMouseY(), m_addClips);
			InvalidateRgn(CApp::GetInst()->GetAnimWnd()->GetHwnd(), NULL, false);
		}
		else if (m_mode == MouseMode::AddClip)
		{
			ToolManager::GetInst()->GetAnimClip()->AddClip(m_pRenderTarget, m_camera, m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
			InvalidateRgn(CApp::GetInst()->GetAnimWnd()->GetHwnd(), NULL, false);
			m_addClips++;
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

	ToolManager::GetInst()->GetBitmap()->Render(m_pRenderTarget, m_camera);

	if(m_mode == MouseMode::DragSlice || m_mode == MouseMode::RemoveSprite)
		m_pMouse->Render(m_pRenderTarget, m_pBlackBrush);

	for(int i=0; i< ToolManager::GetInst()->GetAnimClip()->GetVecSpriteSize(); i++)
	{
		CSprite * sprite = ToolManager::GetInst()->GetAnimClip()->GetVecSprite(i);
		D2D1_RECT_F rect = sprite->GetRect();
		rect.left += m_camera->GetXPos();
		rect.right += m_camera->GetXPos();
		rect.top += m_camera->GetYPos();
		rect.bottom += m_camera->GetYPos();
		m_pRenderTarget->DrawRectangle(rect, m_pBlackBrush);
	}

	for (int i = 0; i < m_addClips-1; i++)
	{
		CSprite* sprite = ToolManager::GetInst()->GetAnimClip()->GetVecClip(i);
		if (sprite == nullptr) continue;
		D2D1_RECT_F rect = sprite->GetRect();
		rect.left += m_camera->GetXPos();
		rect.right += m_camera->GetXPos();
		rect.top += m_camera->GetYPos();
		rect.bottom += m_camera->GetYPos();
		m_pRenderTarget->DrawRectangle(rect, m_pRedBrush);
	}

	std::wstring wstr = ToolManager::GetInst()->GetBitmap()->GetPixelColorString(m_pMouse->GetMouseX(), m_pMouse->GetMouseY());
	const WCHAR* strColor = wstr.c_str();
	m_pRenderTarget->DrawTextW(strColor, wstr.length(), m_pDWTextFormat, D2D1::RectF(100, 500, 250, 550), m_pBlackBrush);

	m_pRenderTarget->EndDraw();
}

INT_PTR CALLBACK GridDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR gridX[5], gridY[5];
	WORD gridXLength = 0, gridYLength = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		// Set the default push button to "Cancel." 
		SendMessage(hDlg,
			DM_SETDEFID,
			(WPARAM)IDCANCEL,
			(LPARAM)0);

		return TRUE;

	case WM_COMMAND:
		// Set the default push button to "OK" when the user enters text. 
		if (HIWORD(wParam) == EN_CHANGE )//&&
			//LOWORD(wParam) == IDE_PASSWORDEDIT)
		{
			SendMessage(hDlg,
				DM_SETDEFID,
				(WPARAM)IDOK,
				(LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK:
		{
			// Get number of characters. 
			gridXLength = (WORD)SendDlgItemMessage(hDlg,
				IDC_EDIT,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);

			gridYLength = (WORD)SendDlgItemMessage(hDlg,
				IDC_EDIT2,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);

			if (gridXLength >= 5 || gridYLength >= 5)
			{
				MessageBox(hDlg,
					L"Too many characters.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}
			else if (gridXLength == 0 || gridYLength == 0)
			{
				MessageBox(hDlg,
					L"No characters entered.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}

			// Put the number of characters into first word of buffer. 
			*((LPWORD)gridX) = gridXLength;
			*((LPWORD)gridY) = gridYLength;

			// Get the characters. 
			SendDlgItemMessage(hDlg,
				IDC_EDIT,
				EM_GETLINE,
				(WPARAM)0,       // line 0 
				(LPARAM)gridX);

			SendDlgItemMessage(hDlg,
				IDC_EDIT2,
				EM_GETLINE,
				(WPARAM)0,       // line 0 
				(LPARAM)gridY);

			// Null-terminate the string. 
			gridX[gridXLength] = 0;
			gridY[gridYLength] = 0;


			char input[5];
			WideCharToMultiByte(CP_ACP, 0, gridX, 5, input, 5, NULL, NULL);
			int n = atoi(input);
			s_gridX = n;
			WideCharToMultiByte(CP_ACP, 0, gridY, 5, input, 5, NULL, NULL);
			n = atoi(input);
			s_gridY = n;
			CApp::GetInst()->GetSpriteWnd()->GridSlice();

			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		return 0;
	}
	return FALSE;

	UNREFERENCED_PARAMETER(lParam);
}

INT_PTR CALLBACK GridXDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR gridX[5];
	WORD gridXLength = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		// Set the default push button to "Cancel." 
		SendMessage(hDlg,
			DM_SETDEFID,
			(WPARAM)IDCANCEL,
			(LPARAM)0);

		return TRUE;

	case WM_COMMAND:
		// Set the default push button to "OK" when the user enters text. 
		if (HIWORD(wParam) == EN_CHANGE)//&&
			//LOWORD(wParam) == IDE_PASSWORDEDIT)
		{
			SendMessage(hDlg,
				DM_SETDEFID,
				(WPARAM)IDOK,
				(LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK:
		{
			// Get number of characters. 
			gridXLength = (WORD)SendDlgItemMessage(hDlg,
				IDC_EDIT,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);

			if (gridXLength >= 5)
			{
				MessageBox(hDlg,
					L"Too many characters.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}
			else if (gridXLength == 0)
			{
				MessageBox(hDlg,
					L"No characters entered.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}

			// Put the number of characters into first word of buffer. 
			*((LPWORD)gridX) = gridXLength;

			// Get the characters. 
			SendDlgItemMessage(hDlg,
				IDC_EDIT,
				EM_GETLINE,
				(WPARAM)0,       // line 0 
				(LPARAM)gridX);

			// Null-terminate the string. 
			gridX[gridXLength] = 0;


			char input[5];
			WideCharToMultiByte(CP_ACP, 0, gridX, 5, input, 5, NULL, NULL);
			int n = atoi(input);
			s_gridX = n;
			CApp::GetInst()->GetSpriteWnd()->AutoXGridSlice(n);

			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		return 0;
	}
	return FALSE;

	UNREFERENCED_PARAMETER(lParam);
}