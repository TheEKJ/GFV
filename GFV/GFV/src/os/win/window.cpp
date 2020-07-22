#include "window.h"
#include "resource.h"

#include <windowsx.h>
#include <math.h>

#define ID_TIMER_PLAY 991
#define ID_TIMER_FADE 992
// Prototipos

void MouseDown(Window*,const UINT&);
void MouseUp(Window*, const UINT&);
//!

Window::Window(const RectI& bounds, const wchar_t* nameclass) : m_bounds(bounds),
m_hwnd(nullptr),
m_hwndParent(nullptr),
m_nameclass(nameclass),
m_cursor(LoadCursor(nullptr,IDC_ARROW))
{}

bool Window::init()
{
	Register_Class(this);
	this->m_hwnd = CreateHWND();

	if (m_hwnd == nullptr)
		return false;

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA,reinterpret_cast<LONG>(this));

	m_graphics = Graphics::Create(m_hwnd);

	m_trackmouseleave.cbSize = sizeof(TRACKMOUSEEVENT);
	m_trackmouseleave.dwFlags = TME_LEAVE;
	m_trackmouseleave.hwndTrack = m_hwnd;

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
	wcex.style = CS_HREDRAW | CS_VREDRAW;

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

		if (window && window->m_hwnd == nullptr)
			window->m_hwnd = hwnd;
	}
	else
	{
		window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (window && window->m_hwnd != hwnd)
			window = nullptr;
	}

	if (window)
	{
		assert(window->m_hwnd == hwnd);
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
	case WM_NCCREATE:
		this->OnCreate();
		break;
	case WM_SETCURSOR:
		::SetCursor(this->m_cursor);
		break;
	case WM_WINDOWPOSCHANGED:
	{
		LPWINDOWPOS newPosition = reinterpret_cast<LPWINDOWPOS>(lparam);
		m_bounds = RectI(newPosition->x, newPosition->y, newPosition->cx, newPosition->cy);
	}
	break;
	case WM_PAINT:
		if (m_graphics->GetRenderTarget()->CheckWindowState() != D2D1_WINDOW_STATE_OCCLUDED)
		{
			m_graphics->BeginDraw();

			this->OnPaint();

			m_graphics->EndDraw();
		}
	case WM_SIZE:
		m_graphics->Resize();
		this->OnResize();
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	/********************************************************************************TODO: Mensaje cuando cerramos la ventana
	case WM_CLOSE:
	{
		// REPRODUCIR SI HAY UNA IMAGEN

		EnableWindow(hwnd, FALSE);
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		SetWindowLongPtr(hwnd, GWL_EXSTYLE,WS_EX_LAYERED);
		//Cambia el mundo. Mi mensaje final. Adios
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);

		SetTimer(hwnd,ID_TIMER_PLAY, 7514 ,NULL);

		return 0;
		break;
	}
	case WM_TIMER:
	{
		static float alpha = 255.f;

		if (wparam == ID_TIMER_PLAY)
		{
			SetTimer(hwnd, ID_TIMER_FADE, 16, NULL);
			KillTimer(hwnd, ID_TIMER_PLAY);
		}
		if (wparam == ID_TIMER_FADE)
		{
			if (alpha < 1)
			{
				alpha = 0;
				KillTimer(hwnd, ID_TIMER_FADE);
				PostQuitMessage(0);
			}

			alpha -= 1.2f;
			SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), roundf(alpha), LWA_ALPHA);
		}
	}
	*/
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
	case WM_COMMAND:
		this->OnCommandExecute(LOWORD(wparam));
		break;
	case WM_DROPFILES:
	{
		HDROP hdrop = reinterpret_cast<HDROP>(wparam);
		wchar_t filePath[MAX_PATH];

		DragQueryFile(hdrop, 0, filePath, ARRAYSIZE(filePath));

		this->OnDropFile(filePath);
	}
		break;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO mmi = reinterpret_cast<LPMINMAXINFO>(lparam);
		if (!m_maxSize.isZero())
		{
			mmi->ptMaxTrackSize = m_maxSize.GetPOINT();
		}
		if (!m_minSize.isZero())
		{
			mmi->ptMinTrackSize = m_minSize.GetPOINT();
		}
	}
		break;
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

HWND Window::CreateHWND()
{
	HWND hwnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_ACCEPTFILES,
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

void Window::SetPredefinedPosition(const PredefinedPosition& pp,const PointI& padding)
{
	RECT rectWindowParent;
	if (m_hwndParent != nullptr)
	{
		GetClientRect(m_hwndParent, &rectWindowParent);
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

	newPosition += padding;

	Window::SetPosition(newPosition);
}

void Window::SetPosition(const PointI& position)
{
	SetWindowPos(m_hwnd, NULL, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSENDCHANGING);
}


void Window::SetSize(const SizeU& size)
{
	SetWindowPos(m_hwnd, NULL, 0, 0, size.width, size.height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSENDCHANGING);
}

void Window::SetMaxSize(const SizeU& size)
{
	m_maxSize = size;
}

void Window::SetMinSize(const SizeU& size)
{
	m_minSize = size;
}

void Window::SetCursor(const Cursor& cursor)
{
	switch (cursor)
	{
	case Cursor::AppStarting:	m_cursor = LoadCursor(nullptr, IDC_APPSTARTING);	break;
	case Cursor::Arrow:			m_cursor = LoadCursor(nullptr, IDC_ARROW);			break;
	case Cursor::Cross:			m_cursor = LoadCursor(nullptr, IDC_CROSS);			break;
	case Cursor::Hand:			m_cursor = LoadCursor(nullptr, IDC_HAND);			break;
	case Cursor::Help:			m_cursor = LoadCursor(nullptr, IDC_HELP);			break;
	case Cursor::IBeam:			m_cursor = LoadCursor(nullptr, IDC_IBEAM);			break;
	case Cursor::No:			m_cursor = LoadCursor(nullptr, IDC_NO);				break;
	case Cursor::SizeALL:		m_cursor = LoadCursor(nullptr, IDC_SIZEALL);		break;
	case Cursor::SizeNESW:		m_cursor = LoadCursor(nullptr, IDC_SIZENESW);		break;
	case Cursor::SizeNS:		m_cursor = LoadCursor(nullptr, IDC_SIZENS);			break;
	case Cursor::SizeNWSE:		m_cursor = LoadCursor(nullptr, IDC_SIZENWSE);		break;
	case Cursor::SizeWE:		m_cursor = LoadCursor(nullptr, IDC_SIZEWE);			break;
	case Cursor::UpArrow:		m_cursor = LoadCursor(nullptr, IDC_UPARROW);		break;
	case Cursor::Wait:			m_cursor = LoadCursor(nullptr, IDC_WAIT);			break;
	}

	::SetCursor(m_cursor);
}