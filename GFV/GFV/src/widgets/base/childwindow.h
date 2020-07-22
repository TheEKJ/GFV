#pragma once
#include "os/win/window.h"
class ChildWindow : public Window
{
public:
	ChildWindow(const RectI& bounds, const wchar_t* nameclass) : Window(bounds, nameclass) { std::cout << "ChildWindow is created" << std::endl; };
	~ChildWindow()
	{
		std::cout << "Childwindow is destroyed" << std::endl;
	}

	bool init(HWND parent);

	virtual void OnCreate() override {};
	virtual void OnPaint() override {};
	virtual void OnLeave() override {};
	virtual void OnMouseUp(const MouseButton& button) override {};
	virtual void OnMouseDown(const MouseButton& button) override {};
	virtual void OnMouseMove(const PointI& position) override {};
	virtual void OnCommandExecute(const UINT& id) override {};
	virtual void OnResize() override {};
	virtual void OnDropFile(const wchar_t* filePath) {};

protected:
	HWND CreateHWND();

};