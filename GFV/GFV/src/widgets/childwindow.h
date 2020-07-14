#pragma once
#include "os/win/window.h"
class ChildWindow : public Window
{
public:
	ChildWindow(const RectI& bounds, const wchar_t* nameclass) : Window(bounds, nameclass) {};

	virtual void OnCreate() override {};
	virtual void OnPaint() override {};
	virtual void OnLeave() override {};
	virtual void OnMouseUp(const MouseButton& button) override {};
	virtual void OnMouseDown(const MouseButton& button) override {};
	virtual void OnMouseMove(const PointI& position) override {};


protected:
	HWND CreateHWND();

};