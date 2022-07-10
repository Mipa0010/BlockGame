#pragma once
#include "Rectangle.h"

class Tile : public Rectangle
{
public:
	using Rectangle::Rectangle;
	Tile(const Rectangle& rectangle);
};

