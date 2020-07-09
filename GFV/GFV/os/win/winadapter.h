#pragma once
#include "window.h"

class WindowAdapter : public Window
{
public:
	WindowAdapter(const RectI& bounds, const wchar_t* nameclass) : Window(bounds, nameclass) {};

	virtual void OnCreate() override {};
};