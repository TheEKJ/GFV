#pragma once
#include <Windows.h>

template<class T>
struct Size
{
public:
	T width, height;
	Size() : width(0), height(0) {};
	Size(const T& size) : width(size), height(size) {};
	Size(const T& Width, const T& Height) : width(Width), height(Height) {};

	POINT GetPOINT() { return POINT{ (long)width,(long)height }; }

	Size operator=(const Size& size)
	{
		this->width = size.width;
		this->height = size.height;
		return *this;
	}

	bool isZero()
	{
		return ((width | height) == 0) ? true : false;
	}
};

typedef Size<float> SizeF;
typedef Size<int> SizeI;
typedef Size<unsigned int> SizeU;