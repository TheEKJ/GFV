#include "app.h"

App::App() : WindowAdapter(RectI(CW_USEDEFAULT, CW_USEDEFAULT, 800, 400), L"App")
{
	assert(init());
}

void App::OnCreate()
{
	SetPredefinedPosition(PredefinedPosition::CenterMedium);
}