#pragma once
#include "childwindow.h"

#include <functional>
/// <summary>
/// Los colores del boton cuando está encima, afuera y presionado, respectivamente
/// </summary>
struct ButtonColors
{
	D2D1_COLOR_F hover;
	D2D1_COLOR_F release;
	D2D1_COLOR_F press;
};

class Button : public ChildWindow
{
public:
	Button(const RectI& bounds, const ButtonColors& colors);
	
	void OnCreate();
	void OnPaint();
	void OnLeave();
	void OnMouseUp(const MouseButton& button);
	void OnMouseMove(const PointI& position);
	void OnMouseDown(const MouseButton& button);

	void Release()
	{
		m_image = nullptr;
		InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void Action(const std::function<void()>& function)
	{
		m_onClick = function;
	}

private:
	ButtonColors m_colors;
	D2D1_COLOR_F* m_actualColor;
	bool m_isHover = false;
	bool m_isPress = false;
	std::function<void()> m_onClick;
	Image* m_image;
};

