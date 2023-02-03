#include "CBitmap.h"
#include "CCore.h"
#include "CSprite.h"
#include "CAnimationClip.h"
#include "CCamera.h"

#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <stack>
#include <unordered_map>

CBitmap* CBitmap::m_inst = nullptr;

CBitmap::CBitmap()
{
}

CBitmap::~CBitmap()
{
}

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

	if (GetOpenFileName(&OFN) == 0) return;

	std::stack<char> s;
	int i = 0;

	for (int i = wcslen(OFN.lpstrFile); i >= 0; i--)
	{
		if (OFN.lpstrFile[i] == '\\')
			break;
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

	ID2D1Bitmap* bitmap;
	m_bitmapPixel = CCore::GetInst()->LoadBitmapFromFile(fileName, _pRenderTarget, &bitmap, &m_size);
	//delete m_bitmap;
	m_bitmap = bitmap;

	InvalidateRgn(_hWnd, NULL, true);
}

void CBitmap::Render(ID2D1HwndRenderTarget* _pRenderTarget, CCamera* _camera, float _x, float _y)
{
	if (!m_bitmap) return;
	if (!_camera) return;

	_pRenderTarget->DrawBitmap(m_bitmap, D2D1::RectF(_x + _camera->GetXPos(), _y + _camera->GetYPos(), m_bitmap->GetSize().width + _x + _camera->GetXPos(), m_bitmap->GetSize().height + _y + _camera->GetYPos()), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(0,0, m_bitmap->GetSize().width, m_bitmap->GetSize().height));
}

std::wstring CBitmap::GetPixelColorString(unsigned int _xpos, unsigned int _ypos)
{
	if(m_bitmapPixel == nullptr)	return L"";
	if (_xpos >= m_size.width || _ypos >= m_size.height) return L"";

	std::wstringstream stream;
	int a = 0;
	DWORD d = m_bitmapPixel[(_ypos * (int)m_size.width) + _xpos];
	stream << std::hex << d;
	std::wstring result(stream.str());

	return result;
}

void CBitmap::KeyColor(DWORD _keyColor)
{
	for (int i = 0; i < m_size.height; i++)
	{
		for (int j = 0; j < m_size.width; j++)
		{
			if (m_bitmapPixel[(i * (int)m_size.width) + j] == _keyColor)
			{
				m_bitmapPixel[(i * (int)m_size.width) + j] &= 0x00000000;
			}
		}
	}
}
