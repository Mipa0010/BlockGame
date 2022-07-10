#include "Rectangle.h"

bool Rectangle::operator==(const Rectangle& other)
{
	return (x == other.x && y == other.y && width == other.width && height == other.height && color == other.color);
}

bool Rectangle::operator!=(const Rectangle& other)
{
	return (!(x == other.x && y == other.y && width == other.width && height == other.height && color == other.color));
}

bool Rectangle::PointInRect(const int x, const int y) const
{
	return (x > GetX() && x < GetX() + width && y > GetY() && y < GetY() + height);
}