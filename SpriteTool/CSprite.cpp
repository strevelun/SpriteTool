#include "CSprite.h"
#include "CBitmap.h"

CSprite::CSprite()
{
}

CSprite::CSprite(const CSprite& sprite)
{
	m_pivotX = sprite.m_pivotX;
	m_pivotY = sprite.m_pivotY;
	m_rect = sprite.m_rect;
	m_pixel = sprite.m_pixel;
	m_bitmap = sprite.m_bitmap;
}

CSprite::CSprite(D2D1_RECT_F _rect)
{
	m_rect = _rect;
}

CSprite::~CSprite()
{
}

void CSprite::CopyPixel(ID2D1HwndRenderTarget* _pRenderTarget)
{
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;
	m_pixel = (DWORD*)malloc(sizeof(DWORD) * width * height);
	//memset(m_pixel, 0, sizeof(DWORD) * width * height);

	DWORD* pixel = CBitmap::GetInst()->GetBitmapPixel();

	D2D1_SIZE_F bitmapSize = CBitmap::GetInst()->GetBitmapSize();
	
	int offset = 0;
	int j = m_rect.left;

	for (int i = m_rect.top; i < m_rect.bottom; i++)
	{
		memcpy(m_pixel + offset, pixel + (DWORD)(i * bitmapSize.width + j), width * sizeof(DWORD));
		offset += width;
	}
	D2D1_BITMAP_PROPERTIES bpp;
	bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bpp.dpiX = (FLOAT)0;
	bpp.dpiY = (FLOAT)0;

	_pRenderTarget->CreateBitmap(D2D1::SizeU(width, height), m_pixel, width * 4, &bpp, &m_bitmap);
}
