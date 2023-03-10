#include "CAnimWnd.h"

#include "CApp.h"
#include "CCore.h"
#include "CBitmap.h"
#include "CSprite.h"
#include "resource.h"
#include "CAnimViewWnd.h"
#include "CAnimationClip.h"
#include "CCamera.h"
#include "ToolManager.h"

#include <windowsx.h>

Type type = Type::Tile;

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int id, event;
	DWORD state;

	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hwndDlg, IDC_TYPE_TILE), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(GetDlgItem(hwndDlg, IDC_TYPE_BLOCK), BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(GetDlgItem(hwndDlg, IDC_TYPE_CHARACTER), BM_SETCHECK, BST_UNCHECKED, 0);
		
		return TRUE;

	case WM_COMMAND:
		id = LOWORD(wParam);
		event = HIWORD(wParam);

		switch (id)
		{
		case IDC_TYPE_TILE:
			if (event == BN_CLICKED) {
				type = Type::Tile;
			}
			break;

		case IDC_TYPE_BLOCK:
			if (event == BN_CLICKED) {
				type = Type::Block;
			}
			break;
			
		case IDC_TYPE_CHARACTER:
			if (event == BN_CLICKED) {
				type = Type::Character;
			}
			break;

		case IDOK:
			ToolManager::GetInst()->GetBitmap()->SaveClip(hwndDlg, type);
			EndDialog(hwndDlg, TRUE);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

CAnimWnd::CAnimWnd(HINSTANCE hInstance)
{
	m_hInst = hInstance;
}

CAnimWnd::~CAnimWnd()
{
	if (m_pRenderTarget) m_pRenderTarget->Release();
	delete m_camera;
}

bool CAnimWnd::Create(int _width, int _height, int nCmdShow)
{
	if (CBWnd::Create(L"D2DTutWindowClassAnim", _width, _height, nCmdShow,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, IDR_MENU2) == false)
		return false;

	m_camera = new CCamera();
	InvalidateRgn(m_hWnd, NULL, true);

	return true;
}

LRESULT CAnimWnd::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HWND hwndRadioButton1, hwndRadioButton2, hwndRadioButton3;


	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_PIVOT:
			break;

		case ID_SAVE_CLIPS:
		{
			type = Type::Tile;
			DialogBox(m_hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)DialogProc);

			break;
		}

		case ID_LOAD_CLIP:
			ToolManager::GetInst()->GetBitmap()->LoadClip(hWnd, m_pRenderTarget);
			break;

		case ID_ANIMATION:
			if(m_viewWnd == nullptr)
				m_viewWnd = new CAnimViewWnd(NULL);
			m_viewWnd->Create(500, 500, SW_SHOWNORMAL);
			break;
		}

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			//if (m_camera->GetXPos() < ToolManager::GetInst()->GetBitmap()->GetBitmapSize().width)
			m_camera->UpdateXPos(15);
			break;
		case VK_RIGHT:
			//if (m_camera->GetXPos() > 0)
			m_camera->UpdateXPos(-15);
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
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int xpos = GET_X_LPARAM(lParam);
		int ypos = GET_Y_LPARAM(lParam);
		xpos -= m_camera->GetXPos();
		D2D1_RECT_F rect = {0};
		CSprite* sprite = ToolManager::GetInst()->GetAnimClip()->GetClipInPos(xpos, ypos, rect, m_camera);
		//rect.left += m_camera->GetXPos();
		//rect.right += m_camera->GetXPos();
		//if (sprite != {})
		
			float pivotX = (xpos - rect.left) / (float)(rect.right - rect.left);
			float pivotY = (ypos - rect.top) / (float)(rect.bottom - rect.top);
			if (pivotY > 1.0f || pivotX > 1.0f)
				break;
			sprite->SetPivotX(pivotX);
			sprite->SetPivotY(pivotY);
		
		InvalidateRect(hWnd, NULL, false);
		
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

void CAnimWnd::Render()
{
	if (!m_pRenderTarget)	return;
	//if (!m_pImage)			return;

	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	//m_pImage->Render(m_pRenderTarget);

	int pos = 0;

	for (int i = 0; i < ToolManager::GetInst()->GetAnimClip()->GetVecClipSize(); i++)
	{
		CSprite* clip = ToolManager::GetInst()->GetAnimClip()->GetVecClip(i);
		ToolManager::GetInst()->GetAnimClip()->RenderClip(m_pRenderTarget, i, pos + m_camera->GetXPos());
		D2D1_RECT_F rect = clip->GetRect();
		rect.left = pos + m_camera->GetXPos();
		rect.bottom -= rect.top;
		rect.top = 0;
		pos += clip->GetRect().right - clip->GetRect().left;
		rect.right = pos + m_camera->GetXPos();
		m_pRenderTarget->DrawRectangle(rect, m_pBlackBrush);

		D2D1_RECT_F dot;
		dot.left = rect.left + (rect.right - rect.left) * clip->GetPivotX();
		dot.top = rect.top + (rect.bottom - rect.top) * clip->GetPivotY();
		dot.right = dot.left + 2;
		dot.bottom = dot.top + 2;
		m_pRenderTarget->DrawRectangle(dot, m_pRedBrush, 2);
	}

	m_pRenderTarget->EndDraw();
}
