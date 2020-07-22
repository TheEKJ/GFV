#pragma once
#include "rect.h"

#include <Windows.h>

template<class T>
struct Point
{
public:
	T x, y;

	Point() : x(0), y(0) {};
	Point(const T& point) : x(point), y(point) {};
	Point(const T& X, const T& Y) : x(X), y(Y) {};
	
	POINT GetPOINT() { return POINT{ x,y }; }

	Point operator=(const Point& point)
	{
		this->x = point.x;
		this->y = point.y;
		return *this;
	}

	Point operator()(const T& points)
	{
		this->x = points;
		this->y = points;
		return *this;
	}

	Point operator+=(const Point& point)
	{
		this->x += point.x;
		this->y += point.y;
		return *this;
	}

	bool Contains(const Rect<T>& bounds) const
	{
		if (x >= bounds.x && x <= bounds.x + bounds.weidth)
		{
			if (y >= bounds.y && y <= bounds.y + bounds.height)
				return true;
		}

		return false;
	}

};

typedef Point<float> PointF;
typedef Point<int> PointI;
typedef Point<unsigned int> PointU;