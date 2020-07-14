#include "window.h"

#include <windowsx.h>

// Prototipos

void MouseDown(Window*,const UINT&);
void MouseUp(Window*, const UINT&);

//!

Window::Window(const RectI& bounds, const wchar_t* nameclass) : m_bounds(bounds),
m_hwnd(nullptr),
m_hwndParent(nullptr),
m_nameclass(nameclass)
{}

bool Window::init()
{
	Register_Class(this);
	this->m_hwnd = CreateHWND();

	if (m_hwnd == nullptr)
		return false;

	m_graphics = Graphics::Create(m_hwnd);

	m_trackmouseleave.cbSize = sizeof(TRACKMOUSEEVENT);
	m_trackmouseleave.dwFlags = TME_LEAVE;
	m_trackmouseleave.hwndTrack = m_hwnd;

	this->OnCreate();

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	return true;
}

void Window::Register_Class(Window* self)
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = nullptr;
	wcex.hCursor = nullptr;
	wcex.hIcon = wcex.hIconSm = nullptr;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = Window::staticWndProc;
	wcex.lpszClassName = self->m_nameclass;
	wcex.style = 0;

	if (GetClassInfoEx(nullptr, self->m_nameclass, &wcex))
		std::cout << "Warning: Clase ya registrada" << std::endl;

	assert(RegisterClassEx(&wcex));
}

LRESULT Window::staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static Window* window = nullptr;
	if (msg == WM_NCCREATE)
	{
		LPCREATESTRUCT lparamWindow = reinterpret_cast<LPCREATESTRUCT>(lparam);
		window = reinterpret_cast<Window*>(lparamWindow->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
	}
	else
	{
		window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (window)
	{
		return window->WndProc(hwnd, msg, wparam, lparam);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_WINDOWPOSCHANGED:
	{
		LPWINDOWPOS newPosition = reinterpret_cast<LPWINDOWPOS>(lparam);
		m_bounds = RectI(newPosition->x, newPosition->y, newPosition->cx, newPosition->cy);
	}
	break;
	case WM_PAINT:
		m_graphics->BeginDraw();
		this->OnPaint();
		for (auto item : m_drawables)
		{
			assert(&item);
			item->Draw(m_graphics);
		}
		m_graphics->EndDraw();
		break;
	case WM_SIZE:
		m_graphics->Resize();
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
#pragma region Mouse
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		MouseDown(this,msg);
		break;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		MouseUp(this, msg);
		break;
	case WM_MOUSEMOVE:
		TrackMouseEvent(&m_trackmouseleave);
		this->OnMouseMove(PointI(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));
		break;
	case WM_MOUSELEAVE:
		this->OnLeave();
		break;
#pragma endregion
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void MouseDown(Window* window,const UINT& flags)
{
	MouseButton button;
	switch (flags)
	{
	case WM_LBUTTONDOWN: button = MouseButton::Left;	break;
	case WM_MBUTTONDOWN: button = MouseButton::Medium;	break;
	case WM_RBUTTONDOWN: button = MouseButton::Right;	break;
	}

	window->OnMouseDown(button);
}
void MouseUp(Window* window, const UINT& flags)
{
	MouseButton button;
	switch (flags)
	{
	case WM_LBUTTONUP: button = MouseButton::Left;		break;
	case WM_MBUTTONUP: button = MouseButton::Medium;	break;
	case WM_RBUTTONUP: button = MouseButton::Right;		break;
	}

	window->OnMouseUp(button);
}

void Window::addDrawable(IDrawable* drawable)
{
	m_drawables.push_back(drawable);
	std::cout << "Añadicion correcta! "<<m_drawables.size()<< std::endl;
	drawable = nullptr;
}

void Window::deleteDrawable()
{
	while (m_drawables.size() > 0)
	{
		delete m_drawables.at(m_drawables.size()-1);
		m_drawables.pop_back();
	}
}

HWND Window::CreateHWND()
{
	HWND hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		this->m_nameclass,
		L"",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		this->m_bounds.x, this->m_bounds.y,
		this->m_bounds.weidth, this->m_bounds.height,
		this->m_hwndParent,
		nullptr,
		nullptr,
		this
	);

	if (!hwnd)
		return nullptr;
	return hwnd;
}

void Window::SetTitle(const wchar_t* title)
{
	SetWindowText(m_hwnd, title);
}

const wchar_t* Window::GetTitle()
{
	wchar_t* title = new wchar_t[MAX_PATH];
	GetWindowText(m_hwnd, title, MAX_PATH);
	return title;
}

void Window::SetPredefinedPosition(const PredefinedPosition& pp)
{
	RECT rectWindowParent;
	if (m_hwndParent != nullptr)
	{
		GetWindowRect(m_hwndParent, &rectWindowParent);
	}
	else
	{
		SystemParametersInfo(SPI_GETWORKAREA, NULL, (LPVOID)& rectWindowParent, NULL);
	}

	int width = rectWindowParent.right - rectWindowParent.left;
	int heigth = rectWindowParent.bottom - rectWindowParent.top;

	PointI newPosition; // X = Y = 0

	switch (pp)
	{
	case PredefinedPosition::TopMedium:
		newPosition = PointI(width / 2 - m_bounds.weidth / 2, 0);
		break;
	case PredefinedPosition::TopRight:
		newPosition = PointI(width - m_bounds.weidth, 0);
		break;
	case PredefinedPosition::CenterLeft:
		newPosition = PointI(0, heigth / 2 - m_bounds.height / 2);
		break;
	case PredefinedPosition::CenterMedium:
		newPosition = PointI(width / 2 - m_bounds.weidth / 2, heigth / 2 - m_bounds.height / 2);
		break;
	case PredefinedPosition::CenterRight:
		newPosition = PointI(width - m_bounds.weidth, heigth / 2 - m_bounds.height / 2);
		break;
	case PredefinedPosition::BottomLeft:
		newPosition = PointI(0, heigth - m_bounds.height);
		break;
	case PredefinedPosition::BottomMedium:
		newPosition = PointI(width / 2 - m_bounds.weidth / 2, heigth - m_bounds.height);
		break;
	case PredefinedPosition::BottomRight:
		newPosition = PointI(width - m_bounds.weidth, heigth - m_bounds.height);
		break;
	}

	Window::SetPosition(newPosition);
}

void Window::SetPosition(const PointI& position)
{
	SetWindowPos(m_hwnd, NULL, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}


void Window::SetSize(const SizeU& size)
{
	SetWindowPos(m_hwnd, NULL, 0, 0, size.width, size.height, SWP_NOMOVE | SWP_NOZORDER);
}