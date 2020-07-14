#pragma once
template<class T>
struct Rect
{
public:
	T x, y, weidth, height;
	Rect() : x(0), y(0) {};
	Rect(const T& X, const T& Y, const T& Weidth, const T& Height) :x(X), y(Y), weidth(Weidth), height(Height) {};

	Rect operator=(const Rect& rect)
	{
		this->x = rect.x;
		this->y = rect.y;
		this->weidth = rect.weidth;
		this->height = rect.height;
		return *this;
	}
};

typedef Rect<int> RectI;
typedef Rect<float> RectF;
typedef Rect<unsigned int> RectU;