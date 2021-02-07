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

	// Attributes
	float x, y, z, w;

	// Methods

	float magnitude();
	Tuple normalize();

	static float dot(const Tuple &, const Tuple &);
	static Tuple cross(const Tuple &, const Tuple &);

	// Overloaded Operators

	friend std::ostream & operator<<(std::ostream &, const Tuple &);
	friend bool operator==(const Tuple &, const Tuple &);
	friend Tuple operator+(const Tuple &, const Tuple &);
	friend Tuple operator-(const Tuple &, const Tuple &);
	friend Tuple operator-(const Tuple &);
	friend Tuple operator*(const float &, const Tuple &);
	friend Tuple operator*(const Tuple &, const float &);
	friend Tuple operator/(const Tuple &, const float &);
};

bool flt_cmp(float, float);