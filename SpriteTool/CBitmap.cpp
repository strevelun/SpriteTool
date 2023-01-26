#include "CBitmap.h"
#include "CCore.h"
#include "CSprite.h"

#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <stack>
#include <queue>
#include <unordered_map>

CBitmap* CBitmap::m_inst = nullptr;

CBitmap::CBitmap()
{
}

CBitmap::~CBitmap()
{
}

void CBitmap::Find(std::vector<std::vector<bool>>& _visited, int _curX, int _curY)
{
	int curX = _curX, curY = _curY;
	int searchX, searchY;
	int dir[8][2] = { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1} }; // {x, y}

	std::queue<std::pair<int, int>> q;

	q.push(std::pair<int, int>(curY, curX));
	_visited[curY][curX] = true;

	int minX = 999999, minY = 999999;
	int maxX = 0, maxY = 0;
	D2D1_RECT_F rect = { 0 };

	while (!q.empty())
	{
		curY = q.front().first;
		curX = q.front().second;
		q.pop();

		for (int i = 0; i < 8; i++)
		{
			int nextX = curX + dir[i][0];
			int nextY = curY + dir[i][1];

			if (nextX < 0 || nextX >= m_size.width || nextY < 0 || nextY >= m_size.height)
				continue;

			if (!_visited[nextY][nextX] && m_bitmapPixel[nextY * (int)m_size.width + nextX] != 4294967295)
			{
				q.push(std::pair<int, int>(nextY, nextX));
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
	AddSprite(sprite);
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
	m_bitmap = bitmap;

	InvalidateRgn(_hWnd, NULL, true);
}

void CBitmap::Render(ID2D1HwndRenderTarget* _pRenderTarget, float _x, float _y)
{
	if (!m_bitmap) return;

	_pRenderTarget->DrawBitmap(m_bitmap, D2D1::RectF(_x, _y, m_bitmap->GetSize().width + _x, m_bitmap->GetSize().height + _y), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(0,0, m_bitmap->GetSize().width, m_bitmap->GetSize().height));
}

void CBitmap::RenderSprite(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x, float _y)
{
	if (!m_bitmap) return;
	if (idx >= m_vecSprite.size()) return;

	_pRenderTarget->DrawBitmap(m_bitmap, D2D1::RectF(_x, _y, m_vecSprite[idx]->GetSize().right - m_vecSprite[idx]->GetSize().left + _x, m_vecSprite[idx]->GetSize().bottom - m_vecSprite[idx]->GetSize().top + _y), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(m_vecSprite[idx]->GetSize().left, m_vecSprite[idx]->GetSize().top, m_vecSprite[idx]->GetSize().right, m_vecSprite[idx]->GetSize().bottom));
}

void CBitmap::RenderClip(ID2D1HwndRenderTarget* _pRenderTarget, unsigned int idx, float _x, float _y, bool _pivot)
{
	if (!m_bitmap) return;
	if (idx >= m_vecClip.size()) return;

	CSprite* sprite = m_vecClip[idx];

	if(!_pivot)
		_pRenderTarget->DrawBitmap(m_bitmap, D2D1::RectF(_x, _y, 
			sprite->GetSize().right - sprite->GetSize().left + _x, 
			sprite->GetSize().bottom - sprite->GetSize().top + _y),
			1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(sprite->GetSize().left, sprite->GetSize().top, sprite->GetSize().right, sprite->GetSize().bottom));
	else
		_pRenderTarget->DrawBitmap(m_bitmap, D2D1::RectF(
			_x - sprite->GetPivotX() * (sprite->GetSize().right - sprite->GetSize().left),
			_y - sprite->GetPivotY() * (sprite->GetSize().bottom - sprite->GetSize().top),
			sprite->GetSize().right - sprite->GetSize().left + _x - sprite->GetPivotX() * (sprite->GetSize().right - sprite->GetSize().left),
			sprite->GetSize().bottom - sprite->GetSize().top + _y - sprite->GetPivotY() * (sprite->GetSize().bottom - sprite->GetSize().top)),
			1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(sprite->GetSize().left, sprite->GetSize().top, sprite->GetSize().right, sprite->GetSize().bottom));
}

void CBitmap::AddSprite(CSprite* _sprite)
{
	m_vecSprite.push_back(_sprite);
}

void CBitmap::ClearVecSpriteAndClip()
{
	int size = m_vecSprite.size();

	if (size <= 0) return;

	for (int i = 0; i < size; i++)
		delete m_vecSprite[i];
	m_vecSprite.clear();
	m_vecClip.clear();
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

void CBitmap::AutoSliceSprite()
{
	if (!m_bitmap) return;

	std::vector<std::vector<bool>> visited(m_size.height, std::vector<bool>(m_size.width, false));

	for (int i = 0; i < m_size.height; i++)
	{
		for (int j = 0; j < m_size.width; j++)
		{
			if (!visited[i][j] && m_bitmapPixel[i * (int)m_size.width + j] != 4294967295)
			{
				Find(visited, j, i);
			}
		}
	}


}

void CBitmap::DragSprite(int _startPosX, int _startPosY, int _endPosX, int _endPosY)
{
	if (_endPosX >= m_size.width || _endPosY >= m_size.height) return;
	if (_startPosX >= m_size.width || _startPosY >= m_size.height) return;

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

			if (m_bitmapPixel[i * (int)m_size.width + j] != 4294967295)
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
	AddSprite(sprite);
}

void CBitmap::RemoveSprite(int _startPosX, int _startPosY, int _endPosX, int _endPosY)
{
	if (_endPosX >= m_size.width || _endPosY >= m_size.height) return;
	if (_startPosX >= m_size.width || _startPosY >= m_size.height) return;

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
	
	for (int i = 0; i < m_vecSprite.size(); i++)
	{
		D2D1_RECT_F r = m_vecSprite[i]->GetSize();
		if(_startPosX <= r.left && r.right <= _endPosX && _startPosY <= r.top && _endPosY >= r.bottom)
		{
			delete m_vecSprite[i];
			m_vecSprite.erase(m_vecSprite.begin() + i);
			i--; // 벡터 원소 지우면서 크기 변화때문에 스킵하는 원소 발생 방지
		}
	}
}

void CBitmap::AddClip(int _xpos, int _ypos)
{
	for (int i = 0; i < m_vecSprite.size(); i++)
	{
		CSprite* sprite = m_vecSprite[i]; // AnimWnd에 있는 클립들의 Rect는 다름
		D2D1_RECT_F rect = sprite->GetSize();
		if (rect.left <= _xpos && _xpos <= rect.right && rect.top <= _ypos && rect.bottom >= _ypos)
		{
			m_vecClip.push_back(sprite);
			break;
		}

	}
}

CSprite* CBitmap::GetClipInPos(int _xpos, int _ypos, D2D1_RECT_F& _r)
{
	size_t size = m_vecClip.size();
	if (size <= 0) return {};

	CSprite* sprite = nullptr;
	int totalWidth = 0;

	for (int i = 0; i < size; i++)
	{
		sprite = m_vecClip[i];
		D2D1_RECT_F rect = sprite->GetSize();
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		// top은 항상 0이고 left는 totalWidth

		if (totalWidth <= _xpos && totalWidth + width >= _xpos && 0 <= _ypos && height >= _ypos)
		{
			_r.left = totalWidth;
			_r.right = totalWidth + width;
			_r.top = 0;
			_r.bottom = height;
			break;
		}
		totalWidth += width;
	}

	return sprite;
}
