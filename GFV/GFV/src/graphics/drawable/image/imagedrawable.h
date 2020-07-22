#pragma once
#include "graphics/drawable/idrawable.h"
#include "graphics/graphics.h"
#include "graphics/point.h"
#include "graphics/size.h"

#include <wincodec.h>

template<typename T>
inline void SafeRelease(T*& pI)
{
	if (nullptr != pI)
	{
		pI->Release();
		pI = nullptr;
	}
}

enum IDRAW_TYPE_LOAD
{
	FILEPATH,
	RESOURCE
};

class ImageDrawable : public IDrawable
{
public:
	ImageDrawable(const wchar_t* wImage,const IDRAW_TYPE_LOAD& typeLoad,const PointF& position = PointF(0));

	void SetPosition(const PointF& position) { m_position = position; }
	PointF GetPosition() const { return m_position; }

	void SetScale(const PointF& scale) { m_scale = scale; }
	void SetOpacity(const float& opacity) { m_opacity = opacity; }

	void SetSize(const SizeU& size) { m_size = size; }
	SizeU GetSize() const { return m_size; }

	virtual void Draw(Graphics* source) override;

protected:
	bool init(const wchar_t* wImage);

	bool m_init;

	PointF m_position;
	SizeU m_size;
	float m_opacity;
	PointF m_scale;
	IDRAW_TYPE_LOAD m_typeLoad;

	IWICBitmapDecoder* m_bitmapDecoder;
	IWICBitmapFrameDecode* m_bitmapFrameDecode;
	IWICFormatConverter* m_formatConverter;
	ID2D1Bitmap* m_bitmap;
};