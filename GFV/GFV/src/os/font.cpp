#include "font.h"

Font::Font(const wchar_t* familyName, const D2D1::ColorF& color, const size_t& size) : 
	m_familyName(familyName),
	m_color(color),
	m_size(size)
{}