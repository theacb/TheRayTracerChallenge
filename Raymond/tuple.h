#ifndef H_RAYMOND_TUPLE
#define H_RAYMOND_TUPLE

#include <iostream>
#include <cmath>

#include "Constants.h"
#include "Utilities.h"

class Tuple
{
public:
	// Constructors
	Tuple();
	Tuple(double x_axis, double y_axis, double z_axis, double w_axis);
	Tuple(const Tuple & src);

	// Factories
	static Tuple Point(double x_axis, double y_axis, double z_axis);
	static Tuple Point2D(double x_axis, double y_axis);
	static Tuple Origin();
	static Tuple Vector(double x_axis, double y_axis, double z_axis);
	static Tuple RandomVector(double min, double max);
	static Tuple RandomInUnitSphere();

	// Destructor
	~Tuple();

	// Properties
	double x, y, z, w;

	// Methods
	double magnitude() const;
	double magnitude_squared() const;
	Tuple normalize() const;
	Tuple multiplicative_inverse() const;

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
	double & operator[](int i);
	const double & operator[](int i) const;
 
};

// Overloaded Operators
std::ostream & operator<<(std::ostream & os, const Tuple & tuple);
bool operator==(const Tuple & left_tuple, const Tuple & right_tuple);
bool operator!=(const Tuple & left_tuple, const Tuple & right_tuple);
Tuple operator*(const double & scalar, const Tuple & right_tuple);

// Helper Functions
double safe_divide(const double a, const double b);

#endif
