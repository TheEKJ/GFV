#pragma once
#include "window.h"

class WindowAdapter : public Window
{
public:
	WindowAdapter(const RectI& bounds, const wchar_t* nameclass) : Window(bounds, nameclass) {};

	virtual void OnCreate() override {};
	virtual void OnPaint() override {};
	virtual void OnLeave() override {};
	virtual void OnMouseUp(const MouseButton& button) override {};
	virtual void OnMouseDown(const MouseButton& button) override {};
	virtual void OnMouseMove(const PointI& position) override {};
	virtual void OnCommandExecute(const UINT& id) override {};
	virtual void OnResize() override {};
	virtual void OnDropFile(const wchar_t* filePath) override {};
};