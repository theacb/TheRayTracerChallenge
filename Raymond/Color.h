#pragma once
#include "tuple.h"
#include <algorithm>
#include <string>

// Float Color

class Color :
	public Tuple
{
public:
	// Constructors
	Color();
	Color(float);
	Color(float, float, float);
	Color(float, float, float, float);

	// Destrcutor
	~Color();

	// Properties
	float& r() { return this->x; }
	float& g() { return this->y; }
	float& b() { return this->z; }
	float& a() { return this->w; }

	// Methods
	Color multiply(const Color &);
	//  - Converters
	Color convert_linear_to_srgb();
	Color convert_srgb_to_linear();
};

// 8 Bit Color

class Color8Bit
{
public:
	// Constructors

	Color8Bit();
	Color8Bit(int, int, int);
	Color8Bit(const Color);

	~Color8Bit();

	// Properties

	int r, g, b;

	// Methods
	std::string output();

	// Overloaded Operators

	friend bool operator==(const Color8Bit &, const Color8Bit &);
	friend std::ostream & operator<<(std::ostream &, const Color8Bit &);
};

// Helper Functions

float linear_to_srgb(const float);
float srgb_to_linear(const float);

template<typename T>
inline T clip(const T & n, const T & lower, const T & upper)
{
	return std::max(lower, std::min(n, upper));
}
