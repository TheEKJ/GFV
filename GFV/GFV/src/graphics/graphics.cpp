#include "graphics.h"
#include "global.h"

#include <sstream>
#include <assert.h>

template<typename T>
inline void SafeRelease(T*& id2d1)
{
	if (nullptr != id2d1)
	{
		id2d1->Release();
		id2d1 = nullptr;
	}
}

bool Graphics::init(HWND windowSystemHandle)
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory)))
		return false;
	
	RECT sizeWindow;
	GetClientRect(windowSystemHandle, &sizeWindow);

	if (FAILED(m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowSystemHandle, D2D1::SizeU(sizeWindow.right, sizeWindow.bottom)),
		&m_renderTarget)))
		return false;

	m_init = true;
	return true;
}

Graphics* Graphics::Create(HWND windowSystemHandle)
{
	Graphics* graphics = new Graphics();

	if (!graphics->init(windowSystemHandle))
	{
		std::wostringstream* messageError = new std::wostringstream();

		*messageError << L"Error: "<<__FILEW__<<L" linea: "<<__LINE__;

		MessageBox(nullptr,messageError->str().c_str(), L"", MB_OK | MB_ICONERROR);

		delete messageError;
		return nullptr;
	}

	return graphics;
}

/*
void Graphics::DrawBitmap(Image* image,const PointF& position)
{
	assert(image);

	std::once_flag flag;
	std::call_once(*image->GetFlag(), [&]()->void
		{
			assert(SUCCEEDED(m_renderTarget->CreateBitmapFromWicBitmap(image->GetBitmapSource(), image->GetBitmapPtr())));
		});
	
	m_renderTarget->DrawBitmap(image->GetBitmap(), D2D1::RectF(position.x, position.y, position.x + image->GetBitmap()->GetSize().width * image->GetScale().x, position.y + image->GetBitmap()->GetSize().height * image->GetScale().y),image->GetOpacity(),D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}*/

void Graphics::DrawRectangle(const RectF& bounds, const D2D1_COLOR_F& color)
{
	ID2D1SolidColorBrush* brush;
	m_renderTarget->CreateSolidColorBrush(color, &brush);
	m_renderTarget->DrawRectangle(D2D1::RectF(bounds.x, bounds.y, bounds.x + bounds.weidth,bounds.y + bounds.height), brush);
	brush->Release();
	// CREAR UN IMAGEDRAWABLE, CREAR UNA ESTRUCTURA DE TAMAÑO, COLOR DE BORDE,RELLENO DE UN RECTANGULO

	// CREAR UN DRAWRECTANGLE CON ESA ESTRUCTURA EN GRAPHICS.H
}

void Graphics::Resize()
{
	if (this != nullptr)
	{
		HWND hwnd = m_renderTarget->GetHwnd();
		RECT sizeWindow;
		GetClientRect(hwnd, &sizeWindow);
		m_renderTarget->Resize(D2D1::SizeU(sizeWindow.right, sizeWindow.bottom));
	}
}

Graphics::~Graphics()
{
	if (nullptr != this && m_init)
	{
		SafeRelease(m_factory);
		SafeRelease(m_renderTarget);
		m_init = false;
	}
}

void Graphics::DrawString(const wchar_t* text, const Font& font)
{
	ID2D1SolidColorBrush* brush;
	IDWriteTextFormat* textFormat;
	// TODO: 15/07/2020: Añadir funcion drawtext en graphics e iniciar con el desarrollo visual
	m_renderTarget->CreateSolidColorBrush(font.GetColor(), &brush);

	global_IDWriteFactory->CreateTextFormat(font.GetFamilyName(), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, font.GetSize(), L"", &textFormat);

	m_renderTarget->DrawTextW(text, std::wcslen(text), textFormat, D2D1::RectF(0, 0,(wcslen(text)-1) *  (font.GetSize() / 2), NULL), brush);

	brush->Release();
	textFormat->Release();
}

void Graphics::Draw(IDrawable* drawable)
{
	if(drawable)
		drawable->Draw(this);
}

/*
void Graphics::DrawString(StringDrawable& format,const PointF& position)
{
	ID2D1SolidColorBrush* brush;
	IDWriteTextFormat* textFormat;
	D2D1_RECT_F rect = D2D1::RectF(position.x, position.y,position.x + (format.GetSize()-1) * (format.GetFont()->GetSize() / 2), NULL);

	m_renderTarget->CreateSolidColorBrush(format.GetFont()->GetColor(), &brush);

	m_writeFactory->CreateTextFormat(format.GetFont()->GetFamilyName(), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, format.GetFont()->GetSize(), L"", &textFormat);
	
	m_renderTarget->DrawTextW(format.GetString(),format.GetSize(),textFormat,rect,brush);
	
	m_renderTarget->DrawRectangle(rect, brush, 3);

	brush->Release();
	textFormat->Release();
}*/