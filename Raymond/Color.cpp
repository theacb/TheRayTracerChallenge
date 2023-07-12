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
= default;

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

Color Color::multiply(const Color & top_layer, const double & alpha)
{
	Color result = Color(
		this->x * top_layer.x,
		this->y * top_layer.y,
		this->z * top_layer.z
		);
	return lerp(alpha, *this, result);
}

Color Color::divide(const Color & top_layer, const double & alpha)
{
	Color result = Color(
		safe_comp_divide(this->x, top_layer.x),
		safe_comp_divide(this->y, top_layer.y),
		safe_comp_divide(this->z, top_layer.z)
	);
	return lerp(alpha, *this, result);
}

Color Color::add(const Color & top_layer, const double & alpha)
{
	Color result = Color(
		this->x + top_layer.x,
		this->y + top_layer.y,
		this->z + top_layer.z
	);
	return lerp(alpha, *this, result);
}

Color Color::subtract(const Color & top_layer, const double & alpha)
{
	Color result = Color(
		this->x - top_layer.x,
		this->y - top_layer.y,
		this->z - top_layer.z
	);
	return lerp(alpha, *this, result);
}

Color Color::overlay(const Color & top_layer, const double & alpha)
{
	Color result = Color(
		overlay_channel(this->x, top_layer.x),
		overlay_channel(this->y, top_layer.y),
		overlay_channel(this->z, top_layer.z)
	);
	return lerp(alpha, *this, result);
}

Color Color::screen(const Color & top_layer, const double & alpha)
{
	Color result = Color(
		screen_channel(this->x, top_layer.x),
		screen_channel(this->y, top_layer.y),
		screen_channel(this->z, top_layer.z)
	);
	return lerp(alpha, *this, result);
}

Color Color::convert_linear_to_srgb()
{
	return {
		linear_to_srgb(x), 
		linear_to_srgb(y),
		linear_to_srgb(z)
		};
}

Color Color::convert_srgb_to_linear()
{
	return {
		srgb_to_linear(x),
		srgb_to_linear(y),
		srgb_to_linear(z)
	};
}

double Color::luminosity() const
{
	return (0.299 * this->x) + (0.587 * this->y) + (0.114 * this->z);
}

Color Color::operator*(const Color & right_color) const
{
	return {
		this->x * right_color.x,
		this->y * right_color.y,
		this->z * right_color.z
	};
}

Color Color::operator/(const Color & right_color) const
{
	return {
		safe_comp_divide(this->x, right_color.x),
		safe_comp_divide(this->y, right_color.y),
		safe_comp_divide(this->z, right_color.z)
	};
}

Color Color::operator+(const Color & right_color) const
{
	return {
		this->x + right_color.x,
		this->y + right_color.y,
		this->z + right_color.z
	};
}

Color Color::operator-(const Color & right_color) const
{
	return {
		this->x - right_color.x,
		this->y - right_color.y,
		this->z - right_color.z
	};
}

Color Color::operator*(const double &scalar) const
{
    return {
            this->x * scalar,
            this->y * scalar,
            this->z * scalar
    };
}

Color Color::operator/(const double &scalar) const
{
    return {
            safe_comp_divide(this->x, scalar),
            safe_comp_divide(this->y, scalar),
            safe_comp_divide(this->z, scalar)
    };
}

Color Color::operator+(const double &scalar) const
{
    return {
            this->x + scalar,
            this->y + scalar,
            this->z + scalar
    };
}

Color Color::operator-(const double &scalar) const
{
    return {
            this->x - scalar,
            this->y - scalar,
            this->z - scalar
    };
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
= default;

std::string Color8Bit::output() const
{
	// returns a string representing the channel digits separated by spaces
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
// sRGB
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

// ------------------------------------------------------------------------
// Blend Operations
// ------------------------------------------------------------------------

double overlay_channel(const double a, const double b)
{
	if (a < 0.5)
		return 2.0 * a * b;
	else
		return 1.0 - (2.0 * (1.0 - a)) * (1.0 - b);
}

double screen_channel(const double a, const double b)
{
	return 1.0 - (1.0 - a) * (1.0 - b);
}

// Based on the implementation from GIMP for safe composite division
//		returns a / b, clamped to [-SAFE_DIV_MAX, SAFE_DIV_MAX].
//		if -SAFE_DIV_MIN <= a <= SAFE_DIV_MIN, returns 0.
// https://gitlab.gnome.org/GNOME/gimp/-/blob/master/app/operations/layer-modes/gimpoperationlayermode-blend.c#L57

double safe_comp_divide(const double a, const double b)
{
	double result = 0.0;

	if (abs(b) > SAFE_DIV_MIN)
	{
		result = a / b;
		result = clip(result, -SAFE_DIV_MAX, SAFE_DIV_MAX);
	}

	return result;
}
