#include "CCore.h"
#include "CBitmap.h"

//#include <winrt/Windows.Foundation.Collections.h>
//#include <winrt/Windows.Storage.Pickers.h>

CCore* CCore::m_inst = nullptr;

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")

CCore::CCore()
{
}

CCore::~CCore()
{

}

HRESULT CCore::InitDevice()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	if (FAILED(hr)) return hr;

	CoInitialize(nullptr);
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
	if (FAILED(hr)) return hr;

	return hr;
}


//https://learn.microsoft.com/ko-kr/windows/win32/wic/-wic-bitmapsources-howto-modifypixels?redirectedfrom=MSDN
DWORD* CCore::LoadBitmapFromFile(PCWSTR _wcFileName, ID2D1HwndRenderTarget* _pRenderTarget, ID2D1Bitmap** _pBitmap, D2D1_SIZE_F* _size)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = nullptr;

	hr = m_pWICFactory->CreateDecoderFromFilename(_wcFileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(hr)) return nullptr;

	IWICBitmapFrameDecode* pFrame = nullptr;
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr)) return nullptr;

	IWICFormatConverter* pConverter = nullptr;
	hr = m_pWICFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr)) return nullptr;

	hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(hr)) return nullptr;

	IWICBitmap* pIBitmap = nullptr;
	IWICBitmapLock* pILock = nullptr;

	// CreateBitmapFromMemory 
	hr = m_pWICFactory->CreateBitmapFromSource(pFrame, WICBitmapCacheOnDemand, &pIBitmap);
	UINT width, height;
	pIBitmap->GetSize(&width, &height);
	WICRect rect = { 0, 0, width, height };
	DWORD* pPixel = nullptr;

	_size->width = width;
	_size->height = height;

	_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, _pBitmap);

	hr = pIBitmap->Lock(&rect, WICBitmapLockWrite, &pILock);

	if (SUCCEEDED(hr))
	{
		UINT cbBufferSize = 0;
		BYTE* pv = nullptr;

		if (SUCCEEDED(hr))
		{
			hr = pILock->GetDataPointer(&cbBufferSize, &pv);
			
			pPixel = (DWORD*)pv;
			//pv[0] = 0,0 ÀÇ pixel r, g, b, a 
			// ff ff 00 00
			return pPixel;
		}

		if (pILock) { pILock->Release(); pILock = nullptr; }
	}

	//hr = _pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, _pBitmap);
	//if (FAILED(hr)) return hr;


	if (pConverter) { pConverter->Release(); pConverter = nullptr; }
	if (pFrame) { pFrame->Release(); pFrame = nullptr; }
	if (pDecoder) { pDecoder->Release(); pDecoder = nullptr; }

	return nullptr;
}

void CCore::SaveBitmaptoFile(PCWSTR _fileName, ID2D1Bitmap* _pBitmap)
{
	HRESULT hr = S_OK;

	ID2D1Factory* pD2DFactory = NULL;
	IWICBitmap* pWICBitmap = NULL;
	ID2D1RenderTarget* pRT = NULL;
	IWICBitmapEncoder* pEncoder = NULL;
	IWICBitmapFrameEncode* pFrameEncode = NULL;
	IWICStream* pStream = NULL;

	UINT sc_bitmapWidth = _pBitmap->GetSize().width;
	UINT sc_bitmapHeight = _pBitmap->GetSize().height;
	
	hr = m_pWICFactory->CreateBitmap(
		sc_bitmapWidth,
		sc_bitmapHeight,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapCacheOnLoad,
		&pWICBitmap);
	if (!SUCCEEDED(hr)) return;

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
	rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
	rtProps.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	rtProps.usage = D2D1_RENDER_TARGET_USAGE_NONE;

		
	hr = m_pD2DFactory->CreateWicBitmapRenderTarget(
		pWICBitmap,
		rtProps,
		&pRT
	);
	if (!SUCCEEDED(hr)) return;

	D2D1_BITMAP_PROPERTIES bpp;
	bpp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bpp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bpp.dpiX = (FLOAT)0;
	bpp.dpiY = (FLOAT)0;

	ID2D1Bitmap* bitmap;

	pRT->CreateBitmap(D2D1::SizeU(sc_bitmapWidth, sc_bitmapHeight), CBitmap::GetInst()->GetBitmapPixel(), sc_bitmapWidth * 4, &bpp, &bitmap);

	pRT->BeginDraw();
	pRT->DrawBitmap(bitmap);
	hr = pRT->EndDraw();
	if (!SUCCEEDED(hr)) return;

	hr = m_pWICFactory->CreateStream(&pStream);
	if (!SUCCEEDED(hr)) return;

	WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;

	hr = pStream->InitializeFromFilename(_fileName, GENERIC_WRITE);
	if (!SUCCEEDED(hr)) return;

	hr = m_pWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
	if (!SUCCEEDED(hr)) return;
	
	hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
	if (!SUCCEEDED(hr)) return;
	
	hr = pEncoder->CreateNewFrame(&pFrameEncode, NULL);
	if (!SUCCEEDED(hr)) return;
	
	hr = pFrameEncode->Initialize(NULL);
	if (!SUCCEEDED(hr)) return;
	
	hr = pFrameEncode->SetSize(sc_bitmapWidth, sc_bitmapHeight);
	if (!SUCCEEDED(hr)) return;
	
	hr = pFrameEncode->SetPixelFormat(&format);
	if (!SUCCEEDED(hr)) return;
	
	hr = pFrameEncode->WriteSource(pWICBitmap, NULL);
	if (!SUCCEEDED(hr)) return;
	
	hr = pFrameEncode->Commit();
	if (!SUCCEEDED(hr)) return;
	
	hr = pEncoder->Commit();
	if (!SUCCEEDED(hr)) return;
}


void CCore::CleanupDevice()
{
	//if (m_pWICFactory) m_pWICFactory->Release();
	if (m_pD2DFactory) m_pD2DFactory->Release();
	CoUninitialize();
}

bool CCore::Init()
{
	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return false;
	}
}

ID2D1HwndRenderTarget* CCore::CreateRenderTarget(HWND _hWnd)
{
	HRESULT hr = S_OK;
	ID2D1HwndRenderTarget* renderTarget = nullptr;

	RECT	rc;
	GetClientRect(_hWnd, &rc);

	hr = m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE),
		D2D1::HwndRenderTargetProperties(_hWnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&renderTarget);
	if (FAILED(hr)) return nullptr;



	return renderTarget;
}
