#pragma once

#include <iostream>
#include <math.h>

class Tuple
{
public:
	// Constructors
	Tuple();
	Tuple(float, float, float, float);

	// Factories
	static Tuple Point(float, float, float);
	static Tuple Vector(float, float, float);

	// Destructor
	~Tuple();

	// Properties
	float x, y, z, w;

	// Methods
	float magnitude();
	Tuple normalize();

	static float dot(const Tuple &, const Tuple &);
	static Tuple cross(const Tuple &, const Tuple &);

	// Overloaded Operators
	Tuple operator+(const Tuple &) const;
	Tuple operator-();
	Tuple operator-(const Tuple &) const;
	Tuple operator*(const float &) const;
	Tuple operator/(const float &) const;
 
};

// Overloaded Operators
std::ostream & operator<<(std::ostream &, const Tuple &);
bool operator==(const Tuple &, const Tuple &);
bool operator!=(const Tuple &, const Tuple &);

// Helper Functions
bool flt_cmp(const float &, const float &);