#include "pch.h"
#include "Light.h"

// ------------------------------------------------------------------------
//
// Light
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Light::Light() : Light(Color(1.0, 1.0, 1.0), 1.0)
{
}

Light::Light(Color color) : Light(color, 1.0)
{
}

Light::Light(Color color, double multiplier) : ObjectBase()
{
	this->color = color;
	this->multiplier = multiplier;

	this->falloff = false;
	this->cutoff = 0.0001;
}

Light::~Light()
{
}

Tuple Light::position() const
{
	return (this->get_transform()).position();
}

// ------------------------------------------------------------------------
//
// PointLight
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

PointLight::PointLight() : PointLight(Tuple::Point(0.0, 0.0, 0.0), Color(1.0), 1.0)
{
}

PointLight::PointLight(Tuple position, Color color) : PointLight(position, color, 1.0)
{
}

PointLight::PointLight(Tuple position, Color color, double multiplier) : Light(color, multiplier)
{
	this->set_transform(Matrix4::Translation(position));
}

PointLight::~PointLight()
{
}

std::vector<double> PointLight::local_intersect_t(const Ray & ray) const
{
	Tuple position = this->position();
	Tuple ix_vector = position - ray.origin;
	if (ix_vector.normalize() == ray.direction.normalize())
	{
		return std::vector<double>({ Tuple::distance(position, ray.origin) });
	}
	else
	{
		return std::vector<double>();
	}
}

Tuple PointLight::local_normal_at(const Tuple & point) const
{
	return Tuple::Vector(0.0, 1.0, 0.0);
}

bool operator==(const PointLight & left_light, const PointLight & right_light)
{
	return (
		left_light.color == right_light.color &&
		left_light.position() == right_light.position()
		);
}

bool operator!=(const PointLight & left_light, const PointLight & right_light)
{
	return !(left_light == right_light);
}