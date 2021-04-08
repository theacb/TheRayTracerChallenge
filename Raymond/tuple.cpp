#include "pch.h"
#include "Tuple.h"

// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Tuple::Tuple()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->w = 0.0;
}

Tuple::Tuple(double x_axis, double y_axis, double z_axis, double w_axis)
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

Tuple Tuple::Point(double x_axis, double y_axis, double z_axis)
{
	return Tuple(x_axis, y_axis, z_axis, 1.0);
}

Tuple Tuple::Origin()
{
	return Tuple::Point(0.0, 0.0, 0.0);
}

Tuple Tuple::Vector(double x_axis, double y_axis, double z_axis)
{
	return Tuple(x_axis, y_axis, z_axis, 0.0);
}

Tuple Tuple::RandomVector(double min, double max)
{
	return Tuple::Vector(random_double(min, max), random_double(min, max), random_double(min, max));
}

// Taken from Ray Tracing in One Weekend by Peter Shirley
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/asimplediffusematerial
Tuple Tuple::RandomInUnitSphere()
{
	while (true) {
		Tuple p = Tuple::RandomVector(-1.0, 1.0);
		if (p.magnitude_squared() >= 1.0) continue;
		return p;
	}
}

// Destructor

Tuple::~Tuple()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

// Magnitude
double Tuple::magnitude() const
{
	// Computes magnitude using Pythagorean theorem
	// sqrt(magnitude_squared())
	
	return sqrt(this->magnitude_squared());
}

double Tuple::magnitude_squared() const
{
	// Computes magnitude using Pythagorean theorem
	// x^2 + y^2 + z^2 + w^2
	return (this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w);
}

// Normalize
Tuple Tuple::normalize() const
{
	double mag = this->magnitude();
	return Tuple(this->x / mag, this->y / mag, this->z / mag, this->w / mag);
}

// Dot product
double Tuple::dot(const Tuple & left_tuple, const Tuple & right_tuple)
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
		0.0

	);
}

double Tuple::distance(const Tuple & left_tuple, const Tuple & right_tuple)
{
	const double power = 2.0;
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
Tuple Tuple::operator*(const double & scalar) const
{
	return Tuple(
		this->x * scalar,
		this->y * scalar,
		this->z * scalar,
		this->w * scalar
	);
}

// Scalar Division
Tuple Tuple::operator/(const double & scalar) const
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

Tuple operator*(const double & scalar, const Tuple & right_tuple)
{
	return right_tuple * scalar;
}

// ------------------------------------------------------------------------
// Helpfer Functions
// ------------------------------------------------------------------------
