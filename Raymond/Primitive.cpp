#include "pch.h"
#include "Primitive.h"

// ------------------------------------------------------------------------
//
// Sphere
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Primitive::Primitive() : ObjectBase()
{
	this->set_name("Default Primitive 000");
	this->material = std::make_shared<PhongMaterial>();
}


Primitive::~Primitive()
{
}

// ------------------------------------------------------------------------
//
// Sphere
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Sphere::Sphere() : Primitive()
{
	this->radius = 1.0;
	this->set_name("Default Sphere 000");
}

Sphere::Sphere(std::string name)
{
	this->radius = 1.0;
	this->set_name(name);
}

Sphere::~Sphere()
{
}

std::vector<double> Sphere::local_intersect_t(const Ray & r) const
{
	std::vector<double> result = std::vector<double>();

	// The vector from the sphere's center to the ray origin
	Tuple sphere_to_ray = r.origin - Tuple::Point(0.0, 0.0, 0.0);

	// Calculate discriminant
	double a = Tuple::dot(r.direction, r.direction);
	double b = 2.0 * Tuple::dot(r.direction, sphere_to_ray);
	double c = Tuple::dot(sphere_to_ray, sphere_to_ray) - 1;

	double discriminant = (b * b) - (4 * a * c);

	if (discriminant >= 0)
	{
		// resize vector
		result.resize(2);

		double sqrt_discriminant = sqrt(discriminant);

		// Calculate intersections
		result[0] = (-b - sqrt_discriminant) / (2 * a);
		result[1] = (-b + sqrt_discriminant) / (2 * a);

		return result;
	}
	else
	{
		return result;
	}
}

Tuple Sphere::local_normal_at(const Tuple & object_space_point) const
{
	// Calculate surface normal
	return object_space_point - Tuple::Origin();
}

// ------------------------------------------------------------------------
//
// Infinite Plane
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

InfinitePlane::InfinitePlane()
{
	this->set_name("Default Sphere 000");
}

InfinitePlane::InfinitePlane(std::string name)
{
	this->set_name(name);
}

InfinitePlane::~InfinitePlane()
{
}

std::vector<double> InfinitePlane::local_intersect_t(const Ray & r) const
{
	// Parallel or coplanar rays return empty vector
	if (abs(r.direction.y) < EPSILON)
	{
		return std::vector<double>();
	}
	// Assumes that the object space plane is on the X and Z axis
	else
	{
		return std::vector<double>({ (-r.origin.y) / r.direction.y });
	}
}

Tuple InfinitePlane::local_normal_at(const Tuple & object_space_point) const
{
	// Assumes that the object space plane is on the X and Z axis, pointing in the positive y direction
	return Tuple::Vector(0.0, 1.0, 0.0);
}

// ------------------------------------------------------------------------
//
// Test Shape
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

TestShape::TestShape()
{
}

TestShape::TestShape(std::string)
{
}

TestShape::~TestShape()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> TestShape::local_intersect_t(const Ray & r) const
{
	return std::vector<double>();
}

Tuple TestShape::local_normal_at(const Tuple & object_space_point) const
{
	return Tuple::Vector(object_space_point.x, object_space_point.y, object_space_point.z);
}


