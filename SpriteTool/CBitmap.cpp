#include "CBitmap.h"
#include "CCore.h"
#include "CSprite.h"

#include <Windows.h>
#include <stack>


void CBitmap::OpenFile(HWND _hWnd, ID2D1HwndRenderTarget* _pRenderTarget)
{
	OPENFILENAME OFN;
	TCHAR filePathName[100] = L"";
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"이미지 파일\0*.png";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = _hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 100;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) == 0)
		return;

	std::stack<char> s;
	int i = 0;

	for (int i = wcslen(OFN.lpstrFile); i >= 0; i--)
	{
		if (OFN.lpstrFile[i] == '\\')
		{
			break;
		}
		char a = OFN.lpstrFile[i];
		s.push(a);
	}

	i = 0;
	TCHAR fileName[100] = L"";
	while (s.top() != '\0')
	{
		fileName[i] = s.top();
		s.pop();
		i++;
	}

	CCore::GetInst()->LoadBitmapFromFile(fileName, _pRenderTarget, &m_bitmap);



	InvalidateRgn(_hWnd, NULL, true);
}

void CBitmap::Render(ID2D1HwndRenderTarget* _pRenderTarget, float _x, float _y)
{
	if (!m_bitmap)
		return;

	_pRenderTarget->DrawBitmap(m_bitmap, D2D1::RectF(0, 0, m_bitmap->GetSize().width, m_bitmap->GetSize().height), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(_x, _y, m_bitmap->GetSize().width, m_bitmap->GetSize().height));

}
