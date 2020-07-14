#pragma once
#include <d2d1.h>
class Font
{
public:
	Font(const wchar_t* familyName, const D2D1::ColorF& color,const size_t& size);
	
	const wchar_t* GetFamilyName() { return m_familyName; }
	D2D1::ColorF GetColor() const { return m_color; }
	size_t GetSize() const { return m_size; }

private:
	const wchar_t* m_familyName;
	D2D1::ColorF m_color;
	size_t m_size;
};