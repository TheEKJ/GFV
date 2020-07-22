#pragma once
#include "point.h"
#include "os/font.h"
#include "graphics/drawable/idrawable.h"

#include <Windows.h>
#include <d2d1.h>

class Graphics
{
public:
	Graphics() : m_init(false) {};

	bool init(HWND windowSystemHandle);

	void BeginDraw() { this->m_renderTarget->BeginDraw(); }
	
	void Clear(const D2D1_COLOR_F& color) { this->m_renderTarget->Clear(color); }
	void DrawRectangle(const RectF& bounds, const D2D1_COLOR_F& color);
	void DrawString(const wchar_t* text,const Font& font);
	void Draw(IDrawable* drawable);

	void EndDraw() { this->m_renderTarget->EndDraw(); }

	void Resize();

	ID2D1HwndRenderTarget* GetRenderTarget() { return m_renderTarget; }

	~Graphics();

	static Graphics* Create(HWND windowSystemHandle);
private:
	bool m_init;
	ID2D1Factory* m_factory;
	ID2D1HwndRenderTarget* m_renderTarget;
};

