#pragma once
class Graphics;

__interface IDrawable
{
public:
	virtual void Draw(Graphics* source) = 0;
};
