#pragma once
#include "Color.h"

class Rectangle
{
public:
	Rectangle() : x(.0f), y(.0f), width(0), height(0), color() {}

	Rectangle(float x_, float y_, int width_, int height_, Color color_) :
		x(x_), y(y_), width(width_), height(height_), color(color_)
	{
	}

	bool operator==(const Rectangle& other);
	bool operator!=(const Rectangle& other);

	inline int GetX() const { return (static_cast<int>(x)); }
	inline int GetY() const { return (static_cast<int>(y)); }
	bool PointInRect(const int x, const int y) const;

public:
	float x;
	float y;
	int width;
	int height;
	Color color;
};
