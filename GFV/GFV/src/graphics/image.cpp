#include "image.h"

#include <assert.h>

Image::Image(const wchar_t* filePath) : m_scale(1.f,1.f),
m_bitmap(nullptr),
m_imagingFactory(nullptr),
m_bitmapDecoder(nullptr),
m_bitmapFrameDecode(nullptr),
m_format(nullptr),
m_filePath(filePath),
m_opacity(1.f)
{
	assert(SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_imagingFactory))));

	if (FAILED(m_imagingFactory->CreateDecoderFromFilename(m_filePath,
		NULL,
		GENERIC_READ,
		WICDecodeOptions::WICDecodeMetadataCacheOnLoad,
		&m_bitmapDecoder)))
	{
		_wassert(L"Image loading error!", __FILEW__,9);
	}

	assert(SUCCEEDED(m_bitmapDecoder->GetFrame(0, &m_bitmapFrameDecode)));

	assert(SUCCEEDED(m_imagingFactory->CreateFormatConverter(&m_format)));

	assert(SUCCEEDED(m_format->Initialize(m_bitmapFrameDecode, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut)));

}
