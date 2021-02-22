#include "pch.h"
#include "Ray.h"

// ------------------------------------------------------------------------
//
// Ray
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Ray::Ray():Ray(Tuple::Point(0.0f, 0.0f, 0.0f), Tuple::Vector(0.0f, 1.0f, 0.0f))
{
}

Ray::Ray(Tuple origin, Tuple direction)
{
	this->origin = origin;
	this->direction = direction;
}


Ray::~Ray()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

Tuple Ray::position(float t) const
{
	return this->origin + (this->direction * t);
}

Ray Ray::transform(Matrix4 m) const
{
	return Ray(m * this->origin, m * this->direction);
}

std::ostream & operator<<(std::ostream & os, const Ray & r)
{
	return os << "<" << r.origin << ", " << r.direction << ">";
}
