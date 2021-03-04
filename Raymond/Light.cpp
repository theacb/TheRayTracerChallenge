#include "pch.h"
#include "Light.h"

// ------------------------------------------------------------------------
//
// Light
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Light::Light() : Light(Color(1.0f, 1.0f, 1.0f))
{
}

Light::Light(Color color) : ObjectBase()
{
	this->color = color;
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

PointLight::PointLight() : Light()
{
}

PointLight::PointLight(Tuple position, Color color) : Light (color)
{
	this->set_transform(Matrix4::Translation(position));
}

PointLight::~PointLight()
{
}

std::vector<float> PointLight::intersect_t(Ray & r) const
{
	Tuple position = this->position();
	Tuple ix_vector = position - r.origin;
	if (ix_vector.normalize() == r.direction.normalize())
	{
		return std::vector<float>({ Tuple::distance(position, r.origin) });
	}
	else
	{
		return std::vector<float>();
	}
}

Tuple PointLight::normal_at(Tuple &) const
{
	return Tuple::Vector(0.0f, 1.0f, 0.0f);
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
