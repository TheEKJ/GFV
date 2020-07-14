#pragma once
#include "graphics/graphics.h"
#include <iostream>


interface IDrawable
{
public:
	~IDrawable()
	{
		std::cout << "ELiminado" << std::endl;
	}

	virtual void Draw(Graphics* source) = 0;
};