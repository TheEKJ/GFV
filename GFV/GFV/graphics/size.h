#pragma once
template<class T>
struct Size
{
public:
	T width, height;
	Size() : width(0), width(0) {};
	Size(const T& Width, const T& Height) : width(Width), height(Height) {};

	Size operator=(const Size& size)
	{
		this->width = size.width;
		this->height = size.height;
		return *this;
	}
};

typedef Size<float> SizeF;
typedef Size<int> SizeI;
typedef Size<unsigned int> SizeU;