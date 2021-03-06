#include "childwindow.h"

bool ChildWindow::init(HWND parent)
{
	this->m_hwndParent = parent;
	return Window::init();
}

HWND ChildWindow::CreateHWND()
{
	HWND hwnd = CreateWindow(
		this->m_nameclass,
		L"",
		WS_CHILD,
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
