#pragma once
#include "idrawable.h"
#include "graphics/rect.h"

class RectDrawable : public IDrawable
{
public:
	RectDrawable(const RectF& bounds, const D2D1::ColorF& colorFill) : m_colorStroke(colorFill), m_bounds(bounds) {};

	virtual void Draw(Graphics* source) override
	{
		source->DrawRectangle(m_bounds, m_colorStroke);
	}

	void Filling(bool boolean) { m_filling = boolean; }

	RectF GetBounds() { return m_bounds; }

private:
	D2D1_COLOR_F m_colorStroke;
	bool m_filling;
	RectF m_bounds;

};
