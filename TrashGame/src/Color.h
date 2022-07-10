#pragma once
#include <iostream>

class Color
{
public:
	Color() : r(0), g(0), b(0), a(255)
	{ }

	Color(unsigned char r_, unsigned char g_, unsigned char b_) :
		r(r_), g(g_), b(b_), a(255)
	{
	}

	bool operator==(const Color& other);
	bool operator!=(const Color& other);

	friend std::ostream& operator<<(std::ostream& stream, const Color& color);

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};