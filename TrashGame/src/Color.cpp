#include "Color.h"

bool Color::operator==(const Color& other)
{
	return (r == other.r && g == other.g && b == other.b);
}

bool Color::operator!=(const Color& other)
{
	return (r != other.r && g != other.g && b != other.b);
}


std::ostream& operator<<(std::ostream& stream, const Color& color)
{
	stream << static_cast<int>(color.r) << ", " << static_cast<int>(color.g) << ", " << static_cast<int>(color.b);
	return (stream);
}