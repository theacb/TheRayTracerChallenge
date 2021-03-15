#pragma once

#include <algorithm>
#include <string>

#include "Tuple.h"

class Color8Bit;

// double Color

class Color :
	public Tuple
{
public:
	// Constructors
	Color();
	Color(double);
	Color(double, double, double);
	Color(double, double, double, double);
	Color(const Tuple &);
	Color(const Color8Bit &);

	// Destrcutor
	~Color();

	// Properties
	double& r() { return this->x; }
	double& g() { return this->y; }
	double& b() { return this->z; }
	double& a() { return this->w; }

	// Methods
	Color multiply(const Color &);
	//  - Converters
	Color convert_linear_to_srgb();
	Color convert_srgb_to_linear();

	Tuple operator*(const Color &) const;
};

// 8 Bit Color

class Color8Bit
{
public:
	// Constructors
	Color8Bit();
	Color8Bit(int, int, int);
	Color8Bit(const Color&);

	~Color8Bit();

	// Properties
	unsigned r, g, b;

	// Methods
	std::string output();

};

// Overloaded Operators
bool operator==(const Color8Bit &, const Color8Bit &);
bool operator!=(const Color8Bit &, const Color8Bit &);
std::ostream & operator<<(std::ostream &, const Color8Bit &);
std::ostream & operator<<(std::ostream &, const Color &);

// Helper Functions
double linear_to_srgb(const double);
double srgb_to_linear(const double);

template<typename T>
inline T clip(const T & n, const T & lower, const T & upper)
{
	return std::max(lower, std::min(n, upper));
}
