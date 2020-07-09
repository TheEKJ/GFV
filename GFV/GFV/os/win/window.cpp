#include "window.h"

Window::Window(const RectI& bounds, const wchar_t* nameclass) : m_bounds(bounds),
m_background(D2D1::ColorF::Black),
m_hwnd(nullptr),
m_hwndParent(nullptr),
m_nameclass(nameclass)
{}

bool Window::init()
{
	Register_Class(this);
	this->m_hwnd = CreateHWND(this);

	if (m_hwnd == nullptr)
		return false;

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
	wcex.hbrBackground = CreateSolidBrush(RGB(self->m_background.r * 255, self->m_background.g * 255, self->m_background.b * 255));
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

	if (window != nullptr)
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
		LPWINDOWPOS newPosition = reinterpret_cast<LPWINDOWPOS>(lparam);
		m_bounds = RectI(newPosition->x, newPosition->y, newPosition->cx, newPosition->cy);
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

HWND Window::CreateHWND(Window* self)
{
	HWND hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		self->m_nameclass,
		L"",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		self->m_bounds.x, self->m_bounds.y,
		self->m_bounds.weidth, self->m_bounds.height,
		self->m_hwndParent,
		nullptr,
		nullptr,
		self
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

