#include "pch.h"
#include "Color.h"

// ------------------------------------------------------------------------
//
// Color Float
//
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

Color Color::convert_linear_to_srgb()
{
	return Color(
		linear_to_srgb(x), 
		linear_to_srgb(y),
		linear_to_srgb(z)
		);
}

Color Color::convert_srgb_to_linear()
{
	return Color(
		srgb_to_linear(x),
		srgb_to_linear(y),
		srgb_to_linear(z)
	);
}

// ------------------------------------------------------------------------
//
// Color 8 Bit
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Color8Bit::Color8Bit()
{
	r = 0;
	g = 0;
	b = 0;
}

Color8Bit::Color8Bit(int red, int green, int blue)
{
	r = clip<int>(red, 0, 255);
	g = clip<int>(green, 0, 255);
	b = clip<int>(blue, 0, 255);
}

Color8Bit::Color8Bit(const Color source_color)
{
	// Casts the float to an integer after clipping it to the 0.0 to 1.0 range 
	// and multiplying it by 255
	r = static_cast<int>(clip<float>(source_color.x, 0.0f, 1.0f) * 255);
	g = static_cast<int>(clip<float>(source_color.y, 0.0f, 1.0f) * 255);
	b = static_cast<int>(clip<float>(source_color.z, 0.0f, 1.0f) * 255);
}

Color8Bit::~Color8Bit()
{
}

std::string Color8Bit::output()
{
	// returns a string representint the channel digits seperated by spaces
	// Examples 
	//		0 0 0
	//		18 2 255
	//		255 255 255
	return std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";
}

// ------------------------------------------------------------------------
// External Overloaded Operators
// ------------------------------------------------------------------------

bool operator==(const Color8Bit & left_color, const Color8Bit & right_color)
{
	return (
		left_color.r == right_color.r &&
		left_color.g == right_color.g &&
		left_color.b == right_color.b
		);
}

bool operator!=(const Color8Bit & left_color, const Color8Bit & right_color)
{
	return ! (left_color == right_color);
}

// Representation with std::cout
std::ostream & operator<< (std::ostream & os, const Color8Bit & color)
{
	os << "(" << color.r << ", " << color.g << ", " << color.b << ", " << ")";
	return os;
}

// ------------------------------------------------------------------------
// Helper Functions
// ------------------------------------------------------------------------

float linear_to_srgb(const float x)
{
	// Converts the channel from linear values to the sRGB color curve
	return (x >= 0.0031308f) ? (1.055f * (pow(x, (1.0f / 2.4f)))) - 0.055f : 12.92f * x;
}

float srgb_to_linear(const float x)
{
	// Converts the channel from the sRGB color curve to linear values
	return (x >= 0.04045f) ? pow((x +0.055f)/1.055f, 2.4f) : x / 12.92f;
}
