#include "button.h"

Button::Button(const RectI& bounds, const ButtonColors& colors) : ChildWindow(bounds, L"App.button.class"),
m_colors(colors)
{}

void Button::OnCreate()
{
	assert(m_hwndParent != nullptr);
	m_actualColor = &m_colors.release;
}

void Button::OnPaint()
{

	m_graphics->Clear(*m_actualColor);
	/*
	if (m_image)
		m_graphics->DrawBitmap(m_image);*/

}


void Button::OnMouseMove(const PointI& position)
{
	if (position.Contains(m_bounds))
	{
		m_isHover = true;
	}
	else
	{
		m_isHover = false;
	}

	if (!m_isPress)
	{
		m_actualColor = &m_colors.hover;
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void Button::OnMouseDown(const MouseButton& button)
{
	if (button == MouseButton::Left)
	{
		SetCapture(m_hwnd);
		m_isPress = true;
		m_actualColor = &m_colors.press;
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void Button::OnMouseUp(const MouseButton& button)
{
	if (button == MouseButton::Left)
	{
		m_isPress = false;
		if (m_isHover && m_onClick)
			m_onClick();
		ReleaseCapture();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void Button::OnLeave()
{
	if (!m_isPress)
	{
		// TODO: AÑADIR OPACIDAD A IMAGENES
		m_actualColor = &m_colors.release;
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}