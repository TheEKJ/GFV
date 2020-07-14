#include "app.h"
#include "widgets/button.h"
#include "graphics/drawable/rectdrawable.h"

Image* image;
Button* button;
/*
...Configuracion...
init()
*/

App::App() : WindowAdapter(RectI(CW_USEDEFAULT, CW_USEDEFAULT, 800, 400), L"App")
{}


void App::OnCreate()
{
	SetPredefinedPosition(PredefinedPosition::CenterMedium);
	/*
	ButtonColors colors = {D2D1::ColorF(D2D1::ColorF::Yellow),
	D2D1::ColorF(D2D1::ColorF::Red),
	D2D1::ColorF(D2D1::ColorF::DarkRed)};
	button = new Button(RectI(0, 0, 300, 300), colors);
	image = new Image(L"C:\\Users\\Usuario\\Desktop\\Pixel Art\\Humano.png");
	
	addComponent(button);*/
	RectDrawable* rect = new RectDrawable(RectF(10, 10, 100, 100), D2D1::ColorF::Chocolate);
	addDrawable(rect);
}

void App::addComponent(ChildWindow* child)
{
	child->SetParentWindowHandle(m_hwnd);
	assert(child->init());
	m_components.push_back(std::make_unique<ChildWindow>(*child));
}
void App::deleteComponents()
{
	m_components.erase(m_components.begin(), m_components.end());
}

void App::OnPaint()
{
	m_graphics->Clear(D2D1::ColorF(D2D1::ColorF::Aqua));
}
