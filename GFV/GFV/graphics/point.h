#pragma once
template<class T>
struct Point
{
public:
	T x, y;

	Point() : x(0), y(0) {};
	Point(const T& X, const T& Y) : x(X), y(Y) {};

	Point operator=(const Point& point)
	{
		this->x = point.x;
		this->y = point.y;
		return *this;
	}
};

typedef Point<float> PointF;
typedef Point<int> PointI;
typedef Point<unsigned int> PointU;