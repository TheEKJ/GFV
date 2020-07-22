#pragma once
#include "graphics/graphics.h"
#include "graphics/rect.h"
#include "graphics/point.h"
#include "graphics/size.h"
#include "os/predefinedpos.h"
#include "os/mousebutton.h"
#include "os/cursor.h"

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

	virtual bool init();
	
	void SetTitle(const wchar_t* title);
	const wchar_t* GetTitle();

	HWND GetWindowHandle() { return this->m_hwnd; }

	RectI GetBounds() { return this->m_bounds; }

	void SetPosition(const PointI& position);
	void SetPredefinedPosition(const PredefinedPosition& pp,const PointI& padding = PointI(0,0));

	void SetSize(const SizeU& size);

	void SetMaxSize(const SizeU& size);
	void SetMinSize(const SizeU& size);

	void SetCursor(const Cursor& cursor);

	virtual void OnCreate() = 0;
	virtual void OnPaint() = 0;
	virtual void OnLeave() = 0;
	virtual void OnResize() = 0;
	virtual void OnDropFile(const wchar_t* filePath) = 0;
	virtual void OnMouseUp(const MouseButton& button) = 0;
	virtual void OnMouseDown(const MouseButton& button) = 0;
	virtual void OnMouseMove(const PointI& position) = 0;
	virtual void OnCommandExecute(const UINT& id) = 0;

protected:
	SizeU m_maxSize,m_minSize;
	HCURSOR m_cursor;
	HWND m_hwndParent;
	Graphics* m_graphics;
	HWND m_hwnd;
	const wchar_t* m_nameclass;
	RectI m_bounds;
protected:
	virtual HWND CreateHWND();
private:
	TRACKMOUSEEVENT m_trackmouseleave;
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void Register_Class(Window* self);
	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};