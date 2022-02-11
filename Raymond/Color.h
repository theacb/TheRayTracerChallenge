#ifndef H_RAYMOND_COLOR
#define H_RAYMOND_COLOR

#include <algorithm>
#include <string>

#include "Tuple.h"
#include "Utilities.h"

class Color8Bit;

// double Color

class Color :
	public Tuple
{
public:
	// Constructors
	Color();
	Color(double luminosity);
	Color(double red, double green, double blue);
	Color(double red, double green, double blue, double alpha);
	Color(const Tuple & a);
	Color(const Color8Bit & col);

	// Destructor
	~Color();

	// Properties
	double& r() { return this->x; }
	double& g() { return this->y; }
	double& b() { return this->z; }
	double& a() { return this->w; }

	// Blend Modes
	Color multiply(const Color & top_layer, const double & alpha);
	Color divide(const Color & top_layer, const double & alpha);
	Color add(const Color & top_layer, const double & alpha);
	Color subtract(const Color & top_layer, const double & alpha);
	Color overlay(const Color & top_layer, const double & alpha);
	Color screen(const Color & top_layer, const double & alpha);

	//  - Converters
	Color convert_linear_to_srgb();
	Color convert_srgb_to_linear();

	double luminosity() const;

	Color operator*(const Color & right_color) const;
	Color operator/(const Color & right_color) const;
	Color operator+(const Color & right_color) const;
	Color operator-(const Color & right_color) const;
};

// 8 Bit Color

class Color8Bit
{
public:
	// Constructors
	Color8Bit();
	Color8Bit(int red, int green, int blue);
	Color8Bit(const Color& source_color);

	~Color8Bit();

	// Properties
	unsigned r, g, b;

	// Methods
	std::string output();

};

// Overloaded Operators
bool operator==(const Color8Bit &, const Color8Bit &);
bool operator!=(const Color8Bit &, const Color8Bit &);
std::ostream & operator<<(std::ostream & os, const Color8Bit & col);
std::ostream & operator<<(std::ostream & os, const Color & col);

// Helper Functions
double linear_to_srgb(const double x);
double srgb_to_linear(const double x);
double overlay_channel(const double a, const double b);
double screen_channel(const double a, const double b);
double safe_comp_divide(const double a, const double b);

#endif
