#pragma once
#include "image.h"
#include "point.h"

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

class Graphics
{
public:
	Graphics() {};

	bool init(HWND windowSystemHandle);

	void BeginDraw() { this->m_renderTarget->BeginDraw(); }
	
	void Clear(const D2D1_COLOR_F& color) { this->m_renderTarget->Clear(color); }
	void DrawBitmap(Image* image,const PointF& position = PointF(0,0));
	void DrawRectangle(const RectF& bounds, const D2D1_COLOR_F& color);
	//void DrawString(StringDrawable& format,const PointF& position);
	void FillRectangle()

	void EndDraw() { this->m_renderTarget->EndDraw(); }

	void Resize();

	~Graphics();

	static Graphics* Create(HWND windowSystemHandle);
private:
	ID2D1Factory* m_factory;
	ID2D1HwndRenderTarget* m_renderTarget;
	IDWriteFactory* m_writeFactory;
};

