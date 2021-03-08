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
	this->radius = 1.0f;
	this->set_name("Default Sphere 000");
}

Sphere::Sphere(std::string name)
{
	this->radius = 1.0f;
	this->set_name(name);
}

Sphere::~Sphere()
{
}

std::vector<float> Sphere::local_intersect_t(const Ray & r) const
{
	std::vector<float> result = std::vector<float>();

	// The vector from the sphere's center to the ray origin
	Tuple sphere_to_ray = r.origin - Tuple::Point(0.0f, 0.0f, 0.0f);

	// Calculate discriminant
	float a = Tuple::dot(r.direction, r.direction);
	float b = 2.0f * Tuple::dot(r.direction, sphere_to_ray);
	float c = Tuple::dot(sphere_to_ray, sphere_to_ray) - 1;

	float discriminant = (b * b) - (4 * a * c);

	if (discriminant >= 0)
	{
		// resize vector
		result.resize(2);

		float sqrt_discriminant = sqrt(discriminant);

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

Tuple Sphere::normal_at(const Tuple & world_space_point) const
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
	world_normal_vector.w = 0.0f;

	// Normalize vector
	return world_normal_vector.normalize();
}

