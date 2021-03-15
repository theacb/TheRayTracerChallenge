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

Tuple Sphere::local_normal_at(const Tuple & world_space_point) const
{
	// Calculate inverse of xform vector
	Matrix4 inverted_xform_matrix = (this->get_transform()).inverse();

	// Multiply point by inverse xform matrix to bring into object space
	Tuple object_space_point = inverted_xform_matrix * world_space_point;

	// Calculate surface normal
	Tuple object_normal_vector = object_space_point - Tuple::Origin();

	// Multiply object space normal by transpose of the inverted x form matrix
	Tuple world_normal_vector = inverted_xform_matrix.transpose() * object_normal_vector;

	// Enforce behavior as a vector by setting w to 0
	world_normal_vector.w = 0.0;

	// Normalize vector
	return world_normal_vector.normalize();
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

std::vector<double> TestShape::local_intersect_t(const Ray &) const
{
	return std::vector<double>();
}

Tuple TestShape::local_normal_at(const Tuple &) const
{
	return Tuple();
}
