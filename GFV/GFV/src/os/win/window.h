#pragma once
#include "graphics/drawable/idrawable.h"
#include "graphics/graphics.h"
#include "graphics/rect.h"
#include "graphics/point.h"
#include "graphics/size.h"
#include "os/predefinedpos.h"
#include "os/mousebutton.h"

#include <iostream>
#include <Windows.h>
#include <d2d1.h>
#include <assert.h>
#include <vector>
#include <memory>

class Window
{
public:
	Window(const RectI& bounds, const wchar_t* nameclass);
	~Window()
	{
		DestroyWindow(m_hwnd);
		m_graphics->~Graphics();
	}

	bool init();
	
	void SetTitle(const wchar_t* title);
	const wchar_t* GetTitle();

	HWND GetWindowHandle() { return this->m_hwnd; }

	HWND GetParentWindowHandle() { return this->m_hwndParent; }
	void SetParentWindowHandle(HWND windowHandle) { this->m_hwndParent = windowHandle; }

	RectI GetBounds() { return this->m_bounds; }

	void SetPosition(const PointI& position);
	void SetPredefinedPosition(const PredefinedPosition& pp);

	void SetSize(const SizeU& size);

	void addDrawable(IDrawable* drawable);
	void deleteDrawable();

	virtual void OnCreate() = 0;
	virtual void OnPaint() = 0;
	virtual void OnLeave() = 0;
	virtual void OnMouseUp(const MouseButton& button) = 0;
	virtual void OnMouseDown(const MouseButton& button) = 0;
	virtual void OnMouseMove(const PointI& position) = 0;

protected:
	HWND m_hwndParent;
	Graphics* m_graphics;
	HWND m_hwnd;
	const wchar_t* m_nameclass;
	RectI m_bounds;
	std::vector<IDrawable*> m_drawables;
protected:
	virtual HWND CreateHWND();
private:
	TRACKMOUSEEVENT m_trackmouseleave;
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void Register_Class(Window* self);
	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};