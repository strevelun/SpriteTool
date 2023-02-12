#include "CBitmap.h"
#include "CCore.h"
#include "CSprite.h"
#include "CAnimationClip.h"
#include "CCamera.h"
#include "CAnimWnd.h"

#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <stack>
#include <unordered_map>
#include <string>

CBitmap* CBitmap::m_inst = nullptr;

CBitmap::CBitmap()
{
}

CBitmap::~CBitmap()
{
}

void CBitmap::OpenImageFile(HWND _hWnd, ID2D1HwndRenderTarget* _pRenderTarget)
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
	if( m_bitmap ) m_bitmap->Release();
	m_bitmap = bitmap;

	InvalidateRgn(_hWnd, NULL, true);
}

void CBitmap::SaveImageFile(HWND _hWnd)
{
	
	if (!m_bitmap)
		return;

	OPENFILENAME ofn;
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"png\0*.png";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = _hWnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrInitialDir = L".";

	if (GetSaveFileName(&ofn) == 0) return;

	std::wstring strFilePath = lpstrFile;
	strFilePath.append(L".");
	strFilePath.append(ofn.lpstrFilter);

	FILE* pFile = nullptr;

	errno_t errNum = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	if (pFile == nullptr || errNum != 0)
		return;

	fclose(pFile);
	

	CCore::GetInst()->SaveBitmaptoFile(strFilePath.c_str(), m_bitmap);
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

void CBitmap::SaveClip(HWND _hWnd, Type _type)
{
	if (!m_bitmap)	return;
	if (CAnimationClip::GetInst()->GetVecClipSize() <= 0) return;

	OPENFILENAME ofn;
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"spr\0*.spr";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = _hWnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrInitialDir = L".";

	if (GetSaveFileName(&ofn) == 0) return;

	std::wstring strFilePath = lpstrFile;
	strFilePath.append(L".");
	strFilePath.append(ofn.lpstrFilter);

	FILE* pFile = nullptr;

	errno_t errNum = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	if (pFile == nullptr || errNum != 0)
		return;

	int size = CAnimationClip::GetInst()->GetVecClipSize();
	fwrite(&size, sizeof(int), 1, pFile);
	fwrite(&_type, sizeof(Type), 1, pFile);

	for (int i = 0; i < size; i++)
	{
		CSprite* sprite = CAnimationClip::GetInst()->GetVecClip(i);
		fwrite(sprite, sizeof(CSprite), 1, pFile);
		int width = sprite->GetSize().right - sprite->GetSize().left;
		int height = sprite->GetSize().bottom - sprite->GetSize().top;
		fwrite(&width, sizeof(int), 1, pFile);
		fwrite(&height, sizeof(int), 1, pFile);
		//for(int i=0; i<height; i++)
		//	fwrite(&sprite->GetPixel()[i * width], sizeof(DWORD) * width, 1, pFile);
		fwrite(&sprite->GetPixel()[0], sizeof(DWORD) * width , height, pFile);
	}

	fclose(pFile);
}

void CBitmap::LoadClip(HWND _hWnd, ID2D1HwndRenderTarget* _pRenderTarget)
{
	// 이미 클립이 있다면 기존 클립 전부 지우기

	int clipSize = CAnimationClip::GetInst()->GetVecClipSize();

	if (clipSize > 0)
	{
		for(int i=0; i<clipSize; i++)
			CAnimationClip::GetInst()->EraseClip(i);
	}

	OPENFILENAME ofn;
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"spr\0*.spr";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = _hWnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 100;
	ofn.lpstrInitialDir = L".";

	if (GetOpenFileName(&ofn) == 0) return;

	std::stack<char> s;
	int i = 0;

	for (int i = wcslen(ofn.lpstrFile); i >= 0; i--)
	{
		if (ofn.lpstrFile[i] == '\\')
			break;
		char a = ofn.lpstrFile[i];
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

	FILE* pFile = nullptr;
	errno_t errNum = _wfopen_s(&pFile, fileName, L"rb");

	if (pFile == nullptr || errNum != 0)
		return;

	fread(&clipSize, sizeof(int), 1, pFile);
	
	int width, height;

	for (int i = 0; i < clipSize; i++)
	{
		CSprite* sprite = new CSprite;
		fread(sprite, sizeof(CSprite), 1, pFile);
		fread(&width, sizeof(int), 1, pFile);
		fread(&height, sizeof(int), 1, pFile);
		DWORD* pixel = (DWORD*)malloc( sizeof(DWORD) * width * height);
	
		//for(int i=0; i<height; i++)
		//	fread(pixel + (i * width), sizeof(DWORD) * width, 1, pFile);
		fread(pixel, sizeof(DWORD)  * width, height, pFile);
	
		D2D1_BITMAP_PROPERTIES bpp;
		bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		bpp.dpiX = (FLOAT)0;
		bpp.dpiY = (FLOAT)0;
		ID2D1Bitmap* bitmap;
		sprite->SetPixel(pixel);
		_pRenderTarget->CreateBitmap(D2D1::SizeU(width, height), pixel, width * 4, &bpp, &bitmap);
		sprite->SetBitmap(bitmap);
		CAnimationClip::GetInst()->AddClip(sprite);
	}

	fclose(pFile);
}
