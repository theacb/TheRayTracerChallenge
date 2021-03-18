#ifndef H_RAYMOND_TUPLE
#define H_RAYMOND_TUPLE

#include <iostream>
#include "math.h"

#include "Constants.h"

class Tuple
{
public:
	// Constructors
	Tuple();
	Tuple(double x_axis, double y_axis, double z_axis, double w_axis);
	Tuple(const Tuple & src);

	// Factories
	static Tuple Point(double x_axis, double y_axis, double z_axis);
	static Tuple Origin();
	static Tuple Vector(double x_axis, double y_axis, double z_axis);

	// Destructor
	~Tuple();

	// Properties
	double x, y, z, w;

	// Methods
	double magnitude() const;
	Tuple normalize() const;

	static double dot(const Tuple & left_tuple, const Tuple & right_tuple);
	static Tuple cross(const Tuple & left_tuple, const Tuple & right_tuple);
	static double distance(const Tuple & left_tuple, const Tuple & right_tuple);
	static Tuple reflect(const Tuple & in, const Tuple & normal);
	static Tuple entry_wise(const Tuple & left_tuple, const Tuple & right_tuple);

	// Overloaded Operators
	Tuple operator+(const Tuple & right_tuple) const;
	Tuple operator-();
	Tuple operator-(const Tuple & right_tuple) const;
	Tuple operator*(const double & scalar) const;
	Tuple operator/(const double & scalar) const;
 
};

// Overloaded Operators
std::ostream & operator<<(std::ostream & os, const Tuple & tuple);
bool operator==(const Tuple & left_tuple, const Tuple & right_tuple);
bool operator!=(const Tuple & left_tuple, const Tuple & right_tuple);

// Helper Functions
bool flt_cmp(const double & left_double, const double & right_double);

#endif
