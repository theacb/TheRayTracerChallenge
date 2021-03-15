#pragma once

#include <iostream>
#include "math.h"

#include "Constants.h"

class Tuple
{
public:
	// Constructors
	Tuple();
	Tuple(double, double, double, double);
	Tuple(const Tuple &);

	// Factories
	static Tuple Point(double, double, double);
	static Tuple Origin();
	static Tuple Vector(double, double, double);

	// Destructor
	~Tuple();

	// Properties
	double x, y, z, w;

	// Methods
	double magnitude() const;
	Tuple normalize() const;

	static double dot(const Tuple &, const Tuple &);
	static Tuple cross(const Tuple &, const Tuple &);
	static double distance(const Tuple &, const Tuple &);
	static Tuple reflect(const Tuple &, const Tuple &);
	static Tuple entry_wise(const Tuple &, const Tuple &);

	// Overloaded Operators
	Tuple operator+(const Tuple &) const;
	Tuple operator-();
	Tuple operator-(const Tuple &) const;
	Tuple operator*(const double &) const;
	Tuple operator/(const double &) const;
 
};

// Overloaded Operators
std::ostream & operator<<(std::ostream &, const Tuple &);
bool operator==(const Tuple &, const Tuple &);
bool operator!=(const Tuple &, const Tuple &);

// Helper Functions
bool flt_cmp(const double &, const double &);