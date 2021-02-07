#include "pch.h"
#include "Color.h"

// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Color::Color()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Color::Color(float luminosity)
{
	x, y, z = luminosity;
	w = 0.0f;
}

Color::Color(float red, float green, float blue)
{
	x = red;
	y = green;
	z = blue;
	w = 0.0f;
}

Color::Color(float red, float green, float blue, float alpha)
{
	x = red;
	y = green;
	z = blue;
	w = alpha;
}


Color::~Color()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

Color Color::multiply(const Color & right_color)
{
	return Color(
		this->x * right_color.x,
		this->y * right_color.y,
		this->z * right_color.z
		);
}
