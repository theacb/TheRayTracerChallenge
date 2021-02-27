#include "pch.h"
#include "Tuple.h"

const float EPSILON = 0.00001f;

// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Tuple::Tuple()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;
}

Tuple::Tuple(float x_axis, float y_axis, float z_axis, float w_axis)
{
	this->x = x_axis;
	this->y = y_axis;
	this->z = z_axis;
	this->w = w_axis;
}

Tuple::Tuple(const Tuple & src)
{
	this->x = src.x;
	this->y = src.y;
	this->z = src.z;
	this->w = src.w;
}

Tuple Tuple::Point(float x_axis, float y_axis, float z_axis)
{
	return Tuple(x_axis, y_axis, z_axis, 1.0f);
}

Tuple Tuple::Origin()
{
	return Tuple::Point(0.0f, 0.0f, 0.0f);
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
float Tuple::magnitude() const
{
	// Computes magnitude using Pythagorean theorem
	// sqrt(x^2 + y^2 + z^2 + w^2)
	const float power = 2.0f;
	return sqrt(pow(this->x, power) + pow(this->y, power) + pow(this->z, power) + pow(this->w, power));
}

// Normalize
Tuple Tuple::normalize() const
{
	float mag = this->magnitude();
	return Tuple(this->x / mag, this->y / mag, this->z / mag, this->w / mag);
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

float Tuple::distance(const Tuple & left_tuple, const Tuple & right_tuple)
{
	const float power = 2.0f;
	return sqrt(
		pow(right_tuple.x - left_tuple.x, power) +
		pow(right_tuple.y - left_tuple.y, power) +
		pow(right_tuple.z - left_tuple.z, power) +
		pow(right_tuple.w - left_tuple.w, power)
	);
}

Tuple Tuple::reflect(const Tuple & in, const Tuple & normal)
{
	return in - (normal * (2 * Tuple::dot(in, normal)));
}

Tuple Tuple::entry_wise(const Tuple & left_tuple, const Tuple & right_tuple)
{
	return Tuple(
		left_tuple.x * right_tuple.x,
		left_tuple.y * right_tuple.y,
		left_tuple.z * right_tuple.z,
		left_tuple.w * right_tuple.w
		);
}

// ------------------------------------------------------------------------
// Overloaded Operators
// ------------------------------------------------------------------------

// Addition
Tuple Tuple::operator+(Tuple const & right_tuple) const
{
	return Tuple(
		this->x + right_tuple.x,
		this->y + right_tuple.y,
		this->z + right_tuple.z,
		this->w + right_tuple.w);
}

// Subtraction
Tuple Tuple::operator-(const Tuple & right_tuple) const
{
	return Tuple(
		this->x - right_tuple.x,
		this->y - right_tuple.y,
		this->z - right_tuple.z,
		this->w - right_tuple.w
	);
}

// Scalar Multiplication
Tuple Tuple::operator*(const float & scalar) const
{
	return Tuple(
		this->x * scalar,
		this->y * scalar,
		this->z * scalar,
		this->w * scalar
	);
}

// Scalar Division
Tuple Tuple::operator/(const float & scalar) const
{
	return Tuple(
		this->x / scalar,
		this->y / scalar,
		this->z / scalar,
		this->w / scalar
	);
}

// Negate (Unary Minus)
Tuple Tuple::operator-()
{
	return Tuple(
		0 - this->x,
		0 - this->y,
		0 - this->z,
		0 - this->w
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

bool operator!=(const Tuple & left_tuple, const Tuple & right_tuple)
{
	return !(left_tuple == right_tuple);
}

// ------------------------------------------------------------------------
// Helpfer Functions
// ------------------------------------------------------------------------

// Float Comparison
bool flt_cmp(const float & left_float, const float & right_float)
{
	return fabs(left_float - right_float) < EPSILON;
}
