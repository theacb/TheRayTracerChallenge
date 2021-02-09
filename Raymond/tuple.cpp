#include "pch.h"
#include "tuple.h"

const float EPSILON = 0.00001f;

// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Tuple::Tuple()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Tuple::Tuple(float x_axis, float y_axis, float z_axis, float w_axis)
{
	x = x_axis;
	y = y_axis;
	z = z_axis;
	w = w_axis;
}

Tuple Tuple::Point(float x_axis, float y_axis, float z_axis)
{
	return Tuple(x_axis, y_axis, z_axis, 1.0f);
}

Tuple Tuple::Vector(float x_axis, float y_axis, float z_axis)
{
	return Tuple(x_axis, y_axis, z_axis, 0.0f);
}

// Destructor

Tuple::~Tuple()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

// Magnitude
float Tuple::magnitude()
{
	// Computes magnitude using Pythagorean theorem
	// sqrt(x^2 + y^2 + z^2 + w^2)
	const float power = 2.0f;
	return sqrt(pow(x, power) + pow(y, power) + pow(z, power) + pow(w, power));
}

// Normalize
Tuple Tuple::normalize()
{
	const float mag = this->magnitude();
	return Tuple(x / mag, y / mag, z / mag, w / mag);
}

// Dot product
float Tuple::dot(const Tuple & left_tuple, const Tuple & right_tuple)
{
	return (
		(left_tuple.x * right_tuple.x) + 
		(left_tuple.y * right_tuple.y) + 
		(left_tuple.z * right_tuple.z) + 
		(left_tuple.w * right_tuple.w)
		);
}

Tuple Tuple::cross(const Tuple & left_tuple, const Tuple & right_tuple)
{
	return Tuple(
		(left_tuple.y * right_tuple.z) - (left_tuple.z * right_tuple.y),
		(left_tuple.z * right_tuple.x) - (left_tuple.x * right_tuple.z),
		(left_tuple.x * right_tuple.y) - (left_tuple.y * right_tuple.x),
		0.0f

	);
}

// ------------------------------------------------------------------------
// Overloaded Operators
// ------------------------------------------------------------------------

// Addition
Tuple Tuple::operator+(Tuple const & right_tuple) const
{
	return Tuple(
		x + right_tuple.x, 
		y + right_tuple.y, 
		z + right_tuple.z, 
		w + right_tuple.w);
}

// Subtraction
Tuple Tuple::operator-(const Tuple & right_tuple) const
{
	return Tuple(
		x - right_tuple.x, 
		y - right_tuple.y, 
		z - right_tuple.z, 
		w - right_tuple.w
	);
}

// Scalar Multiplication
Tuple Tuple::operator*(const float & scalar) const
{
	return Tuple(
		x * scalar, 
		y * scalar, 
		z * scalar, 
		w * scalar
	);
}

// Scalar Division
Tuple Tuple::operator/(const float & scalar) const
{
	return Tuple(
		x / scalar, 
		y / scalar, 
		z / scalar, 
		w / scalar
	);
}

// Negate (Unary Minus)
Tuple Tuple::operator-()
{
	return Tuple(
		0 - x, 
		0 - y, 
		0 - z, 
		0 - w
	);
}

// ------------------------------------------------------------------------
// External Overloaded Operators
// ------------------------------------------------------------------------

// Representation with std::cout
std::ostream & operator<< (std::ostream & os, const Tuple & tuple)
{
	os << "(" << tuple.x << ", " << tuple.y << ", " << tuple.z << ", " << tuple.w << ")";
	return os;
}

// Equality
bool operator==(const Tuple & left_tuple, const Tuple & right_tuple)
{
	return (
		flt_cmp(left_tuple.x, right_tuple.x) &&
		flt_cmp(left_tuple.y, right_tuple.y) &&
		flt_cmp(left_tuple.z, right_tuple.z) &&
		flt_cmp(left_tuple.w, right_tuple.w)
		);
}

// ------------------------------------------------------------------------
// Helpfer Functions
// ------------------------------------------------------------------------

// Float Comparison
bool flt_cmp(float left_float, float right_float)
{
	return fabs(left_float - right_float) < EPSILON;
}
