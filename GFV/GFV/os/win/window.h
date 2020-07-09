#pragma once
#include "graphics/rect.h"
#include "graphics/point.h"
#include "graphics/size.h"
#include "os/predefinedpos.h"

#include <iostream>
#include <Windows.h>
#include <d2d1.h>
#include <assert.h>

class Window
{
public:
	Window(const RectI& bounds, const wchar_t* nameclass);

	bool init();
	
	void SetTitle(const wchar_t* title);
	const wchar_t* GetTitle();

	HWND GetWindowHandle() { return this->m_hwnd; }

	RectI GetBounds() { return this->m_bounds; }

	void SetPosition(const PointI& position);
	void SetPredefinedPosition(const PredefinedPosition& pp);

	void SetSize(const SizeU& size);
	virtual void OnCreate() = 0;
protected:
	HWND m_hwnd;
	HWND m_hwndParent;
	const wchar_t* m_nameclass;
	RectI m_bounds;
	D2D1::ColorF m_background;
protected:
	virtual HWND CreateHWND(Window* self);
private:
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void Register_Class(Window* self);
	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};