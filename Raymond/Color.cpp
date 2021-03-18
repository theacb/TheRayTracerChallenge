#include "pch.h"
#include "Color.h"

// ------------------------------------------------------------------------
//
// Color double
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Color::Color() : Tuple()
{
}

Color::Color(double luminosity) : Tuple(luminosity, luminosity, luminosity, 0.0)
{
}

Color::Color(double red, double green, double blue) : Tuple(red, green, blue, 0.0)
{
}

Color::Color(double red, double green, double blue, double alpha) : Tuple(red, green, blue, alpha)
{
}

Color::Color(const Tuple & a) : Tuple(a)
{
}

Color::Color(const Color8Bit & col) : Color(double(col.r) / 255.0, double(col.g) / 255.0, double(col.b) / 255.0)
{
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

Tuple Color::operator*(const Color & right_color) const
{
	return Tuple(
		this->x * right_color.x,
		this->y * right_color.y,
		this->z * right_color.z,
		this->w * right_color.w
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

Color8Bit::Color8Bit(const Color & source_color)
{
	// Casts the double to an integer after clipping it to the 0.0 to 1.0 range 
	// and multiplying it by 255
	r = static_cast<int>(clip<double>(source_color.x, 0.0, 1.0) * 255);
	g = static_cast<int>(clip<double>(source_color.y, 0.0, 1.0) * 255);
	b = static_cast<int>(clip<double>(source_color.z, 0.0, 1.0) * 255);
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

std::ostream & operator<<(std::ostream & os, const Color & col)
{
	os << "(" << col.x << ", " << col.y << ", " << col.z << ", " << col.w << ")";
	return os;
}

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
std::ostream & operator<< (std::ostream & os, const Color8Bit & col)
{
	os << "(" << col.r << ", " << col.g << ", " << col.b << ", " << ")";
	return os;
}

// ------------------------------------------------------------------------
// Helper Functions
// ------------------------------------------------------------------------

double linear_to_srgb(const double x)
{
	// Converts the channel from linear values to the sRGB color curve
	return (x >= 0.0031308) ? (1.055 * (pow(x, (1.0 / 2.4)))) - 0.055 : 12.92 * x;
}

double srgb_to_linear(const double x)
{
	// Converts the channel from the sRGB color curve to linear values
	return (x >= 0.04045) ? pow((x +0.055)/1.055, 2.4) : x / 12.92;
}
