#pragma once
#include "point.h"
#include "size.h"

#include <mutex>
#include <d2d1.h>
#include <wincodec.h>

class Image
{
public:
	Image(const wchar_t* filePath);
	~Image() { MessageBox(0, m_filePath, L"", MB_OK); }
	std::once_flag* GetFlag() const { return const_cast<std::once_flag*>(&m_flag); }
	IWICBitmapSource* GetBitmapSource() const { return m_format; }
	ID2D1Bitmap* GetBitmap() const { return m_bitmap; }
	ID2D1Bitmap** GetBitmapPtr() const { return const_cast<ID2D1Bitmap**>(&m_bitmap); }
	
	void SetScale(const PointU& scale) { m_scale = scale; }
	PointU GetScale() const { return m_scale; }

	const wchar_t* GetFilePath() const { return this->m_filePath; }

	float GetOpacity() const { return m_opacity; }
	void SetOpacity(const float& opacity) { m_opacity = opacity; }

private:
	const wchar_t* m_filePath;
	IWICImagingFactory* m_imagingFactory;
	IWICBitmapDecoder* m_bitmapDecoder;
	IWICBitmapFrameDecode* m_bitmapFrameDecode;
	IWICFormatConverter* m_format;
	ID2D1Bitmap* m_bitmap;
	std::once_flag m_flag;
	float m_opacity;
	PointU m_scale;
};

